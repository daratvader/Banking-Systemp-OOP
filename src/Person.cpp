#include "Person.h"
#include <iostream>

Person::Person(const std::string& fn, const std::string& ln,
               const std::string& egn, const std::string& addr,
               const std::string& phone)
    : firstName(fn), lastName(ln), egn(egn), address(addr), phone(phone) {}

std::string Person::getFullName() const {
    return firstName + " " + lastName;
}

std::string Person::getMaskedEgn() const {
    if (egn.size() <= 4) return egn;
    return std::string(egn.size() - 4, '*') + egn.substr(egn.size() - 4);
}

void Person::printInfo() const {
    std::cout << "  Name    : " << getFullName()  << "\n"
              << "  EGN     : " << getMaskedEgn()  << "\n"
              << "  Address : " << address         << "\n"
              << "  Phone   : " << phone           << "\n";
}
