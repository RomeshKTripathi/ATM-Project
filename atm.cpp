#include <iostream>
#include <fstream>
#include <conio.h>
#include <cstring>
#include <iomanip>
#include <string>
#include <limits>

#define ACC_DATA_FILE "atm.dat"      //Main data file contains All Account Records.
#define ACC_TEMP_FILE "temp_acc.dat" //Temprory data file used with delete operation.
#define NAME_MAX_LEN 40              //Maximul length of the User name.
// above macro used for adding bottom line in table (show all records).

#define SEP_DATA cout << "+--------------------------------+-----------+-------------------+----------+" << endl \
                      << "|   USER NAME                    |  ACC. NO. |  ACC BALANCE      |  STATUS  |" << endl \
                      << "+--------------------------------+-----------+-------------------+----------+" << endl;
#define CLOSE_BTM cout << "+--------------------------------+-----------+-------------------+----------+" << endl;

#define SEP_DATA_1 cout                                                                                                     \
                       << "+--------------------------------+-----------+-------------------+----------+---------+" << endl \
                       << "|   USER NAME                    |  ACC. NO. |  ACC BALANCE      |  STATUS  |  PIN    |" << endl \
                       << "+--------------------------------+-----------+-------------------+----------+---------+" << endl;
#define CLOSE_BTM_1 cout << "+--------------------------------+-----------+-------------------+----------+---------+" << endl;
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
    double balance = 9.5;
    bool life = true;

