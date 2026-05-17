#include "Bank.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

// ── Static members ─────────────────────────────────────────────────────────
Bank* Bank::instance          = nullptr;
int   Bank::ibanCounter       = 1;
int   Bank::customerIdCounter = 1;
int   Bank::employeeIdCounter = 1;

// ── Singleton ──────────────────────────────────────────────────────────────
Bank* Bank::getInstance() {
    if (!instance) instance = new Bank();
    return instance;
}

Bank::~Bank() {
    for (auto& [id, c] : customers) delete c;
    for (auto& [id, a] : accounts)  delete a;
    for (auto& [id, e] : employees) delete e;
}

// ── ID / IBAN generators ───────────────────────────────────────────────────
std::string Bank::generateIBAN() {
    std::ostringstream oss;
    oss << "BG00BANK" << std::setw(10) << std::setfill('0') << ibanCounter++;
    return oss.str();
}
std::string Bank::generateCustomerId() {
    std::ostringstream oss;
    oss << "CUST" << std::setw(6) << std::setfill('0') << customerIdCounter++;
    return oss.str();
}
std::string Bank::generateEmployeeId() {
    std::ostringstream oss;
    oss << "EMP" << std::setw(4) << std::setfill('0') << employeeIdCounter++;
    return oss.str();
}

// ── Feature 1: Customer CRUD ───────────────────────────────────────────────
Customer* Bank::registerCustomer(const std::string& fn,   const std::string& ln,
                                 const std::string& egn,  const std::string& addr,
                                 const std::string& phone,
                                 const std::string& username, const std::string& pin) {
    for (auto& [id, c] : customers)
        if (c->getUsername() == username)
            throw std::runtime_error("Username '" + username + "' already taken");

    std::string id = generateCustomerId();
    auto* c = new Customer(fn, ln, egn, addr, phone, username, pin, id);
    customers[id] = c;
    std::cout << "  Customer registered. ID: " << id << "\n";
    return c;
}

Customer* Bank::getCustomer(const std::string& customerId) {
    auto it = customers.find(customerId);
    if (it == customers.end())
        throw CustomerNotFoundException("Customer not found: " + customerId);
    return it->second;
}

Customer* Bank::getCustomerByUsername(const std::string& username) {
    for (auto& [id, c] : customers)
        if (c->getUsername() == username) return c;
    throw CustomerNotFoundException("No customer with username: " + username);
}

void Bank::updateCustomer(const std::string& customerId,
                          const std::string& addr, const std::string& phone) {
    auto* c = getCustomer(customerId);
    if (!addr.empty())  c->setAddress(addr);
    if (!phone.empty()) c->setPhone(phone);
    std::cout << "  Customer " << customerId << " updated.\n";
}

void Bank::deactivateCustomer(const std::string& customerId) {
    getCustomer(customerId)->deactivate();
    std::cout << "  Customer " << customerId << " deactivated.\n";
}

void Bank::listAllCustomers() const {
    if (customers.empty()) { std::cout << "  No customers registered.\n"; return; }
    std::cout << "\n  " << std::string(55, '-') << "\n";
    std::cout << "  " << std::left << std::setw(12) << "ID"
              << std::setw(25) << "Name"
              << std::setw(10) << "Status"
              << "Accounts\n";
    std::cout << "  " << std::string(55, '-') << "\n";
    for (auto& [id, c] : customers) {
        std::cout << "  " << std::setw(12) << id
                  << std::setw(25) << c->getFullName()
                  << std::setw(10) << (c->isActive() ? "Active" : "Inactive")
                  << c->getAccountIBANs().size() << "\n";
    }
    std::cout << "  " << std::string(55, '-') << "\n";
}

// ── Feature 2: Account CRUD ───────────────────────────────────────────────
Account* Bank::createSavingsAccount(const std::string& customerId,
                                    double initialBalance, double interestRate) {
    auto* customer = getCustomer(customerId);
    std::string iban = generateIBAN();
    auto* acc = new SavingsAccount(iban, customerId, initialBalance, interestRate);
    accounts[iban] = acc;
    customer->addAccountIBAN(iban);
    std::cout << "  Savings account created. IBAN: " << iban << "\n";
    return acc;
}

Account* Bank::createCheckingAccount(const std::string& customerId,
                                     double initialBalance, double overdraftLimit) {
    auto* customer = getCustomer(customerId);
    std::string iban = generateIBAN();
    auto* acc = new CheckingAccount(iban, customerId, initialBalance, overdraftLimit);
    accounts[iban] = acc;
    customer->addAccountIBAN(iban);
    std::cout << "  Checking account created. IBAN: " << iban << "\n";
    return acc;
}

Account* Bank::getAccount(const std::string& iban) {
    auto it = accounts.find(iban);
    if (it == accounts.end())
        throw AccountNotFoundException("Account not found: " + iban);
    return it->second;
}

void Bank::closeAccount(const std::string& iban) {
    auto* acc = getAccount(iban);
    acc->close();
    auto* cust = getCustomer(acc->getOwnerCustomerId());
    cust->removeAccountIBAN(iban);
    std::cout << "  Account " << iban << " closed.\n";
}

