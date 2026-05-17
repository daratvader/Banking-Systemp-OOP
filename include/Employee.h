#pragma once
#include "Person.h"
#include <string>

class Employee : public Person {
private:
    std::string employeeId;
    std::string role;
    std::string username;
    std::string password;

public:
    Employee(const std::string& fn,   const std::string& ln,
             const std::string& egn,  const std::string& addr,
             const std::string& phone,
             const std::string& role,
             const std::string& username, const std::string& password,
             const std::string& employeeId);

    bool adminLogin(const std::string& user, const std::string& pass) const;

    std::string getEmployeeId() const { return employeeId; }
    std::string getRole()       const { return role; }
    std::string getUsername()   const { return username; }

    std::string getFullName() const override;
    void printInfo() const override;
};