public:

    int enterValidPin()
    {
        char ch;
        int u_pin = 0;
        string s_pin;
        for (int i = 0; i < 4; i++)
        {

            ch = getch();
            if (ch >= '1' && ch <= '9')
            {
                cout << ch;
                s_pin.push_back(ch);
                cin.clear();
            }
            else
            {
                i -= 1;
            }
        }
        u_pin = stoi(s_pin);
        // cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin.clear();
        return u_pin;
    }

    //Description: enterValidPin() function is used, to get an valid pin from user
    //function is used by admin and normal user.

    int getAccNoFromLastRecord()
    {
        fstream file;
        if (is_exists(ACC_DATA_FILE))
        {
            file.open(ACC_DATA_FILE, ios::in | ios::binary | ios::out);
            if (file.tellp() != -1)
            {
                file.seekg(file.tellp() - sizeof(*this));
                file.read((char *)this, sizeof(*this));
                file.close();
                return ((int)(this->accNo) + 1);
            }
            else
            {
                file.close();
                return 1;
            }
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
                {
                    system("cls");
                    cout << "\nRecord Not Found...!\n";
                    return;
                }
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
        string acc_status;
        if (this->life)
        {
            acc_status = "ACTIVE";
        }
        else
        {
            acc_status = "CLOSED";
        }
        setfill(' ');
        cout << "|   ";
        cout << setw(29) << left;
        cout << name << "|  ";
        cout << setw(9) << left;
        cout << accNo << "|  ";
        cout << setw(17) << left;
        cout << fixed << setprecision(2) << (double)balance << "|  ";
        cout << setw(8) << left << acc_status;
        CLOSE_LINE
        CLOSE_BTM
    }

    // Description: displayAllData() function is used to display one record at a time from current object(this)
    // function is mainly used by Admin Users

    void displayAllDataAdvance()
    {

        string acc_status;
        if (this->life)
        {
            acc_status = "ACTIVE";
        }
        else
        {
            acc_status = "CLOSED";
        }

        setfill(' ');
        cout << "|   ";
        cout << setw(29) << left;
        cout << name << "|  ";
        cout << setw(9) << left;
        cout << accNo << "|  ";
        cout << setw(17) << left;
        cout << fixed << setprecision(2) << (double)balance << "|  ";
        cout << setw(8) << left << acc_status << "|  ";
        cout << setw(7) << left;
        cout << pin;
        CLOSE_LINE
        CLOSE_BTM_1
    }

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
                        if(this->life){
                            return true;
                        }else{
                            cout<<"Sorry, Your Account is Deactivated...\nRequest the Admin to Activate It... ";
                            getch();
                            return false;
                        }
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
            cout << "Enter 4 Numeric Digit Pin Do not include '0' : ";
            pin = enterValidPin();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            accNo = getAccNoFromLastRecord();
            life = true;
            balance = 4.5;
            ofstream fout;
            fout.open(ACC_DATA_FILE, ios::app | ios::binary);
            fout.write((char *)this, sizeof(*this));
            fout.close();
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

    void readAllRecords(const bool &is_advance)
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
                if (is_advance)
                    SEP_DATA_1
                else
                    SEP_DATA
            }

            while (!fin.eof())
            {
                if (is_advance)
                {
                    displayAllDataAdvance();
                }
                else
                {
                    displayAllData();
                }
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
                    if (!strcmpi(this->name, u_name.c_str()))
                    {
                    }
                    else
                    {
                        fout.write((char *)this, sizeof(*this));
                    }
                    fin.read((char *)this, sizeof(*this));
                }
                fin.close();
                fout.close();
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
        system("cls");
        cout << "\nOperation Exicuted Successfully...\n";
    }

    //Description: superDelete() function is also used for deleting an Account from file By the username
    // function is used by admin | this function is made for comfort of admin in deleting the account containig the unique name in whole file.

    void displayMyData(){
        system("cls");
        string myLife;
        if(this->life){
            myLife = "Active";
        }else myLife = "Closed";
        setfill(' ');
        cout<<"+-----------------------------------------------------+"<<endl
            <<"|  Name        :  "<<setw(34)<<left<<this->name<<"  |"<<endl
            <<"|  Account No  :  "<<setw(34)<<left<<this->accNo<<"  |"<<endl
            <<"|  Balance     :  "<<setw(34)<<left<<setprecision(2)<<this->balance<<"  |"<<endl
            <<"|  Status      :  "<<setw(34)<<left<<myLife<<"  |"<<endl
            <<"|  Pin         :  "<<setw(34)<<left<<this->pin<<"  |"<<endl
            <<"+-----------------------------------------------------+\n"<<endl;
    }

    void viewMyAccount(int &acn, int &passcode)
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
                    if (this->accNo == acn && this->pin == passcode)
                    {
                        displayMyData();
                        fin.close();
                        return;
                    }else
                    fin.read((char*)this,sizeof(*this));
                }
            }
            else
            {
                cout << "Unable to open Data file...!\n";
                return;
            }
        }
        else
        {
            cout << "Data file not found...!\n";
            return;
        }
    }

    //Description: viewMyAccount() function is used to view Individual Account Information
    //function is used by Normal user.

    void updateMyName(int &acn, int &passcode)
    {
        system("cls");
        string u_name;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter New Name to Update : ";
        getline(cin, u_name);

        fstream file;
        file.open(ACC_DATA_FILE, ios::in | ios::out | ios::binary);
        if (file.is_open())
        {
            file.read((char *)this, sizeof(*this));
            while (!file.eof())
            {
                if (this->pin == passcode && this->accNo == acn)
                {
                    file.seekp(file.tellg() - sizeof(*this));
                    strcpy(this->name, u_name.c_str());
                    file.write((char *)this, sizeof(*this));
                    cout << "Name Updated Successfully...\n";
                    file.close();
                    return;
                }
                else
                {
                    file.read((char *)this, sizeof(*this));
                }
            }
        }
        else
        {
            cout << "Can't open file...\n";
            return;
        }
    }
    void updateMyPin(int &acn, int &passcode) {
        int n_pin;
        system("cls");
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Enter New 4 digit pin : ";
        n_pin = enterValidPin();
        fstream file ;
        file.open(ACC_DATA_FILE,ios::in | ios::out | ios::binary);
        file.seekg(0);
        file.read((char*)this,sizeof(*this));
        if(!file.eof()){
            while(!file.eof()){
                if(this->accNo == acn && this->pin == passcode){
                    this->pin = n_pin;
                    file.seekp(file.tellg()-sizeof(*this));
                    file.write((char*)this, sizeof(*this));
                    cout<<"\nPin Updated Successfully...\n";
                    passcode = n_pin;
                    return;
                }else{
                    file.read((char*)this,sizeof(*this));
                }
            }
            return ;

        }else{
            cout<<"Data not found..\n";
            return;
        }


    }
    void depositMoney(int &acn, int &passcode) {
        system("cls");
        double amount;
        cout<<"Amount should be within range of 5 - 20,000 \nEnter Amount To Deposit : ";
        cin>>amount;
        amount = static_cast<double>(amount);

        if(amount <=20000 && amount>5){
            fstream file;
            if(is_exists(ACC_DATA_FILE)){
                file.open(ACC_DATA_FILE,ios::in | ios::out | ios::binary);
                if(file.is_open()){
                    file.seekg(0);
                    file.read((char*)this,sizeof(*this));
                    while(!file.eof()){
                        if(this->accNo == acn && this->pin == passcode){
                            this->balance += amount;
                            file.seekp(file.tellg() - sizeof(*this));
                            file.write((char*)this,sizeof(*this));
                            file.close();
                            cout<<"Deposit Complete...!\n";
                            return;
                        }else{
                            file.read((char*)this,sizeof(*this));
                        }
                    }
                }else{
                    cout<<"Can't open file...!\n";
                    return;
                }
            }
        }else{
            cout<<"Enter Valid Amount...\n";
            getch();
            depositMoney(acn,passcode);
        }
    }
    bool validateWithdrawalAmount(int &acn,int &passcode,double &amount){
        if(is_exists(ACC_DATA_FILE)){
            ifstream fin;
            fin.open(ACC_DATA_FILE,ios::in | ios::binary);
            if(fin.is_open()){
                fin.read((char*)this,sizeof(*this));
                while(!fin.eof()){
                    if(this->accNo == acn && this->pin == passcode){
                        if(this->balance >= (amount+0.5)){
                            return true;
                        }else{
                            return false;
                        }
                    }else{
                        fin.read((char*)this,sizeof(*this));
                    }
                }
            }else{
                system("cls");
                cout<<"Can't Open File...!\n";
                return false;
            }
        }
    }
    void withdrawAmount(int &acn, int &passcode) {
        double amount;
        cout<<"Enter Amount to Withdraw : ";
        cin>>amount;
        amount = static_cast<double>(amount);
        if(validateWithdrawalAmount(acn,passcode,amount)){
            fstream file;
            file.open(ACC_DATA_FILE,ios::in | ios::out | ios::binary);
            file.seekg(0);
            if(file.is_open()){
                file.read((char*)this,sizeof(*this));
                while(!file.eof()){
                    if(this->accNo == acn && this->pin == passcode){
                        this->balance -= (amount+0.5);
                        file.seekp(file.tellg() - sizeof(*this));
                        file.write((char*)this,sizeof(*this));
                        file.close();

                        cout<<"Withdrawal Successfull...\n";
                        return ;
                    }else{
                        file.read((char*)this,sizeof(*this));
                    }
                }
            }


        }else{
            system("cls");
            cout<<"\nInvalid Amount enterd...\nPlease Enter Valid Amount...!\n";
            withdrawAmount(acn,passcode);
        }
    }

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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            system("cls");
            viewMyAccount(acn, pin);
            goto again;
        case 2:
            updateMyName(acn, pin);
            getch();
            system("cls");
            goto again;
        case 3:
            updateMyPin(acn, pin);
            // cin.ignore(numeric_limits<streamsize>::max(),'\n');
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
            return;
        default:
            system("cls");
            cout << "Invalid Choice...  \nTry Again \n";
            goto again;
        }
    }

    void deactivateAccount()
    {
        int u_acc, u_pin;
        cout << "Enter Acc no : ";
        cin >> u_acc;
        cout << "Enter Pin : ";
        cin >> u_pin;

        fstream file;
        if (is_exists(ACC_DATA_FILE))
        {
            file.open(ACC_DATA_FILE, ios::in | ios::out | ios::ate | ios::binary);
            if (file.is_open())
            {
                file.seekg(0);
                file.read((char *)this, sizeof(*this));
                if (file.eof())
                {
                    cout << "There is no Record in Data File...\n";
                    return;
                }
                else
                {
                    while (!file.eof())
                    {
                        if (this->pin == u_pin && this->accNo == u_acc)
                        {
                            file.seekp(file.tellg() - sizeof(*this));
                            if (this->life)
                            {
                                this->life = false;
                            }
                            else
                            {
                                cout << "Account is Already Closed, Do you want to Activate is y/n : \n";
                                char ch;
                                int re_enter;
                            re_enter:
                                ch = getch();
                                if (ch == 'y' or ch == 'Y')
                                {
                                    cout << ch << "\n";
                                    this->life = true;
                                    file.write((char *)this, sizeof(*this));
                                    file.close();
                                    cout << "Account Activated...\n";
                                    return;
                                }
                                else if (ch == 'n' or ch == 'N')
                                {
                                    return;
                                }
                                else
                                    goto re_enter;
                            }

                            file.write((char *)this, sizeof(*this));
                            file.close();
                            cout << "Account Deactivated...\n";
                            return;
                        }
                        else
                        {
                            file.read((char *)this, sizeof(*this));
                        }
                    }
                    cout << "Record not found...\n";
                    return;
                }
            }
            else
            {
                cout << "Can't open file...\n";
                return;
            }
        }
        else
        {
            cout << "File not found...\n";
            return;
        }
    }

    void adminUser()
    {
        system("cls");
        int choice, again;
    again:
        cout << "+----------------------------------ADMIN---------------------------------+" << endl
             << "|   1.   CREATE NEW ACCOUNT                                              |" << endl
             << "|   2.   CORRECT USER NAMES  (BY NAME)                                   |" << endl
             << "|                                                                        |" << endl
             << "|   3.   VIEW ALL RECORDS(ACCOUNTS)                                      |" << endl
             << "|   4.   VIEW ALL RECORDS ADVANCE (ACCOUNTS)                             |" << endl
             << "|                                                                        |" << endl
             << "|   5.   DEACTIVATE ACCOUNT                                              |" << endl
             << "|                                                                        |" << endl
             << "|   6.   DELETE BY USERNAME                                              |" << endl
             << "|   7.   DELETE BY ACC NO & PIN                                          |" << endl
             << "|                                                                        |" << endl
             << "|   8.   EXIT FROM ADMIN PANEL                                           |" << endl
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            updateAccount();
            goto again;
        case 3:
            system("cls");
            readAllRecords(0);
            goto again;
        case 4:
            system("cls");
            readAllRecords(1);
            goto again;
        case 5:
            system("cls");
            deactivateAccount();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            goto again;
        case 6:
            superDelete();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            goto again;
        case 7:
            system("cls");
            deleteAccount();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            goto again;
        case 8:
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