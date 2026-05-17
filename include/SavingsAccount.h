#pragma once
#include "Account.h"

class SavingsAccount : public Account {
private:
    double interestRate;             // annual rate, e.g. 0.03 = 3 %
    static constexpr bool overdraftAllowed = false;

public:
    SavingsAccount(const std::string& iban,
                   const std::string& ownerCustomerId,
                   double initialBalance = 0.0,
                   double interestRate   = 0.03);

    // Applies 1/12 of annual interest to the balance
    void applyMonthlyRules() override;

    std::string getAccountType() const override { return "Savings"; }
    double getInterestRate()     const { return interestRate; }
    void   setInterestRate(double r)  { interestRate = r; }

    void printInfo() const override;
};
