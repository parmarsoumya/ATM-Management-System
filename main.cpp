#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <limits>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#ifdef __linux__
#include <sys/utsname.h>
#endif

using namespace std;

void sendToATMDriver(const char* message)
{
    int fd = open("/dev/atm_device", O_WRONLY);

    if (fd < 0)
    {
        perror("Failed to open ATM device");
        return;
    }

    ssize_t result = write(fd, message, strlen(message));

    if (result < 0)
    {
        perror("Failed to write to ATM driver");
    }

    close(fd);
}

// Account Class

class Account {
private:
    string accountNumber;
    string name;
    string pin;
    double balance;

public:

    Account() {
        accountNumber = "";
        name = "";
        pin = "";
        balance = 0.0;
    }

    Account(string accNo, string userName, string userPin, double bal) {
        accountNumber = accNo;
        name = userName;
        pin = userPin;
        balance = bal;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    string getName() const {
        return name;
    }

    string getPin() const {
        return pin;
    }

    double getBalance() const {
        return balance;
    }

    void setPin(string newPin) {
        pin = newPin;
    }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            return false;
        }

        if (amount > balance) {
            return false;
        }

        balance -= amount;
        return true;
    }
};

// ATM Class

class ATM {
private:
    vector<Account> accounts;
    Account* currentAccount;

    const string accountFile = "accounts.txt";
    const string transactionFile = "transactions.txt";

public:

    ATM() {
        currentAccount = nullptr;
        loadAccounts();
    }

    // Load accounts from file

    void loadAccounts() {

        ifstream file(accountFile);

        if (!file.is_open()) {

            // Create sample accounts if file doesn't exist
            accounts.push_back(
                Account("1001", "Soumya", "1234", 10000.0)
            );

            accounts.push_back(
                Account("1002", "Ram", "5678", 7500.0)
            );

            accounts.push_back(
                Account("1003", "Shalu", "1111", 5000.0)
            );

            saveAccounts();
            return;
        }

        string line;

        while (getline(file, line)) {

            if (line.empty()) {
                continue;
            }

            stringstream ss(line);

            string accNo;
            string name;
            string pin;
            string balanceString;

            getline(ss, accNo, '|');
            getline(ss, name, '|');
            getline(ss, pin, '|');
            getline(ss, balanceString, '|');

            try {

                double balance = stod(balanceString);

                accounts.push_back(
                    Account(accNo, name, pin, balance)
                );

            } catch (...) {
                // Ignore invalid records
            }
        }

        file.close();
    }

    // Save accounts to file

    void saveAccounts() {

        ofstream file(accountFile);

        if (!file.is_open()) {
            cout << "Error: Unable to save account data.\n";
            return;
        }

        for (const Account& account : accounts) {

            file << account.getAccountNumber()
                 << "|"
                 << account.getName()
                 << "|"
                 << account.getPin()
                 << "|"
                 << fixed << setprecision(2)
                 << account.getBalance()
                 << "\n";
        }

        file.close();
    }

    // Get current date and time

    string getDateTime() {

        time_t now = time(nullptr);

        tm* localTime = localtime(&now);

        char buffer[80];

        strftime(
            buffer,
            sizeof(buffer),
            "%Y-%m-%d %H:%M:%S",
            localTime
        );

        return string(buffer);
    }

    // Save transaction

    void saveTransaction(
        string type,
        double amount
    ) {

        ofstream file(
            transactionFile,
            ios::app
        );

        if (!file.is_open()) {
            cout << "Warning: Transaction could not be recorded.\n";
            return;
        }

        file << currentAccount->getAccountNumber()
             << "|"
             << getDateTime()
             << "|"
             << type
             << "|"
             << fixed
             << setprecision(2)
             << amount
             << "|"
             << currentAccount->getBalance()
             << "\n";

        file.close();
    }

    // Find account

    Account* findAccount(string accountNumber) {

        for (auto& account : accounts) {

            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }

        return nullptr;
    }

