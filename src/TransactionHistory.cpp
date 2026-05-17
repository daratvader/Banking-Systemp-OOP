#include "TransactionHistory.h"
#include <iostream>
#include <iomanip>

void TransactionHistory::add(const Transaction& t) {
    transactions.push_back(t);
}

const std::vector<Transaction>& TransactionHistory::getAll() const {
    return transactions;
}

std::vector<Transaction> TransactionHistory::filterByDate(const std::string& from,
                                                          const std::string& to) const {
    std::vector<Transaction> result;
    for (const auto& t : transactions) {
        if ((from.empty() || t.getDate() >= from) &&
            (to.empty()   || t.getDate() <= to + " 23:59:59"))
            result.push_back(t);
    }
    return result;
}

void TransactionHistory::printStatement(const std::string& iban,
                                        double openingBalance,
                                        const std::string& from,
                                        const std::string& to) const {
    auto list = (from.empty() && to.empty()) ? transactions : filterByDate(from, to);

    std::cout << "\n" << std::string(65, '=') << "\n";
    std::cout << "  ACCOUNT STATEMENT  |  IBAN: " << iban << "\n";
    if (!from.empty() || !to.empty())
        std::cout << "  Period: " << (from.empty() ? "start" : from)
                  << " to " << (to.empty() ? "now" : to) << "\n";
    std::cout << std::string(65, '=') << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Opening balance: " << openingBalance << " BGN\n";
    std::cout << std::string(65, '-') << "\n";

    for (const auto& t : list)
        std::cout << "  " << t.getDetails() << "\n";

    std::cout << std::string(65, '-') << "\n";
    if (!list.empty())
        std::cout << "  Closing balance: " << list.back().getBalanceAfter() << " BGN\n";
    std::cout << std::string(65, '=') << "\n";
}
