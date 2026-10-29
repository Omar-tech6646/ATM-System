#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";

void ShowATMMainMenu();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void Login();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;

enum enATMMainMenuOptions
{
    eQuickWithdraw = 1, eNormalWithdraw = 2,
    eDeposit = 3, eCheckBalance = 4,
    eClientInfo = 5, eLogout = 6
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientRecordBalancesLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y / n ? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);

                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
        return false;
    }
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

short ReadQuickWithdrawAmount()
{
    short Choice = 0;
    do
    {
        cout << "\nChoose what to do from [1 to 9]?\n";
        cin >> Choice;

    } while (Choice < 1 || Choice > 9);
   

    return Choice;
}

int ReadNormalWithdrawAmount()
{
    int WithdrawAmount = 0;
    do
    {
        cout << "\nEnter an amount mupltiple of 5?\n";
        cin >> WithdrawAmount;

    } while (WithdrawAmount %5 != 0);

    return WithdrawAmount;
}

double ReadDepositAmount()
{
    double DepositAmount = 0;

    do
    {
        cout << "\nEnter positive Deposit Amount?\n";
        cin >> DepositAmount;

    } while (DepositAmount <= 0);

    return DepositAmount;
}

short GetQuickWithdrawAmount(short QuickWithdrawOption)
{
    switch (QuickWithdrawOption)
    {
    case 1:
        return 20;

    case 2:
        return 50;

    case 3:
        return 100;

    case 4:
        return 200;

    case 5:
        return 400;

    case 6:
        return 600;

    case 7:
        return 800;

    case 8:
        return 1000;

    default:
        return 0;
    }
  
}

void PerformQuickWithdraw(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)
        return;

    short WithdrawBalance = GetQuickWithdrawAmount(QuickWithdrawOption);

    if (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "\nPress any key to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);
}

void PerformNormalWithdraw()
{
    int WithdrawAmount = ReadNormalWithdrawAmount();

    if (WithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "\nPress any key to continue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);

    CurrentClient.AccountBalance -= WithdrawAmount;
}

void PerformDeposit()
{
    double DepositAmount = ReadDepositAmount();

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);

    CurrentClient.AccountBalance += DepositAmount;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw Screen\n";
    cout << "===========================================\n";

    cout << "\t[1] 20"; cout << "\t\t[2] 50\n";
    cout << "\t[3] 100"; cout << "\t\t[4] 200\n";
    cout << "\t[5] 400"; cout << "\t\t[6] 600\n";
    cout << "\t[7] 800"; cout << "\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "\nYour Balance is " << CurrentClient.AccountBalance << endl;

    PerformQuickWithdraw(ReadQuickWithdrawAmount());
    
}

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";
    cout << "\nYour Balance is " << CurrentClient.AccountBalance << endl;

    PerformNormalWithdraw();
}

void ShowDepositScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";
    cout << "\nYour Balance is " << CurrentClient.AccountBalance << endl;

    PerformDeposit();
}

void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";

    cout << "\nYour balance is " << CurrentClient.AccountBalance << endl;
}

void ShowClientInfo()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\t Client Info Screen\n";
    cout << "===========================================\n";

    PrintClientRecordLine(CurrentClient);

    /*cout << "\t" << "Client name: " << CurrentClient.Name << endl;
    cout << "\t" << "Client phone: " << CurrentClient.Phone << endl;
    cout << "\t" << "Client account number: " << CurrentClient.AccountNumber << endl;
    cout << "\t" << "Client account balance: " << CurrentClient.AccountBalance << endl;
    cout << "\t" << "Client pin code: " << CurrentClient.PinCode << endl;*/

    cout << "\n\nPress any key to continue...";
    system("pause>0");
    ShowATMMainMenu();
}

void Logout()
{
    Login();
}

void GoBackToATMMainMenu()
{
    cout << "\n\nPress any key to go back to ATM Main Menu...";
    system("pause>0");
    ShowATMMainMenu();

}

short ReadATMMainMenuOption()
{
    short Choice = 0;

    do
    {
        cout << "Choose what do you want to do? [1 to 6]? ";
        cin >> Choice;

    } while (Choice < 1 || Choice > 6);
   

    return Choice;
}

void PerfromATMMainMenuOption(enATMMainMenuOptions ATMMainMenuOption)
{
    switch (ATMMainMenuOption)
    {
    case enATMMainMenuOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToATMMainMenu();
        break;
    }
    case enATMMainMenuOptions::eNormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToATMMainMenu();
        break;

    case enATMMainMenuOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToATMMainMenu();
        break;

    case enATMMainMenuOptions::eCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToATMMainMenu();
        break;

    case enATMMainMenuOptions::eClientInfo:
        system("cls");
        ShowClientInfo();
        GoBackToATMMainMenu();
        break;

    case enATMMainMenuOptions::eLogout:
        system("cls");
        Logout();
        break;

    }
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }

    }
    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

void ShowATMMainMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Client Info.\n";
    cout << "\t[6] Logout.\n";
    cout << "===========================================\n";
    PerfromATMMainMenuOption((enATMMainMenuOptions)ReadATMMainMenuOption());
}

void Login()
{
    bool LoginFailed = false;

    string AccountNumber, PinCode;

    do
    {
        system("cls");
        cout << "-------------------------------------------\n";
        cout << "\t\tLogin Screen\n";
        cout << "-------------------------------------------\n";

        if (LoginFailed)
        {
            cout << "\nInvalid Account Number / Pin code!\n";
        }
        cout << "Enter Account Number?\n";
        cin >> AccountNumber;

        cout << "\nEnter Pin code?\n";
        cin >> PinCode;

        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFailed);

    ShowATMMainMenu();

}

int main()
{
   Login();
    
   
    system("pause>0");
    return 0;
}