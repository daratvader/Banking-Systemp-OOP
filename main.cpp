#include "Bank.h"
#include "AuthService.h"
#include <iostream>
#include <string>
#include <limits>

// ── Helpers ────────────────────────────────────────────────────────────────
static void pause() {
    std::cout << "\n  Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

static std::string prompt(const std::string& label) {
    std::string val;
    std::cout << "  " << label << ": ";
    std::getline(std::cin, val);
    return val;
}

static double promptDouble(const std::string& label) {
    double val = 0.0;
    std::cout << "  " << label << ": ";
    std::cin >> val;
    std::cin.ignore();
    return val;
}

// Reads a menu choice; recovers gracefully from non-numeric input
static int promptChoice() {
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore();
    return choice;
}

static void header(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n"
              << "  " << title << "\n"
              << std::string(60, '=') << "\n";
}

// ── Menus ──────────────────────────────────────────────────────────────────
static void menuCustomers(Bank* bank) {
    bool back = false;
    while (!back) {
        header("CUSTOMER MANAGEMENT  [Feature 1]");
        std::cout << "  1. Register new customer\n"
                  << "  2. View customer info\n"
                  << "  3. Update customer (address / phone)\n"
                  << "  4. Deactivate customer\n"
                  << "  5. List all customers\n"
                  << "  0. Back\n> ";
        int ch; std::cin >> ch; std::cin.ignore();
        try {
            if (ch == 1) {
                auto fn    = prompt("First name");
                auto ln    = prompt("Last name");
                auto egn   = prompt("EGN");
                auto addr  = prompt("Address");
                auto phone = prompt("Phone");
                auto user  = prompt("Username");
                auto pin   = prompt("PIN");
                bank->registerCustomer(fn, ln, egn, addr, phone, user, pin);
            } else if (ch == 2) {
                auto id = prompt("Customer ID");
                bank->getCustomer(id)->printInfo();
            } else if (ch == 3) {
                auto id    = prompt("Customer ID");
                auto addr  = prompt("New address (leave blank to skip)");
                auto phone = prompt("New phone  (leave blank to skip)");
                bank->updateCustomer(id, addr, phone);
            } else if (ch == 4) {
                auto id = prompt("Customer ID to deactivate");
                bank->deactivateCustomer(id);
            } else if (ch == 5) {
                bank->listAllCustomers();
            } else if (ch == 0) {
                back = true;
            }
        } catch (const std::exception& e) {
            std::cout << "  ERROR: " << e.what() << "\n";
        }
        if (!back) pause();
    }
}

static void menuAccounts(Bank* bank) {
    bool back = false;
    while (!back) {
        header("ACCOUNT MANAGEMENT  [Feature 2]");
        std::cout << "  1. Open savings account\n"
                  << "  2. Open checking account\n"
                  << "  3. View account details\n"
                  << "  4. Close account\n"
                  << "  5. List all accounts for customer\n"
                  << "  0. Back\n> ";
        int ch; std::cin >> ch; std::cin.ignore();
        try {
            if (ch == 1) {
                auto   id      = prompt("Customer ID");
                double bal     = promptDouble("Initial balance (BGN)");
                double rate    = promptDouble("Annual interest rate (e.g. 0.03 for 3%)");
                bank->createSavingsAccount(id, bal, rate);
            } else if (ch == 2) {
                auto   id       = prompt("Customer ID");
                double bal      = promptDouble("Initial balance (BGN)");
                double overdraft= promptDouble("Overdraft limit (BGN, 0 for none)");
                bank->createCheckingAccount(id, bal, overdraft);
            } else if (ch == 3) {
                auto iban = prompt("IBAN");
                bank->getAccount(iban)->printInfo();
            } else if (ch == 4) {
                auto iban = prompt("IBAN to close");
                bank->closeAccount(iban);
            } else if (ch == 5) {
                auto id = prompt("Customer ID");
                bank->listCustomerAccounts(id);
            } else if (ch == 0) {
                back = true;
            }
        } catch (const std::exception& e) {
            std::cout << "  ERROR: " << e.what() << "\n";
        }
        if (!back) pause();
    }
}

static void menuTransactions(Bank* bank) {
    bool back = false;
    while (!back) {
        header("TRANSACTIONS  [Features 3 & 4]");
        std::cout << "  1. Deposit\n"
                  << "  2. Withdraw\n"
                  << "  3. Transfer between accounts\n"
                  << "  0. Back\n> ";
        int ch; std::cin >> ch; std::cin.ignore();
        try {
            if (ch == 1) {
                auto iban   = prompt("IBAN");
                double amt  = promptDouble("Amount (BGN)");
                bank->deposit(iban, amt);
            } else if (ch == 2) {
                auto iban   = prompt("IBAN");
                double amt  = promptDouble("Amount (BGN)");
                bank->withdraw(iban, amt);
            } else if (ch == 3) {
                auto from   = prompt("From IBAN");
                auto to     = prompt("To IBAN");
                double amt  = promptDouble("Amount (BGN)");
                bank->transfer(from, to, amt);
            } else if (ch == 0) {
                back = true;
            }
        } catch (const std::exception& e) {
            std::cout << "  ERROR: " << e.what() << "\n";
        }
        if (!back) pause();
    }
}

static void menuReports(Bank* bank) {
    bool back = false;
    while (!back) {
        header("REPORTS  [Features 5 & 6]");
        std::cout << "  1. Transaction history for account\n"
                  << "  2. Balance report for customer\n"
                  << "  3. Full account statement\n"
                  << "  4. Apply monthly rules (interest & fees)  [Feature 7]\n"
                  << "  5. Export statement to file               [Feature 9]\n"
                  << "  0. Back\n> ";
        int ch; std::cin >> ch; std::cin.ignore();
        try {
            if (ch == 1) {
                auto iban = prompt("IBAN");
                bank->printTransactionHistory(iban);
            } else if (ch == 2) {
                auto id = prompt("Customer ID");
                bank->printBalanceReport(id);
            } else if (ch == 3) {
                auto iban = prompt("IBAN");
                bank->generateStatement(iban);
            } else if (ch == 4) {
                bank->applyMonthlyRules();
            } else if (ch == 5) {
                auto iban = prompt("IBAN");
                bank->exportStatementToFile(iban);
            } else if (ch == 0) {
                back = true;
            }
        } catch (const std::exception& e) {
            std::cout << "  ERROR: " << e.what() << "\n";
        }
        if (!back) pause();
    }
}


// ── Login screen using AuthService [Feature 8] ─────────────────────────────
static void menuLogin(Bank* bank, AuthService& auth) {
    header("LOGIN  [Feature 8]");
    std::cout << "  1. Login as customer\n"
              << "  2. Login as employee\n"
              << "  3. Continue without login\n> ";
    int ch; std::cin >> ch; std::cin.ignore();
    if (ch == 1) {
        auto u = prompt("Username");
        auto p = prompt("PIN");
        auth.loginAsCustomer(u, p);
    } else if (ch == 2) {
        auto u = prompt("Username");
        auto p = prompt("Password");
        auth.loginAsEmployee(u, p);
    }
    (void)bank;
}

// ── Main ───────────────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    Bank* bank = Bank::getInstance();

    // Seed a default employee so employee login works out of the box
    bank->registerEmployee("Admin", "User", "0000000000", "HQ", "0000",
                           "Manager", "admin", "admin");
    AuthService auth(bank);

    menuLogin(bank, auth);

    bool running = true;
    while (running) {
        header("BANKING SYSTEM  —  Main Menu");
        std::cout << "  1. Customer Management      [Feature 1]\n"
                  << "  2. Account Management       [Feature 2]\n"
                  << "  3. Transactions             [Features 3 & 4]\n"
                  << "  4. Reports & Statements     [Features 5, 6 & 7]\n"
                  << "  0. Exit\n> ";
        int choice = promptChoice();

        switch (choice) {
            case 1: menuCustomers(bank);    break;
            case 2: menuAccounts(bank);     break;
            case 3: menuTransactions(bank); break;
            case 4: menuReports(bank);      break;
            case 0: running = false;        break;
            default: std::cout << "  Unknown option.\n"; break;
        }
    }
    std::cout << "\n  Goodbye.\n";
    return 0;
}
