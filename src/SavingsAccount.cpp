#include "SavingsAccount.h"
#include <iostream>
#include <iomanip>
#include <sstream>

SavingsAccount::SavingsAccount(const std::string& iban,
                               const std::string& ownerCustomerId,
                               double initialBalance,
                               double interestRate)
    : Account(iban, ownerCustomerId, initialBalance),
      interestRate(interestRate) {}

void SavingsAccount::applyMonthlyRules() {
    double monthly  = interestRate / 12.0;
    double interest = getBalance() * monthly;
    if (interest > 0.001) {
        std::ostringstream desc;
        desc << std::fixed << std::setprecision(2)
             << "Monthly interest (" << interestRate * 100.0 << "% p.a.)";
        addToBalance(interest, TransactionType::INTEREST, desc.str());
    }
}

void SavingsAccount::printInfo() const {
    Account::printInfo();
    std::cout << std::fixed << std::setprecision(2)
              << "  Rate    : " << interestRate * 100.0 << "% p.a.\n"
              << "  Overdraft: NOT allowed\n";
}
