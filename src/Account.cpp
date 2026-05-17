#include "Account.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ── Constructor ────────────────────────────────────────────────────────────
Account::Account(const std::string& iban,
                 const std::string& ownerCustomerId,
                 double initialBalance)
    : iban(iban), balance(initialBalance),
      ownerCustomerId(ownerCustomerId), active(true) {}

// ── Protected helpers ──────────────────────────────────────────────────────

// Controlled balance mutation used by applyMonthlyRules() in subclasses.
// Always records a proper Transaction so the audit trail is never broken.
void Account::addToBalance(double delta, TransactionType type, const std::string& desc) {
    balance += delta;
    history.add(Transaction(type, std::abs(delta), balance, desc, iban));
}

bool Account::canWithdraw(double amount) const {
    return balance >= amount;   // default: no overdraft
}

// ── Public operations ──────────────────────────────────────────────────────

void Account::deposit(double amount, const std::string& desc) {
    if (amount <= 0) throw std::invalid_argument("Deposit amount must be positive");
    if (!active)    throw std::runtime_error("Cannot operate on closed account");
    balance += amount;
    history.add(Transaction(TransactionType::DEPOSIT, amount, balance, desc, "", iban));
}

void Account::withdraw(double amount, const std::string& desc) {
    if (amount <= 0) throw std::invalid_argument("Withdrawal amount must be positive");
    if (!active)    throw std::runtime_error("Cannot operate on closed account");
    if (!canWithdraw(amount))
        throw InsufficientFundsException("Insufficient funds in " + iban);
    balance -= amount;
    history.add(Transaction(TransactionType::WITHDRAWAL, amount, balance, desc, iban));
}

void Account::creditTransfer(double amount, const std::string& fromIBAN) {
    if (amount <= 0) throw std::invalid_argument("Amount must be positive");
    if (!active)    throw std::runtime_error("Cannot operate on closed account");
    balance += amount;
    history.add(Transaction(TransactionType::TRANSFER_CREDIT, amount, balance,
                            "Transfer from " + fromIBAN, fromIBAN, iban));
}

void Account::debitTransfer(double amount, const std::string& toIBAN) {
    if (amount <= 0) throw std::invalid_argument("Amount must be positive");
    if (!active)    throw std::runtime_error("Cannot operate on closed account");
    if (!canWithdraw(amount))
        throw InsufficientFundsException("Insufficient funds for transfer from " + iban);
    balance -= amount;
    history.add(Transaction(TransactionType::TRANSFER_DEBIT, amount, balance,
                            "Transfer to " + toIBAN, iban, toIBAN));
}

void Account::printInfo() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  IBAN    : " << iban                    << "\n"
              << "  Type    : " << getAccountType()         << "\n"
              << "  Balance : " << balance << " BGN"        << "\n"
              << "  Status  : " << (active ? "Active" : "Closed") << "\n";
}
