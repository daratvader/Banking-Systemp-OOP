#include "Person.h"
#include <iostream>

Person::Person(const std::string& fn, const std::string& ln,
               const std::string& egn, const std::string& addr,
               const std::string& phone)
    : firstName(fn), lastName(ln), egn(egn), address(addr), phone(phone) {}

std::string Person::getFullName() const {
    return firstName + " " + lastName;
}

void Person::printInfo() const {
    std::cout << "  Name    : " << getFullName()  << "\n"
              << "  EGN     : " << egn            << "\n"
              << "  Address : " << address         << "\n"
              << "  Phone   : " << phone           << "\n";
}