    // Login

    bool login() {

        string accountNumber;
        string pin;

        cout << "\n";
        cout << "========================================\n";
        cout << "             ATM LOGIN\n";
        cout << "========================================\n";

        cout << "Enter Account Number: ";
        cin >> accountNumber;

        cout << "Enter PIN: ";
        cin >> pin;

        Account* account = findAccount(accountNumber);

        if (account == nullptr) {

            cout << "\nInvalid account number.\n";
            return false;
        }

        if (account->getPin() != pin) {

            cout << "\nIncorrect PIN.\n";
            return false;
        }

        currentAccount = account;

        cout << "\nLogin successful!\n";
        cout << "Welcome, "
             << currentAccount->getName()
             << "!\n";

        return true;
    }

    // Check Balance

    void checkBalance() {

        cout << "\n----------------------------------------\n";
        cout << "             BALANCE\n";
        cout << "----------------------------------------\n";

        cout << "Available Balance: Rs. "
             << fixed
             << setprecision(2)
             << currentAccount->getBalance()
             << "\n";
    }

    // Deposit

    void depositMoney() {

        double amount;

        cout << "\nEnter amount to deposit: Rs. ";

        if (!(cin >> amount)) {

            cout << "Invalid input.\n";

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            return;
        }

        if (amount <= 0) {

            cout << "Amount must be greater than zero.\n";
            return;
        }

        currentAccount->deposit(amount);

        saveTransaction("DEPOSIT", amount);

        saveAccounts();

        cout << "\nDeposit successful.\n";
        sendToATMDriver("DEPOSIT");

        cout << "New Balance: Rs. "
             << fixed
             << setprecision(2)
             << currentAccount->getBalance()
             << "\n";
    }

    // Withdraw

    void withdrawMoney() {

        double amount;

        cout << "\nEnter amount to withdraw: Rs. ";

        if (!(cin >> amount)) {

            cout << "Invalid input.\n";

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            return;
        }

        if (amount <= 0) {

            cout << "Amount must be greater than zero.\n";
            return;
        }

        if (amount > currentAccount->getBalance()) {

            cout << "\nInsufficient balance.\n";
            return;
        }

        if (amount > 25000) {

            cout << "\nMaximum withdrawal limit is Rs. 25,000.\n";
            return;
        }

        currentAccount->withdraw(amount);

        saveTransaction("WITHDRAW", amount);

        saveAccounts();

        cout << "\nPlease collect your cash.\n";

        cout << "Remaining Balance: Rs. "
             << fixed
             << setprecision(2)
             << currentAccount->getBalance()
             << "\n";
       sendToATMDriver("WITHDRAW");
    }

    // Change PIN

    void changePin() {

        string oldPin;
        string newPin;
        string confirmPin;

        cout << "\nEnter current PIN: ";
        cin >> oldPin;

        if (oldPin != currentAccount->getPin()) {

            cout << "Incorrect current PIN.\n";
            return;
        }

        cout << "Enter new 4-digit PIN: ";
        cin >> newPin;

        if (newPin.length() != 4) {

            cout << "PIN must contain exactly 4 digits.\n";
            return;
        }

        for (char c : newPin) {

            if (!isdigit(c)) {

                cout << "PIN must contain only digits.\n";
                return;
            }
        }

        cout << "Confirm new PIN: ";
        cin >> confirmPin;

        if (newPin != confirmPin) {

            cout << "PIN confirmation does not match.\n";
            return;
        }

        currentAccount->setPin(newPin);

        saveAccounts();

        cout << "\nPIN changed successfully.\n";
    }

    // Mini Statement

