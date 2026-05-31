#include "AuthService.h"
#include "Bank.h"
#include "Exceptions.h"
#include <iostream>

AuthService::AuthService(Bank* bank) : bank(bank) {}

bool AuthService::loginAsCustomer(const std::string& username, const std::string& pin) {
    auto* c = bank->authenticateCustomer(username, pin);
    if (!c) {
        std::cout << "  Login failed: invalid customer credentials.\n";
        return false;
    }
    currentSession = { UserRole::CUSTOMER, c->getCustomerId(), true };
    std::cout << "  Logged in as customer: " << c->getFullName() << "\n";
    return true;
}

bool AuthService::loginAsEmployee(const std::string& username, const std::string& password) {
    auto* e = bank->authenticateEmployee(username, password);
    if (!e) {
        std::cout << "  Login failed: invalid employee credentials.\n";
        return false;
    }
    currentSession = { UserRole::EMPLOYEE, e->getEmployeeId(), true };
    std::cout << "  Logged in as employee: " << e->getFullName() << "\n";
    return true;
}

void AuthService::logout() {
    currentSession = {};
    std::cout << "  Logged out.\n";
}

bool AuthService::canAccessAccount(const std::string& iban) const {
    if (!currentSession.active)            return false;
    if (currentSession.role == UserRole::EMPLOYEE) return true;  // full access
    // Customer: only own accounts
    return bank->customerOwnsAccount(currentSession.userId, iban);
}

void AuthService::requireEmployee() const {
    if (currentSession.role != UserRole::EMPLOYEE)
        throw UnauthorizedAccessException("This action requires employee privileges");
}
