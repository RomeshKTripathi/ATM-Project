#include <iostream>
#include <fstream>
#include <conio.h>
#include <cstring>
#include <iomanip>
#include <limits>


#define ACC_DATA_FILE "atm.dat" //Main data file contains All Account Records.
#define ACC_TEMP_FILE "temp_acc.dat" //Temprory data file used with delete operation.
#define NAME_MAX_LEN 40 //Maximul length of the User name.
#define CLOSE_BTM cout << "+--------------------------------+----------------+-------------------+" << endl;
// above macro used for adding bottom line in table (show all records).
#define SEP_DATA cout << "+--------------------------------+----------------+-------------------+" << endl \
                      << "|   USER NAME                    |  ACC. NO.      |  ACC BALANCE      |" << endl \
                      << "+--------------------------------+----------------+-------------------+" << endl;
// above macro used for creatin head of the table in (show all records);
#define CLOSE_LINE cout << "|" << endl;
// above macro used for closing the right boundry of the line in table.
using namespace std;

//width = 71
//width = 33 with boundry left. name
//width = 17 with boundry left. acc no
//width = 20 with boundry left. acc balance
//last right boundry 1;

//==============================================================================================================
class Atm
{
private:
    char name[40];
    int pin;
    int accNo = 0;
    double balance = 10.4;
    bool life = true;

public:
    int getAccNoFromLastRecord()
    {
        fstream file;
        if (is_exists(ACC_DATA_FILE))
        {
            file.open(ACC_DATA_FILE, ios::in | ios::binary | ios::out);
            if (file.tellp() != -1)
            {
                file.seekg(file.tellp() - sizeof(*this));
                return ((int)(this->accNo) + 1);
            }
        }
        else
        {
            return 1;
        }
    }

    // Above Function will be used to fetch last record of the file : main purpose is to 
    // get the last account number so that the createAccount() function can create new account number based
    // on previous it will save files from containing duplicate data.


    string &getNameOnly()
    {
        cout << "Enter Name to replace :  ";
        string u_name;
        getline(cin, u_name);
        return u_name;
    }


