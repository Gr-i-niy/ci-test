#include "finance_manager.h"
#include <fmt/core.h>

FinanceManager::FinanceManager(const std::string& dbPath) : dbPath_(dbPath) {}

FinanceManager::~FinanceManager() {
    if (db_) {
        sqlite3_close(db_);
    }
}

bool FinanceManager::initialize() {
    int rc = sqlite3_open(dbPath_.c_str(), &db_);
    if (rc != SQLITE_OK) {
        fmt::print(stderr, "Cannot open database: {}\n", sqlite3_errmsg(db_));
        sqlite3_close(db_);
        return false;
    }

    // Создаем таблицу для транзакций, если она не существует
    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS transactions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "type INTEGER NOT NULL,"
        "amount REAL NOT NULL,"
        "category TEXT NOT NULL,"
        "description TEXT,"
        "date TEXT NOT NULL"
        ");";

    return executeQuery(createTableSQL);
}

bool FinanceManager::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        fmt::print(stderr, "SQL error: {}\n", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool FinanceManager::addTransaction(const Transaction& transaction) {
    std::string query = fmt::format(
        "INSERT INTO transactions (type, amount, category, description, date) "
        "VALUES ({}, {}, '{}', '{}', '{}');",
        static_cast<int>(transaction.getType()),
        transaction.getAmount(),
        transaction.getCategory(),
        transaction.getDescription(),
        transaction.getDate()
    );

    return executeQuery(query);
}

bool FinanceManager::removeTransaction(int id) {
    std::string query = fmt::format("DELETE FROM transactions WHERE id = {};", id);
    return executeQuery(query);
}

std::vector<Transaction> FinanceManager::executeTransactionQuery(const std::string& query) {
    std::vector<Transaction> transactions;
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fmt::print(stderr, "Failed to prepare statement: {}\n", sqlite3_errmsg(db_));
        return transactions;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        TransactionType type = static_cast<TransactionType>(sqlite3_column_int(stmt, 1));
        double amount = sqlite3_column_double(stmt, 2);
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        transactions.emplace_back(id, type, amount, category, description, date);
    }

    sqlite3_finalize(stmt);
    return transactions;
}

std::optional<Transaction> FinanceManager::getTransaction(int id) {
    std::string query = fmt::format("SELECT * FROM transactions WHERE id = {};", id);
    auto transactions = executeTransactionQuery(query);

    if (transactions.empty()) {
        return std::nullopt;
    }

    return transactions[0];
}

std::vector<Transaction> FinanceManager::getAllTransactions() {
    return executeTransactionQuery("SELECT * FROM transactions ORDER BY date DESC;");
}

std::vector<Transaction> FinanceManager::getTransactionsByType(TransactionType type) {
    std::string query = fmt::format(
        "SELECT * FROM transactions WHERE type = {} ORDER BY date DESC;",
        static_cast<int>(type)
    );
    return executeTransactionQuery(query);
}

std::vector<Transaction> FinanceManager::getTransactionsByCategory(const std::string& category) {
    std::string query = fmt::format(
        "SELECT * FROM transactions WHERE category = '{}' ORDER BY date DESC;",
        category
    );
    return executeTransactionQuery(query);
}

double FinanceManager::getTotalIncome() {
    sqlite3_stmt* stmt;
    std::string query = fmt::format(
        "SELECT SUM(amount) FROM transactions WHERE type = {};",
        static_cast<int>(TransactionType::INCOME)
    );

    int rc = sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fmt::print(stderr, "Failed to prepare statement: {}\n", sqlite3_errmsg(db_));
        return 0.0;
    }

    double total = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}

double FinanceManager::getTotalExpense() {
    sqlite3_stmt* stmt;
    std::string query = fmt::format(
        "SELECT SUM(amount) FROM transactions WHERE type = {};",
        static_cast<int>(TransactionType::EXPENSE)
    );

    int rc = sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fmt::print(stderr, "Failed to prepare statement: {}\n", sqlite3_errmsg(db_));
        return 0.0;
    }

    double total = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}

double FinanceManager::getBalance() {
    return getTotalIncome() - getTotalExpense();
}
