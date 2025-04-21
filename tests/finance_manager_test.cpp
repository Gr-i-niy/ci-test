#include <gtest/gtest.h>
#include "finance_manager.h"
#include <filesystem>

class FinanceManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Используем временную БД для тестов
        testDbPath = "test_finance.db";

        // Удалим старую тестовую БД, если осталась
        if (std::filesystem::exists(testDbPath)) {
            std::filesystem::remove(testDbPath);
        }

        manager = std::make_unique<FinanceManager>(testDbPath);
        manager->initialize();
    }

    void TearDown() override {
        manager.reset();
        if (std::filesystem::exists(testDbPath)) {
            std::filesystem::remove(testDbPath);
        }
    }

    std::string testDbPath;
    std::unique_ptr<FinanceManager> manager;
};

TEST_F(FinanceManagerTest, AddTransaction) {
    Transaction trx(TransactionType::INCOME, 1000.0, "Salary", "Monthly salary");
    EXPECT_TRUE(manager->addTransaction(trx));

    auto transactions = manager->getAllTransactions();
    ASSERT_EQ(transactions.size(), 1);
    EXPECT_EQ(transactions[0].getType(), TransactionType::INCOME);
    EXPECT_DOUBLE_EQ(transactions[0].getAmount(), 1000.0);
    EXPECT_EQ(transactions[0].getCategory(), "Salary");
    EXPECT_EQ(transactions[0].getDescription(), "Monthly salary");
}

TEST_F(FinanceManagerTest, GetTransactionById) {
    Transaction trx(TransactionType::INCOME, 1000.0, "Salary", "Monthly salary");
    manager->addTransaction(trx);

    auto transactions = manager->getAllTransactions();
    ASSERT_FALSE(transactions.empty());

    int id = transactions[0].getId();
    auto optTrx = manager->getTransaction(id);
    ASSERT_TRUE(optTrx.has_value());
    EXPECT_EQ(optTrx->getType(), TransactionType::INCOME);
    EXPECT_DOUBLE_EQ(optTrx->getAmount(), 1000.0);
}

TEST_F(FinanceManagerTest, RemoveTransaction) {
    Transaction trx(TransactionType::INCOME, 1000.0, "Salary", "Monthly salary");
    manager->addTransaction(trx);

    auto transactions = manager->getAllTransactions();
    ASSERT_FALSE(transactions.empty());

    int id = transactions[0].getId();
    EXPECT_TRUE(manager->removeTransaction(id));

    transactions = manager->getAllTransactions();
    EXPECT_TRUE(transactions.empty());
}

TEST_F(FinanceManagerTest, GetTransactionsByType) {
    manager->addTransaction(Transaction(TransactionType::INCOME, 1000.0, "Salary", "Monthly salary"));
    manager->addTransaction(Transaction(TransactionType::EXPENSE, 300.0, "Food", "Groceries"));
    manager->addTransaction(Transaction(TransactionType::INCOME, 200.0, "Interest", "Bank interest"));

    auto incomes = manager->getTransactionsByType(TransactionType::INCOME);
    EXPECT_EQ(incomes.size(), 2);

    auto expenses = manager->getTransactionsByType(TransactionType::EXPENSE);
    EXPECT_EQ(expenses.size(), 1);
}

TEST_F(FinanceManagerTest, GetTotalAmounts) {
    manager->addTransaction(Transaction(TransactionType::INCOME, 1000.0, "Salary", "Monthly salary"));
    manager->addTransaction(Transaction(TransactionType::EXPENSE, 300.0, "Food", "Groceries"));
    manager->addTransaction(Transaction(TransactionType::INCOME, 200.0, "Interest", "Bank interest"));
    manager->addTransaction(Transaction(TransactionType::EXPENSE, 150.0, "Transport", "Bus tickets"));

    EXPECT_DOUBLE_EQ(manager->getTotalIncome(), 1200.0);
    EXPECT_DOUBLE_EQ(manager->getTotalExpense(), 450.0);
    EXPECT_DOUBLE_EQ(manager->getBalance(), 750.0);
}