    void updateAccount()
    {
        if (is_exists(ACC_DATA_FILE))
        {
            // cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter Name Which You want to Update : ";
            string u_name;
            getline(cin, u_name);
            fstream file;
            file.open(ACC_DATA_FILE, ios::in | ios::ate | ios::out | ios::binary);
            file.seekg(0);
            file.read((char *)this, sizeof(*this));
            if (file.eof())
            {
                system("cls");
                cout << "No record exists in the file...!" << endl;
                return;
            }
            else
            {
                bool find = false;
                while (!file.eof())
                {
                    if (!strcmpi(this->name, u_name.c_str()))
                    {
                        cout << "Enter New Name : ";
                        string nu_name;
                        getline(cin, nu_name);
                        strcpy(this->name, nu_name.c_str());
                        file.seekp(file.tellg() - sizeof(*this));
                        file.write((char *)this, sizeof(*this));
                        find = true; // to display result messege.
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    else
                    {
                        file.read((char *)this, sizeof(*this));
                    }
                }
                if (find)
                    cout << "\nRecord Updated Successfully.\n ";
                else
                    cout << "\nRecord Not Found...!\n";
            }
        }
        else
        {
            cout << "File not Found\n";
        }
    }

    // Description: updateAccount() function is used to Update Account holder's name by name
    // this function only used by Admin users.

    void displayAllData() //function to Print Record in format.
    {
        setfill(' ');
        cout << "|   ";
        cout << setw(29) << left;
        cout << name << "|  ";
        cout << setw(14) << left;
        cout << accNo << "|  ";
        cout << setw(17) << left;
        cout << fixed << setprecision(2) << (double)balance;
        CLOSE_LINE
        CLOSE_BTM
    }

    // Description: displayAllData() function is used to display one record at a time from current object(this)
    // function is mainly used by Admin Users

    inline bool is_exists(string name)
    {
        ifstream fin;
        fin.open(name, ios::in);
        if (fin.is_open())
        {
            fin.close();
            return true;
        }
        else
            return false;
    } 
    
    // Description: is_exists() function is used to check that the any file is exists or not it takes name of the file
    // Function is used by Normal and Admin users (both)

    bool checkAuthenticity(int &acn, int &passcode)
    {
        if (is_exists(ACC_DATA_FILE))
        {

            ifstream fin;
            fin.open(ACC_DATA_FILE, ios::in | ios::binary);
            if (fin.is_open())
            {
                fin.read((char *)this, sizeof(*this));
                while (!fin.eof())
                {
                    if (this->accNo == acn and this->pin == passcode)
                    {
                        return true;
                    }
                    else
                    {
                        fin.read((char *)this, sizeof(*this));
                    }
                }
                system("cls");
                cout << "Invalid Credentials...!\n";
                return false;
            }
            else
            {
                system("cls");
                cout << "Can't Open file ...\n";
            }
        }
        else
        {
            system("cls");
            cout << "File not found...!\n";
            return false;
        }
    }

    //Description: checkAuthenticity() function is used to identify the user by given credentials ( Account number , pin) 
    // Function is mainly used by Normal User Module

    void createAccount()
    {

        if (is_exists(ACC_DATA_FILE))
        {
            cout << "Enter name : ";
            cin.getline(name, NAME_MAX_LEN);
            cout << "Enter pin : ";
            cin >> pin;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            accNo = getAccNoFromLastRecord();

            ofstream fout;
            fout.open(ACC_DATA_FILE, ios::app | ios::binary);
            fout.write((char *)this, sizeof(*this));
            cout << "\nAccount Created Successfully...";
            // _sleep(2000);
        }
        else
        {
            cout << "File not found...!" << endl;
        }
    }

    //Description: createAccount() function creates an Account, it takes Username and Pin to create an account.
    // function is mainly used by Admin users.


    void readAllRecords()
    {
        if (is_exists(ACC_DATA_FILE))
        {
            ifstream fin;
            system("cls");
            fin.open(ACC_DATA_FILE, ios::in | ios::binary);
            fin.read((char *)this, sizeof(*this));
            if (fin.eof())
            {
                cout << "No Record Found...!\nTry to add Some Record :)" << endl;
            }
            else
            {
                SEP_DATA
            }

            while (!fin.eof())
            {
                displayAllData();
                fin.read((char *)this, sizeof(*this));
            }
        }
        else
        {
            cout << "File Not Found...!\n";
        }
    }
    
    //Description: readAllRecords() function is used for reading all record from data file 
    // function is mainly used by Admin

    void deleteAccount()
    {
        int u_pin;
        int u_acc;

        cout << "Enter Account Number";
        cin >> u_acc;
        cout << "Enter PIN";
        cin >> u_pin;

        ifstream fin;
        ofstream fout;
        if (is_exists(ACC_DATA_FILE))
        {
            remove(ACC_TEMP_FILE);
            fin.open(ACC_DATA_FILE, ios::in | ios::binary);
            fout.open(ACC_TEMP_FILE, ios::app | ios::binary);
            if (fin.is_open() && fout.is_open())
            {
                fin.read((char *)this, sizeof(*this));
                while (!fin.eof())
                {
                    if (this->pin == u_pin && this->accNo == u_acc)
                    {
                    }
                    else
                    {
                        fout.write((char *)this, sizeof(*this));
                    }
                    fin.read((char *)this, sizeof(*this));
                }
            }
            else
            {
                cout << "file not found...!" << endl;
            }
        }
        else
        {
            cout << "Account data file not found...!" << endl;
        }
        fin.close();
        fout.close();
        remove(ACC_DATA_FILE);
        rename(ACC_TEMP_FILE, ACC_DATA_FILE);
        system("cls");
        cout << "Operation Successfully executed..\n";
    }

    //Description: function is used for deleting an Account By Account number and Pin
    // Function is mainly used by Admin users

    void superDelete()
    {
        string u_name;
        cin.ignore();
        cout << "Enter name to delete record : ";
        getline(cin, u_name);
        ifstream fin;
        ofstream fout;
        if (is_exists(ACC_DATA_FILE))
        {
            remove(ACC_TEMP_FILE);
            fin.open(ACC_DATA_FILE, ios::in | ios::binary);
            fout.open(ACC_TEMP_FILE, ios::app | ios::binary);
            if (fin.is_open() && fout.is_open())
            {
                fin.read((char *)this, sizeof(*this));
                while (!fin.eof())
                {
                    if (!strcmp(this->name, u_name.c_str()))
                    {
                    }
                    else
                    {
                        fout.write((char *)this, sizeof(*this));
                    }
                    fin.read((char *)this, sizeof(*this));
                }
            }
            else
            {
                cout << "file not found...!" << endl;
                return;
            }
        }
        else
        {
            cout << "Account data file not found...!" << endl;
            return;
        }
        fin.close();
        fout.close();
        remove(ACC_DATA_FILE);
        rename(ACC_TEMP_FILE, ACC_DATA_FILE);
        cout << "\nOperation Exicuted Successfully...\n";
    }

    //Description: superDelete() function is also used for deleting an Account from file By the username 
    // function is used by admin | this function is made for comfort of admin in deleting the account containig the unique name in whole file.

    
    void viewMyAccount(int &acn, int &passcode) {

        if(is_exists(ACC_DATA_FILE)){
            ifstream fin;
            fin.open(ACC_DATA_FILE,ios::in | ios::binary);
            if(fin.is_open()){

            }else{
                cout<<"Unable to open Data file...!\n";
                return;
            }
        }else{
            cout<<"Data file not found...!\n";
            return;
        }
    }

    //Description: viewMyAccount() function is used to view Individual Account Information 
    //function is used by Normal user.

    void updateMyName(int &acn, int &passcode) {}
    void updateMyPin(int &acn, int &passcode) {}
    void depositMoney(int &acn, int &passcode) {}
    void withdrawAmount(int &acn, int &passcode) {}

    void normalUser()
    {
        system("cls");
        // cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Enter Your Account No. : ";
        int acn, pin;
        cin >> acn;
        cout << "Enter Your Pin : ";
        cin >> pin;
        if (!checkAuthenticity(acn, pin))
        {
            return;
        }
        int again;
    again:
        cout << "+---------------------------------Welcome--------------------------------+" << endl
             << "|   1.   VIEW ACCOUNT INFO                                               |" << endl
             << "|   2.   UPDATE NAME                                                     |" << endl
             << "|   3.   UPDATE PIN                                                      |" << endl
             << "|   4.   WITHDRAW AMOUNT                                                 |" << endl
             << "|   5.   DEPOSIT MONEY                                                   |" << endl
             << "|   6.   TRANSFER FUND (NOT WORKING)                                     |" << endl
             << "|   7.   VIEW TRANSACTION HISTORY                                        |" << endl
             << "|   8.   PRINT TRANSACTION HISTORY                                       |" << endl
             << "|   9.   EXIT                                                            |" << endl
             << "+------------------------------------------------------------------------+" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        int choice;
        cin>>choice;
        switch (choice)
        {
        case 1:
            system("cls");
            viewMyAccount(acn, pin);
            goto again;
        case 2:
            updateMyName(acn, pin);
            system("cls");
            goto again;
        case 3:
            updateMyPin(acn, pin);
            system("cls");
            goto again;
        case 4:
            withdrawAmount(acn, pin);
            system("cls");
            goto again;
        case 5:
            depositMoney(acn, pin);
            system("cls");
            goto again;
        case 6:
            system("cls");
            cout << "Can't Transfer funds now Module is Under Development...\n";
            goto again;
        case 7:
            system("cls");
            cout << "Can't view T.H. Module is Under Development...\n";
            goto again;
        case 8:
            system("cls");
            cout << "Can't Print T.H. Module is Under Development...\n";
            goto again;
        case 9:
            return ;
        default:
            system("cls");
            cout<<"Invalid Choice...  \nTry Again \n";
            goto again;
        }
    }

    void deactivateAccount() {}
    void permanentDeleteChoice()
    {
        system("cls");
        int again;
    again:
        cout << "+---------------------------------Welcome--------------------------------+" << endl
             << "|   1.   DELETE BY ACC. NO. AND PIN                                      |" << endl
             << "|   2.   DELETE BY USERNAME   ! ('Delete Unique Records Only...!')       |" << endl
             << "|   3.   VIEW ALL RECORDS                                                |" << endl
             << "|   4.   DISCARD OPERATION                                               |" << endl
             << "+------------------------------------------------------------------------+" << endl;
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            system("cls");
            deleteAccount();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            goto again;
        case 2:
            superDelete();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            goto again;
        case 3:
            system("cls");
            readAllRecords();
            goto again;
        case 4:
            return;
        default:
            system("cls");
            cout << "Invalid Choice...! \nTry Again \n";
            goto again;
        }
    }
    void deleteChoice()
    {
        system("cls");
        int again;
    again:
        cout << "+---------------------------------Welcome--------------------------------+" << endl
             << "|   1.   DEACTIVATE ACCOUNT                                              |" << endl
             << "|   2.   DELETE ACCOUNT PERMANENTLY                                      |" << endl
             << "|   3.   DISCARD OPERATION                                               |" << endl
             << "+------------------------------------------------------------------------+" << endl;
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            system("cls");
            deactivateAccount();
            goto again;
        case 2:
            system("cls");
            permanentDeleteChoice();
            goto again;
        case 3:
            return;
        default:
            system("cls");
            cout << "Invalid Choice, Enter your choice again.\n";
            goto again;
        }
    }
    void adminUser()
    {
        system("cls");
        int choice, again;
    again:
        cout << "+---------------------------------Welcome--------------------------------+" << endl
             << "|   1.   CREATE NEW ACCOUNT                                              |" << endl
             << "|   2.   DELETE AN ACCOUNT                                               |" << endl
             << "|   3.   CORRECT USER NAMES  (BY NAME)                                   |" << endl
             << "|   4.   VIEW ALL RECORDS(ACCOUNTS)                                      |" << endl
             << "|   5.   EXIT FROM ADMIN PANEL                                           |" << endl
             << "+------------------------------------------------------------------------+" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            createAccount();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            goto again;
        case 2:
            deleteChoice();
            system("cls");
            goto again;
        case 3:
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            updateAccount();
            goto again;
        case 4:
            system("cls");
            readAllRecords();
            goto again;
        case 5:
            return;
        default:
            system("cls");
            cout << "\nInvalid Choice , Enter your choice again...\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            goto again;
        }
    }
};

int main()
{
    Atm user;
    int choice, again, _again;
again:
    system("cls");
_again:

    cout << "+---------------------------------Welcome--------------------------------+" << endl
         << "|   1.  LOGIN AS NORMAL USER                                             |" << endl
         << "|   2.  LOGIN AS ADMIN  USER                                             |" << endl
         << "|   3.  EXIT                                                             |" << endl
         << "+------------------------------------------------------------------------+" << endl;
    cin >> choice;
    switch (choice)
    {
    case 1:
        user.normalUser();
        goto again;
    case 2:
        user.adminUser();
        goto again;
    case 3:
        return 0;
    default:
        system("cls");
        cout << "Invalid Choice...Try Again\n";
        goto _again;
        break;
    }
    return 0;
}