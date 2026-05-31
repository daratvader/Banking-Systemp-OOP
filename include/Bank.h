#pragma once
#include "Customer.h"
#include "Employee.h"
#include "Account.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "Exceptions.h"
#include <map>
#include <string>

class Bank {
private:
    static Bank* instance;
    static int   ibanCounter;
    static int   customerIdCounter;
    static int   employeeIdCounter;

    std::map<std::string, Customer*> customers;   // customerId  -> Customer*
    std::map<std::string, Account*>  accounts;    // iban        -> Account*
    std::map<std::string, Employee*> employees;   // employeeId  -> Employee*

    Bank()  = default;
    ~Bank();
    Bank(const Bank&)            = delete;
    Bank& operator=(const Bank&) = delete;

public:
    static Bank* getInstance();

    // ── Customer CRUD (Feature 1) ─────────────────────────────────────────
    Customer* registerCustomer(const std::string& fn,   const std::string& ln,
                               const std::string& egn,  const std::string& addr,
                               const std::string& phone,
                               const std::string& username, const std::string& pin);

    Customer* getCustomer(const std::string& customerId);
    Customer* getCustomerByUsername(const std::string& username);
    void      updateCustomer(const std::string& customerId,
                             const std::string& addr, const std::string& phone);
    void      deactivateCustomer(const std::string& customerId);
    void      listAllCustomers() const;

    // ── Account CRUD (Feature 2) ──────────────────────────────────────────
    Account* createSavingsAccount(const std::string& customerId,
                                  double initialBalance = 0.0,
                                  double interestRate   = 0.03);

    Account* createCheckingAccount(const std::string& customerId,
                                   double initialBalance = 0.0,
                                   double overdraftLimit = 0.0);

    Account* getAccount(const std::string& iban);
    void     closeAccount(const std::string& iban);
    void     listCustomerAccounts(const std::string& customerId) const;

    // ── Transactions (Features 3 & 4) ─────────────────────────────────────
    void deposit (const std::string& iban, double amount);
    void withdraw(const std::string& iban, double amount);
    void transfer(const std::string& fromIban,
                  const std::string& toIban, double amount);

    // ── Reports (Features 5 & 6) ──────────────────────────────────────────
    void printTransactionHistory(const std::string& iban) const;
    void printBalanceReport(const std::string& customerId) const;
    void generateStatement(const std::string& iban,
                           const std::string& from = "",
                           const std::string& to   = "") const;
    // Writes the statement to a text file; returns the filename written.
    std::string exportStatementToFile(const std::string& iban,
                                      const std::string& from = "",
                                      const std::string& to   = "") const;

    // ── Interest (Feature 7) ──────────────────────────────────────────────
    void applyMonthlyRules();

    // ── Auth helper ───────────────────────────────────────────────────────
    Customer* authenticateCustomer(const std::string& username,
                                   const std::string& pin);
    Employee* authenticateEmployee(const std::string& username,
                                   const std::string& password);
    // Verifies a customer is allowed to touch a given account (owns it)
    bool customerOwnsAccount(const std::string& customerId,
                             const std::string& iban) const;
    Employee* registerEmployee(const std::string& fn,   const std::string& ln,
                               const std::string& egn,  const std::string& addr,
                               const std::string& phone, const std::string& role,
                               const std::string& username, const std::string& password);

    // ── Utilities ─────────────────────────────────────────────────────────
    static std::string generateIBAN();
    static std::string generateCustomerId();
    static std::string generateEmployeeId();
};
