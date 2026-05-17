#pragma once
#include <string>

enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER_DEBIT,
    TRANSFER_CREDIT,
    INTEREST,
    FEE
};

class Transaction {
private:
    static int nextId;
    int         id;
    std::string date;
    TransactionType type;
    double      amount;
    double      balanceAfter;
    std::string description;
    std::string fromIBAN;
    std::string toIBAN;

public:
    Transaction(TransactionType type, double amount, double balanceAfter,
                const std::string& description,
                const std::string& fromIBAN = "",
                const std::string& toIBAN   = "");

    int             getId()           const { return id; }
    std::string     getDate()         const { return date; }
    TransactionType getType()         const { return type; }
    double          getAmount()       const { return amount; }
    double          getBalanceAfter() const { return balanceAfter; }
    std::string     getDescription()  const { return description; }
    std::string     getFromIBAN()     const { return fromIBAN; }
    std::string     getToIBAN()       const { return toIBAN; }

    std::string getDetails() const;
    static std::string typeToString(TransactionType t);
    static std::string currentTimestamp();
};
