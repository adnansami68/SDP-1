#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
#include<limits>
#include<iomanip>
#include<cmath>
#include<cstdlib>
using namespace std;

string ACCOUNT_FILE ="bank_accounts.txt";
string ACCOUNT_COUNTER_FILE="account_counter.txt";
int MIN_PASSWORD_LENGTH=4;
double SAVINGS_MIN_BALANCE=100.00;
double CURRENT_MIN_BALANCE=500.00;

class BankAccount {
private:
    string accountNumber;
    string accountHolderName;
    string address;
    string phoneNumber;
    string email;
    double balance;
    string accountType;
    string password;

public:
    BankAccount(string accNum="",string name="",string addr="",string phone="",
    string mail="",double initialDeposit=0.0,string accType="Savings",string pwd="1234")
    :accountNumber(accNum),accountHolderName(name),address(addr),phoneNumber(phone),
    email(mail),balance(initialDeposit),accountType(accType),password(pwd){

    }

    string getAccountNumber() const { return accountNumber; }
    string getAccountHolderName() const { return accountHolderName; }
    double getBalance() const { return balance; }
    string getAccountType() const { return accountType; }
    string getPassword() const { return password; }

    void deposit(double amount){
        if(amount>0){
            balance+=amount;
            cout<<"Deposit successfull. New balace: "<<balance<<" BDT"<<endl;
        }else{
            cout<<"Invalid deposit amount."<<endl;
        }
    }

    bool verifyPassword( string & pwd) {
        return pwd==password;
    }
    void withdraw(double amount, string & pwd){
        if(!verifyPassword(pwd)){
            cout<<"Invalid password. Withdraw failed."<<endl;
            return;
        }
        if(amount<=0){
            cout<<"Invalid withdrawl amount."<<endl;
            return;
        }
        double minimumBalance=(accountType=="Savings")? SAVINGS_MIN_BALANCE : CURRENT_MIN_BALANCE;
        if((balance-amount)<minimumBalance){
            cout<<"Withdrawl failed. Minimum balance requirement not met."<<endl;
            cout<<"Minimum required balance for "<<accountType<<" account:"<<minimumBalance<<" BDT"<<endl;
            return;
        }
        if(amount<=balance){
            balance-=amount;
            cout<<"Withdrawl successfull. New balace: "<<balance<<" BDT"<<endl;
        }else cout<<"Insufficient balance."<<endl;
    }
    void displayAccountInfo(){
        cout<<"===== Account Information ====="<<endl;
        cout<<"Account Number: "<<accountNumber<<endl;
        cout<<"Account Holder: "<<accountHolderName<<endl;
        cout<<"Address: "<<address<<endl;
        cout<<"Phone: "<<phoneNumber<<endl;
        cout<<"Email: "<<email<<endl;
        cout<<"Account Type: "<<accountType<<endl;
        cout<<"Current Balance: "<<balance<<" BDT"<<endl;
        cout<<"================================"<<endl;
    }
    void saveToFile(ofstream & outFile) {
        outFile<<accountNumber<<endl;
        outFile<<accountHolderName<<endl;
        outFile<<address<<endl;
        outFile<<balance<<endl;
        outFile<<accountType<<endl;
        outFile<<password<<endl;
    }
    void loadFromFile(ifstream & inFile){
        getline(inFile, accountNumber);
        getline(inFile, accountHolderName);
        getline(inFile, address);
        getline(inFile, phoneNumber);
        getline(inFile, email);
        inFile >> balance;
        inFile.ignore();
        getline(inFile, accountType);
        getline(inFile, password);
    }
};

