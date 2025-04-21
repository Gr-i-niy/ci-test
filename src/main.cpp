#include <fmt/core.h>
#include <sqlite3.h>
#include "finance_manager.h"
#include <iostream>

int main() {
  fmt::print("Finance Manager v0.1.0\n");
  fmt::print("SQLite version: {}\n", sqlite3_libversion());

  FinanceManager manager("finance.db");
  if (!manager.initialize()) {
    fmt::print(stderr, "Failed to initialize database\n");
    return 1;
  }

  // Добавляем несколько примеров
  manager.addTransaction(Transaction(TransactionType::INCOME, 1000.0, "Salary", "Monthly salary"));
  manager.addTransaction(Transaction(TransactionType::EXPENSE, 300.0, "Food", "Groceries"));
  manager.addTransaction(Transaction(TransactionType::EXPENSE, 500.0, "Rent", "Monthly rent"));

  // Выводим статистику
  fmt::print("Total income: ${:.2f}\n", manager.getTotalIncome());
  fmt::print("Total expenses: ${:.2f}\n", manager.getTotalExpense());
  fmt::print("Current balance: ${:.2f}\n", manager.getBalance());

  fmt::print("\nAll transactions:\n");
  for (const auto& trx : manager.getAllTransactions()) {
    fmt::print("[{}] {} - ${:.2f} - {}: {}\n",
        trx.getDate(),
        trx.getType() == TransactionType::INCOME ? "INCOME" : "EXPENSE",
        trx.getAmount(),
        trx.getCategory(),
        trx.getDescription()
    );
  }

  return 0;
}
