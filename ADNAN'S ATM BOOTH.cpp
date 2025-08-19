#include <iostream>
#include <conio.h>
using namespace std;

void display()
{
    cout << "\n\n\n\n   *Welcome to Adnan's ATM Booth*" << endl;
    cout << "   1. Check Balance" << endl;
    cout << "   2. Deposit Money" << endl;
    cout << "   3. Withdraw Money" << endl;
    cout << "   4. Exit" << endl;
}

bool checkPin()
{
    int correctPin = 1234;
    int enteredPin;
    int attempts = 0;

    while (attempts < 3)
    {
        cout << "Enter your 4-digit PIN: ";
        cin >> enteredPin;

        if (enteredPin == correctPin)
        {
            return true;
        }
        else
        {
            cout << "Incorrect PIN. Try again." << endl;
            attempts++;
        }
    }

    cout << "Too many incorrect attempts. Exiting..." << endl;
    return false;
}

int main()
{
    int balance = 50000;
    int option;
    int deposit;
    int withdraw;

    if (!checkPin())
    {
        return 0;
    }

    while (true)
    {
        display();
        cout << "\n   Choose Option: ";
        cin >> option;

        system("cls");

        switch (option)
        {
            case 1:
                cout << "Your Balance Now: " << balance << endl;
                break;

            case 2:
                cout << "Enter Deposit Amount: ";
                cin >> deposit;
                balance += deposit;
                cout << "You have deposited: " << deposit << endl;
                cout << "My Current Balance Now: " << balance << endl;
                break;

            case 3:
                cout << "Enter Withdraw Amount: ";
                cin >> withdraw;

                if (withdraw <= balance)
                {
                    balance -= withdraw;
                    cout << "You have withdrawn: " << withdraw << endl;
                    cout << "My Current Balance Now: " << balance << endl;
                }
                else
                {
                    cout << "Insufficient Balance!" << endl;
                }
                break;

            case 4:
                cout << "Thank you for using Adnan's ATM Booth!" << endl;
                return 0;

            default:
                cout << "Wrong Input, please try again." << endl;
        }
    }

    return 0;
}
