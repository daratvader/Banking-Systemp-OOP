#include "AuthService.h"
#include "Bank.h"
#include <iostream>

AuthService::AuthService(Bank* bank) : bank(bank) {}

bool AuthService::loginAsCustomer(const std::string& username, const std::string& pin) {
    auto* c = bank->authenticateCustomer(username, pin);
    if (!c) {
        std::cout << "  Login failed: invalid credentials.\n";
        return false;
    }
    currentSession = { UserRole::CUSTOMER, c->getCustomerId(), true };
    std::cout << "  Logged in as customer: " << c->getFullName() << "\n";
    return true;
}

bool AuthService::loginAsEmployee(const std::string& username, const std::string& password) {
    // Simplified: employee auth not persisted to Bank in this demo
    (void)username; (void)password;
    currentSession = { UserRole::EMPLOYEE, "EMP0001", true };
    std::cout << "  Logged in as employee.\n";
    return true;
}

void AuthService::logout() {
    currentSession = {};
    std::cout << "  Logged out.\n";
}
