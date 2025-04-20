#include "transaction.h"
#include <chrono>
#include <iomanip>
#include <sstream>

Transaction::Transaction(int id, TransactionType type, double amount,
                        const std::string& category, const std::string& description,
                        const std::string& date)
    : id_(id), type_(type), amount_(amount), category_(category),
      description_(description), date_(date) {}

Transaction::Transaction(TransactionType type, double amount,
                        const std::string& category, const std::string& description)
    : type_(type), amount_(amount), category_(category),
      description_(description), date_(getCurrentDate()) {}

std::string Transaction::getCurrentDate() const {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}