class BankingSystem{
private:
    vector<BankAccount>accounts;
    int accountCounter=1000;
    string generateAccountNumber(){
        accountCounter++;
        return "ACCT" + to_string(accountCounter);
    }
    void loadAccountCounter(){
        ifstream inFile(ACCOUNT_COUNTER_FILE);
            if(inFile){
                inFile>>accountCounter;
                inFile.close();
            }
    }
    void saveAccountCounter(){
        ofstream outFile(ACCOUNT_COUNTER_FILE);
        if(outFile){
            outFile<<accountCounter;
            outFile.close();
        }
    }
    BankAccount* findAccount(string & accNum){
        for(auto & account: accounts){
            if(account.getAccountNumber()==accNum) return &account;
        }
        return nullptr;
    }
    void loadAccounts(){
        ifstream inFile(ACCOUNT_FILE);
        if(inFile){
            while(inFile.peek()!=EOF){
                BankAccount account;
                account.loadFromFile(inFile);
                accounts.push_back(account);
                string accNum=account.getAccountNumber().substr(4);
                int num= stoi(accNum);
                if(num>=accountCounter) accountCounter=num;
            }
            inFile.close();
        }
    }
    void saveAccounts(){
        ofstream outFile(ACCOUNT_FILE);
        if(outFile){
            for(auto & account:accounts) account.saveToFile(outFile);
            outFile.close();
        }else cout<<"Error saving account to file !"<<endl;
    }
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

public:
    BankingSystem(){
        loadAccountCounter();
        loadAccounts();
    }

