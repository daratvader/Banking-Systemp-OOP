#pragma once
#include <exception>
#include <string>

class BankException : public std::exception {
protected:
    std::string message;
public:
    explicit BankException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class InsufficientFundsException : public BankException {
public:
    explicit InsufficientFundsException(const std::string& msg = "Insufficient funds")
        : BankException(msg) {}
};

class AccountNotFoundException : public BankException {
public:
    explicit AccountNotFoundException(const std::string& msg = "Account not found")
        : BankException(msg) {}
};

class CustomerNotFoundException : public BankException {
public:
    explicit CustomerNotFoundException(const std::string& msg = "Customer not found")
        : BankException(msg) {}
};

class UnauthorizedAccessException : public BankException {
public:
    explicit UnauthorizedAccessException(const std::string& msg = "Unauthorized access")
        : BankException(msg) {}
};

class DuplicateAccountException : public BankException {
public:
    explicit DuplicateAccountException(const std::string& msg = "Account already exists")
        : BankException(msg) {}
};
