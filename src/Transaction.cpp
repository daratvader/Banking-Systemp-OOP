#include "Transaction.h"
#include <ctime>
#include <sstream>
#include <iomanip>

int Transaction::nextId = 1;

std::string Transaction::currentTimestamp() {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

Transaction::Transaction(TransactionType type, double amount, double balanceAfter,
                         const std::string& description,
                         const std::string& fromIBAN,
                         const std::string& toIBAN)
    : id(nextId++), date(currentTimestamp()), type(type),
      amount(amount), balanceAfter(balanceAfter),
      description(description), fromIBAN(fromIBAN), toIBAN(toIBAN) {}

std::string Transaction::typeToString(TransactionType t) {
    switch (t) {
        case TransactionType::DEPOSIT:          return "DEPOSIT";
        case TransactionType::WITHDRAWAL:       return "WITHDRAWAL";
        case TransactionType::TRANSFER_DEBIT:   return "TRANSFER OUT";
        case TransactionType::TRANSFER_CREDIT:  return "TRANSFER IN";
        case TransactionType::INTEREST:         return "INTEREST";
        case TransactionType::FEE:              return "FEE";
        default:                                return "UNKNOWN";
    }
}

std::string Transaction::getDetails() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "[#" << id << " | " << date << "] "
        << std::left << std::setw(14) << typeToString(type)
        << " | Amount: " << std::setw(10) << amount
        << " | Balance: " << balanceAfter;
    if (!description.empty())
        oss << " | " << description;
    return oss.str();
}
