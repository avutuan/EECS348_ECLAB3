#include <iostream>
#include <iomanip>
#include <string>

class Account {

public:
    std::string accountNumber;
    std::string accountHolder;
    double balance;
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    void displayDetails() const {
        std::cout << "Account Details for " << getAccountType() << " (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        displayAdditionalDetails();
        std::cout << std::endl;
    }

    void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
        } else {
            std::cout << "Insufficient balance." << std::endl;
        }
    }

    virtual std::string getAccountType() const {
        return "Account";
    }

    virtual void displayAdditionalDetails() const {}

    friend std::ostream& operator<<(std::ostream& os, const Account& account);

    friend Account operator+(const Account& account1, const Account& account2);
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void withdraw(double amount) override {
        if (balance - amount >= 0) {
            balance -= amount;
        } else {
            std::cout << "Insufficient balance. Minimum balance required: $0.00" << std::endl;
        }
    }

    std::string getAccountType() const override {
        return "Savings Account";
    }

    void displayAdditionalDetails() const override {
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << (interestRate * 100) << "%" << std::endl;
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void withdraw(double amount) override {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
        } else {
            std::cout << "Insufficient balance. Overdraft limit: $" << overdraftLimit << std::endl;
        }
    }

    std::string getAccountType() const override {
        return "Current Account";
    }

    void displayAdditionalDetails() const override {
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }
};

Account operator+(Account& account1, Account& account2) {
    int transfer = 300;
    account1.withdraw(transfer);
    account2.deposit(transfer);
    return account1;
}

std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << "Account Details for " << account.getAccountType() << " (ID: " << account.accountNumber << "):" << std::endl;
    os << "   Holder: " << account.accountHolder << std::endl;
    os << "   Balance: $" << std::fixed << std::setprecision(2) << account.balance << std::endl;
    account.displayAdditionalDetails();
    os << std::endl;
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    savings.displayDetails();
    current.displayDetails();

    savings.deposit(500);
    current.withdraw(1000);

    savings.displayDetails();
    current.displayDetails();

    // Transfer 300 from savings to current
    Account result = savings + current;

    savings.displayDetails();
    current.displayDetails();

    return 0;
}
