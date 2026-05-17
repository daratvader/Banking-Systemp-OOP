#pragma once
#include "Person.h"
#include <vector>
#include <string>

class Customer : public Person {
private:
    std::string customerId;
    std::string registrationDate;
    std::vector<std::string> accountIBANs;
    std::string username;
    std::string pin;
    bool        active;

public:
    Customer(const std::string& fn,   const std::string& ln,
             const std::string& egn,  const std::string& addr,
             const std::string& phone,
             const std::string& username, const std::string& pin,
             const std::string& customerId);

    bool login(const std::string& user, const std::string& p) const;

    std::string getCustomerId()        const { return customerId; }
    std::string getUsername()          const { return username; }
    std::string getRegistrationDate()  const { return registrationDate; }
    bool        isActive()             const { return active; }

    void deactivate() { active = false; }
    void activate()   { active = true;  }

    const std::vector<std::string>& getAccountIBANs() const { return accountIBANs; }
    void addAccountIBAN(const std::string& iban)    { accountIBANs.push_back(iban); }
    void removeAccountIBAN(const std::string& iban);

    std::string getFullName() const override;
    void printInfo() const override;
};
