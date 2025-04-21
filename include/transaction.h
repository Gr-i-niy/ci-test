#pragma once

#include <string>
#include <chrono>

enum class TransactionType {
  INCOME,
  EXPENSE
};

class Transaction {
public:
  Transaction(int id, TransactionType type, double amount,
              const std::string& category, const std::string& description,
              const std::string& date);

  Transaction(TransactionType type, double amount,
              const std::string& category, const std::string& description);

  int getId() const { return id_; }
  TransactionType getType() const { return type_; }
  double getAmount() const { return amount_; }
  const std::string& getCategory() const { return category_; }
  const std::string& getDescription() const { return description_; }
  const std::string& getDate() const { return date_; }

private:
  int id_ = -1;
  TransactionType type_;
  double amount_;
  std::string category_;
  std::string description_;
  std::string date_;

  std::string getCurrentDate() const;
};