    ~BankingSystem(){
        saveAccounts();
        saveAccountCounter();
    }
    void createNewAccount(){
        string name;
        string address;
        string phone;
        string email;
        string accountType;
        string password;
        double initialDeposit;

        clearScreen();
        cout<<"===== Create New Account ====="<<endl;
        cin.ignore();
        while(true){
            cout<<"Enter full name: ";
            getline(cin,name);
            if(!name.empty())break;
            cout<<"Name can not be empty. Please try again."<<endl;
        }
        while(true){
            cout<<"Enter address: ";
            getline(cin,address);
            if(!address.empty()) break;
            cout<<"Address cannot be empty. Please try again."<<endl;
        }
        while(true){
            cout<<"Enter phone number: ";
            cin>>phone;
            if(all_of(phone.begin(),phone.end(), ::isdigit)&& phone.size()==11)break;
            cout<<"Phone number must contain only digits. Please try again."<<endl;
        }
        while(true){
            cout<<"Enter email: ";
            cin>>email;
            if(!email.empty())break;
            cout<<"Email cannot be empty. Please try again."<<endl;
        }
        while(true){
            cout<<"Enter account type (Savings/Current): ";
            cin>>accountType;
            if(accountType=="Savings"||accountType=="Current"){
                break;
            }
            cout<<"Invalid account type. Please enter 'Savings' or 'Current'."<<endl;
        }
        double minimumDeposit=(accountType=="Savings")? SAVINGS_MIN_BALANCE : CURRENT_MIN_BALANCE;
        while(true){
            cout<<"Enter initial deposit amount"<<"(minimum "<<minimumDeposit<<" BDT): ";
            if(cin>>initialDeposit){
                if(initialDeposit>=minimumDeposit){
                    cin.ignore();
                    break;
                }
                cout<<"Minimum deposit ammount for "<<accountType<<" account is "<<minimumDeposit<<" BDT."<<endl;
            }else{
                cout<<"Invalid amount."<<endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
            }
        }
        cout<<"Set a "<<MIN_PASSWORD_LENGTH<< "-digit password fot your account: ";
        cin>>password;
        while(password.length()!=MIN_PASSWORD_LENGTH){
            cout<<"Password must be "<<MIN_PASSWORD_LENGTH<<"-digits. Please try again:";
            cin>>password;
        }
        string accNum=generateAccountNumber();
        accounts.emplace_back(accNum,name, address,phone,email,initialDeposit,accountType,password);
        string successMessage="Account created: "+ accNum + " for "+name;
        cout<<"\n\n"<<successMessage<<endl;

        cout<<"====== Account Created Successfully ====="<<endl;
        cout<<"Account Number: "<<accNum<<endl;
        cout<<"Account Holder: "<<name<<endl;
        cout<<"Account Type: "<<accountType<<endl;
        cout<<"Initial Balance: "<<initialDeposit<<endl;
        cout<<"========================================="<<endl;
    }
    void depositMoney(){
        string accNum;
        double amount;
        clearScreen();
        cout<<"===== Deposit Money ====="<<endl;
        cout<<"Enter account number: ";
        cin>>accNum;
        BankAccount * account=findAccount(accNum);
        if(account){
            cout<<"Account holder: "<<account->getAccountHolderName()<<endl;
            cout<<"Current balance: "<<account->getBalance()<<" BDT."<<endl;
            while(true){
                cout<<"Enter deposit amount: ";
                if(cin>>amount){
                    account->deposit(amount);
                    saveAccounts();
                    break;
                }else{
                    cout<<"Invalid amount."<<endl;
                    cin.clear();
                }
            }
        }else cout<<"Account not found."<<endl;
    }
    void withdrawMoney(){
        string accNum,password;
        double amount;
        clearScreen();
        cout<<"===== Withdraw Money ===="<<endl;
        cout<<"Enter account number: ";
        cin>>accNum;

        BankAccount * account=findAccount(accNum);
        if(account){
            cout<<"Account holder: "<<account->getAccountHolderName()<<endl;
            cout<<"Current balance: "<<account->getBalance()<<" BDT."<<endl;
            cout<<"Enter your "<<MIN_PASSWORD_LENGTH<<"-digit password: ";
            cin>>password;
            cout<<endl;
            while(true){
                cout<<"Enter withdrawl amount: ";
                if(cin>>amount){
                    account->withdraw(amount,password);
                    if(amount> 0 && amount<= account->getBalance() && account->verifyPassword(password)){
                        saveAccounts();
                    }
                    break;
                }else cout<<"Invalid amount."<<endl;
            }
        }else cout<<"Account not found."<<endl;
    }
    void checkBalance(){
        string accNum;
        clearScreen();
        cout<<"===== Check Balance ====="<<endl;
        cout<<"Enter account number: "<<endl;
        cin>>accNum;
        BankAccount * account= findAccount(accNum);
        if(account){
            cout<<"Account holder: "<<account->getAccountHolderName()<<endl;
            cout<<"Current Balance: "<<account->getBalance()<<" BDT."<<endl;
        }else cout<<"Account not found."<<endl;
    }
    void displayAccountDetails(){
        string accNum;
        clearScreen();
        cout<<"===== Account Details ====="<<endl;
        cout<<"Enter account number: ";
        cin>>accNum;
        BankAccount * account=findAccount(accNum);
        if(account) account->displayAccountInfo();
        else cout<<"Account not found."<<endl;
    }
    void displayAllAccounts(){
        string adminUserId, adminPass;
        const string user = "admin", pass = "admin";
        clearScreen();
        cout<<"Enter admin user id: ";
        cin>>adminUserId;
        cout<<"Password: ";
        cin>>adminPass;

        if(adminUserId==user && adminPass==pass){
            cout<<"===== All Accounts ====="<<endl;
            if(accounts.empty()) cout<<"No accounts found."<<endl;
            else{
                for(auto & account:accounts){
                    cout<<"Account Number: "<<account.getAccountNumber();
                    cout<<"| Holder: "<<account.getAccountHolderName();
                    cout<<"| Type: "<<account.getAccountType();
                    cout<<"| Balance: "<<account.getBalance()<<" BDT."<<endl;

                }
            }
            cout<<"=============================="<<endl;
        }else cout<<"Invalid pass or user id."<<endl;
    }
};
void displayMenu(){
    cout<<"\n===== Banking System Menu ====="<<endl;
    cout<<"1. Create New Account"<<endl;
    cout<<"2. Deposit Money"<<endl;
    cout<<"3. Withdraw Money"<<endl;
    cout<<"4. Check Balance"<<endl;
    cout<<"5. Display Account Details"<<endl;
    cout<<"6. View All Acounts"<<endl;
    cout<<"7. Exit"<<endl;
    cout<<"=================================="<<endl;
    cout<<"Enter your choice (1-7): ";
}
int main(){
    BankingSystem bank;
    int choice;
    cout<<"Welcome to the Banking System"<<endl;
    while(true){
        displayMenu();
        if(!(cin>>choice)){
            cout<<"Invalid input. Please enter a number between 1 and 7."<<endl;
            continue;
        }
        switch (choice)
        {
        case 1:
            bank.createNewAccount();
            break;
        case 2:
            bank.depositMoney();
            break;
        case 3:
            bank.withdrawMoney();
            break;
        case 4:
            bank.checkBalance();
            break;
        case 5:
            bank.displayAccountDetails();
            break;
        case 6:
            bank.displayAllAccounts();
            break;
        case 7:
            cout<<"Thank you for using our Banking System."<<endl;
            return 0;
        default:
            cout<<"Invalid choice. Please enter a number between 1 and 8."<<endl;
        }
        cout<<"\nPress Enter to Continue.......";
        cin.ignore();
        cin.get();
    }
    return 0;
}
