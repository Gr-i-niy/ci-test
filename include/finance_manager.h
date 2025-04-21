#pragma once

#include "transaction.h"
#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>

class FinanceManager {
public:
  explicit FinanceManager(const std::string& dbPath);
  ~FinanceManager();

  FinanceManager(const FinanceManager&) = delete;
  FinanceManager& operator=(const FinanceManager&) = delete;

  bool initialize();
  bool addTransaction(const Transaction& transaction);
  bool removeTransaction(int id);
  std::optional<Transaction> getTransaction(int id);
  std::vector<Transaction> getAllTransactions();
  std::vector<Transaction> getTransactionsByType(TransactionType type);
  std::vector<Transaction> getTransactionsByCategory(const std::string& category);
  double getTotalIncome();
  double getTotalExpense();
  double getBalance();

private:
  sqlite3* db_ = nullptr;
  std::string dbPath_;

  bool executeQuery(const std::string& query);
  std::vector<Transaction> executeTransactionQuery(const std::string& query);
};