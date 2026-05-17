#include "Customer.h"
#include "Transaction.h"   // for currentTimestamp
#include <iostream>
#include <algorithm>

Customer::Customer(const std::string& fn,   const std::string& ln,
                   const std::string& egn,  const std::string& addr,
                   const std::string& phone,
                   const std::string& username, const std::string& pin,
                   const std::string& customerId)
    : Person(fn, ln, egn, addr, phone),
      customerId(customerId),
      registrationDate(Transaction::currentTimestamp()),
      username(username), pin(pin), active(true) {}

bool Customer::login(const std::string& user, const std::string& p) const {
    return active && (user == username) && (p == pin);
}

void Customer::removeAccountIBAN(const std::string& iban) {
    accountIBANs.erase(
        std::remove(accountIBANs.begin(), accountIBANs.end(), iban),
        accountIBANs.end());
}

std::string Customer::getFullName() const {
    return firstName + " " + lastName;
}

void Customer::printInfo() const {
    std::cout << "  Customer ID : " << customerId        << "\n"
              << "  Username    : " << username           << "\n"
              << "  Registered  : " << registrationDate  << "\n"
              << "  Status      : " << (active ? "Active" : "Inactive") << "\n";
    Person::printInfo();
    std::cout << "  Accounts    : " << accountIBANs.size() << "\n";
}
