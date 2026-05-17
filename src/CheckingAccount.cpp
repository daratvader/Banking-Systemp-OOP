#include "CheckingAccount.h"
#include <iostream>
#include <iomanip>

CheckingAccount::CheckingAccount(const std::string& iban,
                                 const std::string& ownerCustomerId,
                                 double initialBalance,
                                 double overdraftLimit)
    : Account(iban, ownerCustomerId, initialBalance),
      overdraftLimit(overdraftLimit) {}

// Template-method override: allow balance to dip into the overdraft zone
bool CheckingAccount::canWithdraw(double amount) const {
    return getBalance() + overdraftLimit >= amount;
}

void CheckingAccount::applyMonthlyRules() {
    // Charge a maintenance fee when balance falls below a threshold
    constexpr double FEE_THRESHOLD = 200.0;
    constexpr double MAINTENANCE   = 2.50;
    if (getBalance() < FEE_THRESHOLD) {
        addToBalance(-MAINTENANCE, TransactionType::FEE, "Monthly maintenance fee");
    }
}

void CheckingAccount::printInfo() const {
    Account::printInfo();
    std::cout << std::fixed << std::setprecision(2)
              << "  Overdraft: " << overdraftLimit << " BGN\n";
}
