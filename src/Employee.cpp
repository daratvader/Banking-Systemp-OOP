#include "Employee.h"
#include <iostream>

Employee::Employee(const std::string& fn,   const std::string& ln,
                   const std::string& egn,  const std::string& addr,
                   const std::string& phone, const std::string& role,
                   const std::string& username, const std::string& password,
                   const std::string& employeeId)
    : Person(fn, ln, egn, addr, phone),
      employeeId(employeeId), role(role),
      username(username), password(password) {}

bool Employee::adminLogin(const std::string& user, const std::string& pass) const {
    return (user == username) && (pass == password);
}

std::string Employee::getFullName() const {
    return firstName + " " + lastName + " [" + role + "]";
}

void Employee::printInfo() const {
    std::cout << "  Employee ID : " << employeeId  << "\n"
              << "  Role        : " << role         << "\n"
              << "  Username    : " << username     << "\n";
    Person::printInfo();
}
