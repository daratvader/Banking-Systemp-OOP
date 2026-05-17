#pragma once
#include "TransactionHistory.h"
#include "Exceptions.h"
#include <string>

class Account {
protected:
    std::string iban;           // readable by subclasses

    // --- Protected helpers used ONLY by subclasses (e.g. interest, fees) ---
    // This is the controlled exception to private balance: subclasses may
    // adjust the balance for specialised rule-based operations, but still
    // produce a proper Transaction record.
    void addToBalance(double delta, TransactionType type, const std::string& desc);

    // Template-method hook: subclasses override to customise withdrawal check
    virtual bool canWithdraw(double amount) const;

private:
    double             balance;
    std::string        ownerCustomerId;
    TransactionHistory history;
    bool               active;

public:
    Account(const std::string& iban,
            const std::string& ownerCustomerId,
            double initialBalance = 0.0);
    virtual ~Account() = default;

    // Core financial operations
    void deposit(double amount, const std::string& desc = "Deposit");
    void withdraw(double amount, const std::string& desc = "Withdrawal");

    // Transfer helpers (create correct transaction types)
    void creditTransfer(double amount, const std::string& fromIBAN);
    void debitTransfer (double amount, const std::string& toIBAN);

    // Accessors
    double      getBalance()         const { return balance; }
    std::string getIban()            const { return iban; }
    std::string getOwnerCustomerId() const { return ownerCustomerId; }
    bool        isActive()           const { return active; }

    const TransactionHistory& getHistory() const { return history; }

    void close() { active = false; }

    // --- Pure-virtual interface ---
    virtual void applyMonthlyRules()      = 0;
    virtual std::string getAccountType()  const = 0;
    virtual void printInfo()              const;
};
