#pragma once
#include "Transaction.h"
#include <vector>
#include <string>

class TransactionHistory {
private:
    std::vector<Transaction> transactions;

public:
    void add(const Transaction& t);
    const std::vector<Transaction>& getAll() const;

    // Returns transactions whose date string falls in [from, to] lexicographically.
    std::vector<Transaction> filterByDate(const std::string& from,
                                          const std::string& to) const;

    void printStatement(const std::string& iban,
                        double openingBalance,
                        const std::string& from = "",
                        const std::string& to   = "") const;
};
