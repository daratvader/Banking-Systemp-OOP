# Banking System — OOP Project (C++)

A console banking system demonstrating core OOP principles: inheritance,
polymorphism, abstraction, and encapsulation.

## Build & Run

```bash
make          # compile
make run      # run interactive menu
make demo     # run automated demonstration of all features
make clean    # remove build artifacts
```

Requires `g++` with C++17 support.

## Features

| # | Feature | Status |
|---|---------|--------|
| 1 | Customer management (CRUD) | working |
| 2 | Account management (CRUD)  | working |
| 3 | Deposit & withdrawal       | working |
| 4 | Transfers between accounts | working |
| 5 | Transaction history        | working |
| 6 | Balance report             | working |
| 7 | Interest accrual (savings) | working |
| 8 | Authentication & roles     | working |
| 9 | Statement export to file   | working |

## Project Structure

```
include/   — class declarations (.h)
src/       — class implementations (.cpp)
main.cpp   — interactive menu + demo
Makefile   — build configuration
```

## Class Hierarchy

- `Person` (abstract) → `Customer`, `Employee`
- `Account` (abstract) → `SavingsAccount`, `CheckingAccount`
- `Transaction`, `TransactionHistory`
- `Bank` (Singleton)
- `AuthService`

## Design Note (change from Checkpoint 1)

`CheckingAccount` does not override `withdraw()` directly. Instead the
**Template Method** pattern is used: `Account::withdraw()` calls a protected
virtual `canWithdraw()` hook that `CheckingAccount` overrides to permit
overdraft. This keeps `balance` strictly private in `Account` (as justified in
Checkpoint 1) while still enabling overdraft behaviour.