void Bank::listCustomerAccounts(const std::string& customerId) const {
    auto it = customers.find(customerId);
    if (it == customers.end())
        throw CustomerNotFoundException("Customer not found: " + customerId);
    const auto& ibans = it->second->getAccountIBANs();
    if (ibans.empty()) { std::cout << "  No accounts.\n"; return; }
    for (const auto& iban : ibans) {
        auto ait = accounts.find(iban);
        if (ait != accounts.end()) ait->second->printInfo();
        std::cout << "  " << std::string(40, '-') << "\n";
    }
}

// ── Features 3 & 4: Transactions ──────────────────────────────────────────
void Bank::deposit(const std::string& iban, double amount) {
    getAccount(iban)->deposit(amount);
    std::cout << std::fixed << std::setprecision(2)
              << "  Deposited " << amount << " BGN to " << iban << "\n";
}

void Bank::withdraw(const std::string& iban, double amount) {
    getAccount(iban)->withdraw(amount);
    std::cout << std::fixed << std::setprecision(2)
              << "  Withdrew " << amount << " BGN from " << iban << "\n";
}

void Bank::transfer(const std::string& fromIban,
                    const std::string& toIban, double amount) {
    // Atomic: both operations or neither
    auto* from = getAccount(fromIban);
    auto* to   = getAccount(toIban);
    from->debitTransfer(amount, toIban);   // throws if insufficient
    to->creditTransfer(amount, fromIban);
    std::cout << std::fixed << std::setprecision(2)
              << "  Transferred " << amount << " BGN: "
              << fromIban << " -> " << toIban << "\n";
}

// ── Feature 5: Transaction History ────────────────────────────────────────
void Bank::printTransactionHistory(const std::string& iban) const {
    auto it = accounts.find(iban);
    if (it == accounts.end())
        throw AccountNotFoundException("Account not found: " + iban);
    const auto& hist = it->second->getHistory().getAll();
    if (hist.empty()) { std::cout << "  No transactions yet.\n"; return; }
    std::cout << "\n  Transaction history for " << iban << ":\n"
              << "  " << std::string(65, '-') << "\n";
    for (const auto& t : hist)
        std::cout << "  " << t.getDetails() << "\n";
    std::cout << "  " << std::string(65, '-') << "\n";
}

// ── Feature 6: Balance Report ──────────────────────────────────────────────
void Bank::printBalanceReport(const std::string& customerId) const {
    auto it = customers.find(customerId);
    if (it == customers.end())
        throw CustomerNotFoundException("Customer not found: " + customerId);
    auto* cust = it->second;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n  Balance report for: " << cust->getFullName() << "\n"
              << "  " << std::string(55, '-') << "\n"
              << "  " << std::left << std::setw(22) << "IBAN"
              << std::setw(12) << "Type"
              << "Balance\n"
              << "  " << std::string(55, '-') << "\n";
    double total = 0.0;
    for (const auto& iban : cust->getAccountIBANs()) {
        auto ait = accounts.find(iban);
        if (ait != accounts.end()) {
            auto* acc = ait->second;
            std::cout << "  " << std::setw(22) << iban
                      << std::setw(12) << acc->getAccountType()
                      << acc->getBalance() << " BGN\n";
            total += acc->getBalance();
        }
    }
    std::cout << "  " << std::string(55, '-') << "\n"
              << "  " << std::setw(34) << "NET TOTAL:" << total << " BGN\n"
              << "  " << std::string(55, '-') << "\n";
}

void Bank::generateStatement(const std::string& iban,
                             const std::string& from,
                             const std::string& to) const {
    auto it = accounts.find(iban);
    if (it == accounts.end())
        throw AccountNotFoundException("Account not found: " + iban);
    const auto& hist = it->second->getHistory();
    double opening = 0.0;
    if (!hist.getAll().empty()) opening = hist.getAll().front().getBalanceAfter()
                                        - hist.getAll().front().getAmount();
    hist.printStatement(iban, opening, from, to);
}

// ── Feature 7: Monthly rules ───────────────────────────────────────────────
void Bank::applyMonthlyRules() {
    for (auto& [iban, acc] : accounts)
        if (acc->isActive()) acc->applyMonthlyRules();
    std::cout << "  Monthly rules applied to all accounts.\n";
}

// ── Auth ───────────────────────────────────────────────────────────────────
Customer* Bank::authenticateCustomer(const std::string& username, const std::string& pin) {
    for (auto& [id, c] : customers)
        if (c->login(username, pin)) return c;
    return nullptr;
}

Employee* Bank::registerEmployee(const std::string& fn,   const std::string& ln,
                                 const std::string& egn,  const std::string& addr,
                                 const std::string& phone, const std::string& role,
                                 const std::string& username, const std::string& password) {
    std::string id = generateEmployeeId();
    auto* e = new Employee(fn, ln, egn, addr, phone, role, username, password, id);
    employees[id] = e;
    return e;
}
