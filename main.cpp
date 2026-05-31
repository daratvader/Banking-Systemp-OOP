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

// ── Quick demo: seeds data and exercises all 5+ features automatically ─────
static void runDemo(Bank* bank) {
    header("AUTOMATED DEMO — all features");

    std::cout << "\n[1] Registering customers...\n";
    auto* c1 = bank->registerCustomer("Ivan",  "Petrov",  "9001011234",
                                      "Sofia, ul. Vitosha 1", "0888111111",
                                      "ivan.petrov", "1234");
    auto* c2 = bank->registerCustomer("Maria", "Georgieva","9205152345",
                                      "Plovdiv, ul. Maritsa 5","0888222222",
                                      "maria.g", "5678");
    bank->listAllCustomers();

    std::cout << "\n[2] Opening accounts...\n";
    bank->createSavingsAccount (c1->getCustomerId(), 1000.0, 0.04);
    bank->createCheckingAccount(c1->getCustomerId(),  500.0, 200.0);
    bank->createSavingsAccount (c2->getCustomerId(), 2500.0, 0.025);
    bank->listCustomerAccounts(c1->getCustomerId());

    const auto& ibanList1 = c1->getAccountIBANs();
    std::string savIban  = ibanList1[0];
    std::string chkIban  = ibanList1[1];
    std::string c2Iban   = c2->getAccountIBANs()[0];

    std::cout << "\n[3] Deposit & Withdrawal...\n";
    bank->deposit (savIban, 300.0);
    bank->withdraw(savIban, 150.0);
    bank->deposit (chkIban, 50.0);
    // CheckingAccount overdraft: withdraw more than balance (500+50=550, limit 200)
    bank->withdraw(chkIban, 600.0);

    std::cout << "\n[4] Transfer...\n";
    bank->transfer(savIban, c2Iban, 200.0);

    std::cout << "\n[5] Transaction history...\n";
    bank->printTransactionHistory(savIban);

    std::cout << "\n[6] Balance report...\n";
    bank->printBalanceReport(c1->getCustomerId());

    std::cout << "\n[7] Apply monthly interest/fees...\n";
    bank->applyMonthlyRules();
    bank->printBalanceReport(c1->getCustomerId());

    std::cout << "\n[+] Full statement...\n";
    bank->generateStatement(savIban);

    // ── Feature 8: Authentication & roles ──────────────────────────────────
    std::cout << "\n[8] Authentication & roles...\n";
    bank->registerEmployee("Petar", "Dimitrov", "8503031234",
                           "Sofia, HQ", "0888999999",
                           "Teller", "admin", "adminpass");
    AuthService auth(bank);
    auth.loginAsCustomer("ivan.petrov", "1234");
    std::cout << "  Customer can access own savings account? "
              << (auth.canAccessAccount(savIban) ? "YES" : "NO") << "\n";
    std::cout << "  Customer can access another customer's account? "
              << (auth.canAccessAccount(c2Iban) ? "YES" : "NO") << "\n";
    auth.logout();
    auth.loginAsEmployee("admin", "adminpass");
    std::cout << "  Employee can access any account? "
              << (auth.canAccessAccount(c2Iban) ? "YES" : "NO") << "\n";

    // ── Feature 9: Statement export to file ─────────────────────────────────
    std::cout << "\n[9] Export statement to file...\n";
    bank->exportStatementToFile(savIban);

    std::cout << "\n  Demo complete.\n";
}

// ── Main ───────────────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    Bank* bank = Bank::getInstance();

    // Launch demo if requested
    if (argc > 1 && std::string(argv[1]) == "--demo") {
        runDemo(bank);
        return 0;
    }

    bool running = true;
    while (running) {
        header("BANKING SYSTEM  —  Main Menu");
        std::cout << "  1. Customer Management      [Feature 1]\n"
                  << "  2. Account Management       [Feature 2]\n"
                  << "  3. Transactions             [Features 3 & 4]\n"
                  << "  4. Reports & Statements     [Features 5, 6 & 7]\n"
                  << "  5. Run automated demo\n"
                  << "  0. Exit\n> ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: menuCustomers(bank);    break;
            case 2: menuAccounts(bank);     break;
            case 3: menuTransactions(bank); break;
            case 4: menuReports(bank);      break;
            case 5: runDemo(bank); pause(); break;
            case 0: running = false;        break;
            default: std::cout << "  Unknown option.\n"; break;
        }
    }
    std::cout << "\n  Goodbye.\n";
    return 0;
}
