#pragma once
#include <string>

class Bank;
class Customer;
class Employee;

enum class UserRole { NONE, CUSTOMER, EMPLOYEE };

struct Session {
    UserRole    role   = UserRole::NONE;
    std::string userId;          // customerId or employeeId
    bool        active = false;
};

class AuthService {
private:
    Session currentSession;
    Bank*   bank;

public:
    explicit AuthService(Bank* bank);

    bool loginAsCustomer(const std::string& username, const std::string& pin);
    bool loginAsEmployee(const std::string& username, const std::string& password);
    void logout();

    bool isLoggedIn()  const { return currentSession.active; }
    bool isEmployee()  const { return currentSession.role == UserRole::EMPLOYEE; }
    bool isCustomer()  const { return currentSession.role == UserRole::CUSTOMER; }

    const Session& getSession() const { return currentSession; }
};