    void miniStatement() {

        ifstream file(transactionFile);

        if (!file.is_open()) {

            cout << "\nNo transactions found.\n";
            return;
        }

        string line;

        cout << "\n";
        cout << "========================================\n";
        cout << "             MINI STATEMENT\n";
        cout << "========================================\n";

        bool found = false;

        while (getline(file, line)) {

            stringstream ss(line);

            string accountNo;
            string dateTime;
            string type;
            string amount;
            string balance;

            getline(ss, accountNo, '|');
            getline(ss, dateTime, '|');
            getline(ss, type, '|');
            getline(ss, amount, '|');
            getline(ss, balance, '|');

            if (
                accountNo ==
                currentAccount->getAccountNumber()
            ) {

                found = true;

                cout << dateTime
                     << " | "
                     << type
                     << " | Rs. "
                     << amount
                     << " | Balance: Rs. "
                     << balance
                     << "\n";
            }
        }

        if (!found) {
            cout << "No transactions available.\n";
        }

        file.close();
    }

    // Account Information

    void accountInformation() {

        cout << "\n";
        cout << "========================================\n";
        cout << "          ACCOUNT INFORMATION\n";
        cout << "========================================\n";

        cout << "Account Number : "
             << currentAccount->getAccountNumber()
             << "\n";

        cout << "Account Holder : "
             << currentAccount->getName()
             << "\n";

        cout << "Balance        : Rs. "
             << fixed
             << setprecision(2)
             << currentAccount->getBalance()
             << "\n";
    }

    // Linux System Information

    void linuxInformation() {

        cout << "\n";
        cout << "========================================\n";
        cout << "          LINUX SYSTEM INFORMATION\n";
        cout << "========================================\n";

#ifdef __linux__

        struct utsname systemInfo;

        if (uname(&systemInfo) == 0) {

            cout << "Operating System : Linux\n";
            cout << "Kernel Name      : "
                 << systemInfo.sysname
                 << "\n";

            cout << "Kernel Release   : "
                 << systemInfo.release
                 << "\n";

            cout << "Machine          : "
                 << systemInfo.machine
                 << "\n";

            cout << "Node Name        : "
                 << systemInfo.nodename
                 << "\n";
        }
        else {

            cout << "Unable to retrieve Linux information.\n";
        }

#else

        cout << "This feature is designed for Linux.\n";

#endif
    }

    // Main ATM Menu

    void menu() {

        int choice;

        do {

            cout << "\n";
            cout << "========================================\n";
            cout << "          ATM MANAGEMENT SYSTEM\n";
            cout << "========================================\n";

            cout << "Welcome, "
                 << currentAccount->getName()
                 << "\n\n";

            cout << "1. Check Balance\n";
            cout << "2. Deposit Money\n";
            cout << "3. Withdraw Money\n";
            cout << "4. Change PIN\n";
            cout << "5. Mini Statement\n";
            cout << "6. Account Information\n";
            cout << "7. Linux System Information\n";
            cout << "8. Logout\n";

            cout << "\nEnter your choice: ";

            if (!(cin >> choice)) {

                cout << "Invalid input.\n";

                cin.clear();

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                continue;
            }

            switch (choice) {

                case 1:
                    checkBalance();
                    break;

                case 2:
                    depositMoney();
                    break;

                case 3:
                    withdrawMoney();
                    break;

                case 4:
                    changePin();
                    break;

                case 5:
                    miniStatement();
                    break;

                case 6:
                    accountInformation();
                    break;

                case 7:
                    linuxInformation();
                    break;

                case 8:
                    cout << "\nLogging out...\n";
                    currentAccount = nullptr;
                    break;

                default:
                    cout << "\nInvalid choice.\n";
            }

        } while (currentAccount != nullptr);
    }

    // Start ATM

    void start() {

        cout << "\n";
        cout << "========================================\n";
        cout << "       WELCOME TO C++ ATM SYSTEM\n";
        cout << "========================================\n";

        bool loggedIn = login();

        if (loggedIn) {
            menu();
        }

        cout << "\nThank you for using the ATM.\n";
    }
};


// Main Function

int main() {

    ATM atm;

    atm.start();

    return 0;
}
