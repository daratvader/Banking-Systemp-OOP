#pragma once
#include "Account.h"

class CheckingAccount : public Account {
private:
    double overdraftLimit;   // absolute value; balance may go down to -overdraftLimit

protected:
    // Template-method override: allow balance to go negative up to overdraftLimit
    bool canWithdraw(double amount) const override;

public:
    CheckingAccount(const std::string& iban,
                    const std::string& ownerCustomerId,
                    double initialBalance  = 0.0,
                    double overdraftLimit  = 0.0);

    // Applies monthly maintenance fee when balance is below threshold
    void applyMonthlyRules() override;

    std::string getAccountType()   const override { return "Checking"; }
    double getOverdraftLimit()     const { return overdraftLimit; }
    void   setOverdraftLimit(double l)  { overdraftLimit = l; }

    void printInfo() const override;
};
