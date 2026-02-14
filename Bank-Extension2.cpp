#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

enum enPermisionsValue { allpermisions = -1, showclients = 1, addnewclients = 2, deleteclient = 4, updateclient = 8, findclient = 16, transaction = 32, manageusers = 64 };
enum enMainMenueOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eManageUsers = 7, eLogout = 8 };
enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };
enum enManageUsers { eListUsers = 1, eAddUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenu = 6 };

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

struct sUser {

    string UserName;
    string UserPassword;
    short UserPermisions = 0;
    bool MarkForDelete = false;
};

void ShowAddNewUsersScreen();
void ShowTransactionsMenue(sUser User);
void ManageUsersScreen(sUser User);
void ShowMainMenue(sUser User);
bool FindUserByUserName(string UserName, vector<sUser> vUsers, sUser& User);
vector<sUser> LoadDataUsersFromFile(string FileName);
void GivePermisions(sUser& User);

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

bool CheckPermisions(sUser User, enPermisionsValue PermissionValue) {

    if (User.UserPermisions == enPermisionsValue::allpermisions)
        return true;

    if ((User.UserPermisions & PermissionValue) == PermissionValue)
        return true;

    return false;

    /*switch (PermissionValue) {
    
    case enPermisionsValue::showclients:
        if ((User.UserPermisions & enPermisionsValue::showclients) == enPermisionsValue::showclients)
            return true;
    return false;

    case enPermisionsValue::addnewclients:
        if ((User.UserPermisions & enPermisionsValue::addnewclients) == enPermisionsValue::addnewclients)
            return true;
    return false;

    case enPermisionsValue::deleteclient:
        if ((User.UserPermisions & enPermisionsValue::deleteclient) == enPermisionsValue::deleteclient)
            return true;
    return false;

    case enPermisionsValue::updateclient:
        if ((User.UserPermisions & enPermisionsValue::updateclient) == enPermisionsValue::updateclient)
            return true;
    return false;

    case enPermisionsValue::findclient:
        if ((User.UserPermisions & enPermisionsValue::findclient) == enPermisionsValue::findclient)
            return true;
    return false;

    case enPermisionsValue::transaction:
        if ((User.UserPermisions & enPermisionsValue::transaction) == enPermisionsValue::transaction)
            return true;
    return false;

    case enPermisionsValue::manageusers:
        if ((User.UserPermisions & enPermisionsValue::manageusers) == enPermisionsValue::manageusers)
            return true;
    return false;

    default:
        return false;
    }*/
}

sClient ConvertLinetoRecordClients(string Line, string Seperator = "#//#")
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

void NoAccessScreen() {

    cout << "\n------------------------------\n";
    cout << "Access Denied,\nYou Dont have permission to do this,\nPlease conact your admin.\n";
    cout << "------------------------------\n";
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

string ConvertRecordToLine(sUser User, string Seperator = "#//#")
{

    string stUserRecord = "";

    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.UserPassword + Seperator;
    stUserRecord += to_string(User.UserPermisions);

    return stUserRecord;
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

            Client = ConvertLinetoRecordClients(Line);
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

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

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

            Client = ConvertLinetoRecordClients(Line);

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

void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientsScreen(sUser User)
{

    if (CheckPermisions(User, enPermisionsValue::showclients)) {

        vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

        cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;

        cout << "| " << left << setw(15) << "Accout Number";
        cout << "| " << left << setw(10) << "Pin Code";
        cout << "| " << left << setw(40) << "Client Name";
        cout << "| " << left << setw(12) << "Phone";
        cout << "| " << left << setw(12) << "Balance";
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;

        if (vClients.size() == 0)
            cout << "\t\t\t\tNo Clients Available In the System!";
        else
            for (sClient Client : vClients) {

                PrintClientRecordLine(Client);
                cout << endl;
            }

        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
    }
    else
        NoAccessScreen();
}

void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number   : " << Client.AccountNumber;
    cout << "\nPin Code        : " << Client.PinCode;
    cout << "\nName            : " << Client.Name;
    cout << "\nPhone           : " << Client.Phone;
    cout << "\nAccount Balance : " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}

void PrintUserCard(sUser User) {

    cout << "\nThe following are the User details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.UserName;
    cout << "\nPassword    : " << User.UserPassword;
    cout << "\npermissions : " << User.UserPermisions;
    cout << "\n-----------------------------------\n";
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

sUser ChangeUserRecord(string UserName) {

    sUser User;

    User.UserName = UserName;

    cout << "\n\nEnter Password ? ";
    getline(cin >> ws, User.UserPassword);

    GivePermisions(User);

    return User;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}

bool MarkUserForDeleteUserName(string UserName, vector <sUser>& vusers)
{

    for (sUser& User : vusers)
    {

        if (User.UserName == UserName)
        {
            User.MarkForDelete = true;
            return true;
        }

    }

    return false;

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

vector <sUser> SaveUsersDataToFile(string FileName, vector <sUser> vUsers)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sUser User : vUsers)
        {

            if (User.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(User);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vUsers;

}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DeleteUserByUserName(string UserName, vector <sUser>& vUsers) {

    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUsers, User))
    {

        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteUserName(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            //Refresh Clients 
            vUsers = LoadDataUsersFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with username (" << UserName << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateUserByUserName(string UserName, vector <sUser>& vUsers)
{

    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUsers, User)) {

        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User ? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sUser& C : vUsers)
            {
                if (C.UserName == UserName)
                {
                    C = ChangeUserRecord(UserName);
                    break;
                }

            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
            return true;
        }

    }
    else{

        cout << "\nUser with User Name (" << UserName << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients) {

    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
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

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

string ReadUserName() {

    string UserName = "";
    cout << "\nPlease Enter Username ? ";
    cin >> UserName;
    return UserName;
}

void ShowDeleteUserScreen(sUser User) {

        cout << "\n-----------------------------------\n";
        cout << "\tDelete User Screen";
        cout << "\n-----------------------------------\n";

        string UserName = ReadUserName();

        if (UserName == "Admin") {
        
            cout << "Can not delete admin from system!!";
            return;
        }

        vector <sUser> vUser = LoadDataUsersFromFile(UsersFileName);
        DeleteUserByUserName(UserName, vUser);

}

void ShowDeleteClientScreen(sUser User)
{
    if (CheckPermisions(User, enPermisionsValue::deleteclient)) {

        cout << "\n-----------------------------------\n";
        cout << "\tDelete Client Screen";
        cout << "\n-----------------------------------\n";

        vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        string AccountNumber = ReadClientAccountNumber();
        DeleteClientByAccountNumber(AccountNumber, vClients);
    }
    else
        NoAccessScreen();

}

void ShowUpdateUserScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUsers = LoadDataUsersFromFile(UsersFileName);
    string UserName = ReadUserName();
    UpdateUserByUserName(UserName, vUsers);
}

void ShowUpdateClientScreen(sUser User) {

    if (CheckPermisions(User, enPermisionsValue::updateclient)) {

        cout << "\n-----------------------------------\n";
        cout << "\tUpdate Client Info Screen";
        cout << "\n-----------------------------------\n";

        vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        string AccountNumber = ReadClientAccountNumber();
        UpdateClientByAccountNumber(AccountNumber, vClients);
    }
    else
        NoAccessScreen();
}

void ShowAddNewClientsScreen(sUser User)
{
    if (CheckPermisions(User, enPermisionsValue::addnewclients)) {

        cout << "\n-----------------------------------\n";
        cout << "\tAdd New Clients Screen";
        cout << "\n-----------------------------------\n";

        AddNewClients();
    }
    else
        NoAccessScreen();

}

void ShowFindUserScreen() {

        cout << "\n-----------------------------------\n";
        cout << "\tFind User Screen";
        cout << "\n-----------------------------------\n";

        vector <sUser> vUsers = LoadDataUsersFromFile(UsersFileName);
        sUser User;
        string UserName = ReadUserName();

        if (FindUserByUserName(UserName, vUsers, User))
            PrintUserCard(User);
        else
            cout << "\nClient with Account Number[" << UserName << "] is not found!";
}

void ShowFindClientScreen(sUser User)
{
    if (CheckPermisions(User, enPermisionsValue::findclient)) {

        cout << "\n-----------------------------------\n";
        cout << "\tFind Client Screen";
        cout << "\n-----------------------------------\n";

        vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        sClient Client;
        string AccountNumber = ReadClientAccountNumber();
        if (FindClientByAccountNumber(AccountNumber, vClients, Client))
            PrintClientCard(Client);
        else
            cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
    }
    else
        NoAccessScreen();
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}

void GoBackToMainMenue(sUser User)
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue(User);

}

void GoBackToTransactionsMenue(sUser User)
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue(User);

}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption, sUser User)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:

        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue(User);
        break;

    case enTransactionsMenueOptions::eWithdraw:

        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue(User);
        break;

    case enTransactionsMenueOptions::eShowTotalBalance:

        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue(User);
        break;

    case enTransactionsMenueOptions::eShowMainMenue:
        ShowMainMenue(User);
    }
}

void ShowTransactionsMenue(sUser User)
{
    if (CheckPermisions(User, enPermisionsValue::transaction)) {

        system("cls");
        cout << "===========================================\n";
        cout << "\t\tTransactions Menue Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] Deposit.\n";
        cout << "\t[2] Withdraw.\n";
        cout << "\t[3] Total Balances.\n";
        cout << "\t[4] Main Menue.\n";
        cout << "===========================================\n";
        PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption(), User);
    }
    else
        NoAccessScreen();
}
sUser ConvertLineToRecordUsers(string Line) {

    sUser User;
    vector<string> vUser = SplitString(Line, "#//#");

    User.UserName = vUser[0];
    User.UserPassword = vUser[1];
    User.UserPermisions = stoi(vUser[2]);

    return User;
}

vector<sUser> LoadDataUsersFromFile(string FileName) {

    vector<sUser> vUsers;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
    
        string Line;
        sUser User;

        while (getline(MyFile, Line)) {
        
            vUsers.push_back(ConvertLineToRecordUsers(Line));
        }

        MyFile.close();
    }

    return vUsers;
}

bool FindUserByUserNameAndPassword(string UserName, string UserPassword, vector<sUser> vUsers, sUser& User) {

    for (sUser& U : vUsers)
        if (U.UserName == UserName && U.UserPassword == UserPassword) {

            User = U;
            return true;
        }

    return false;
}

bool FindUserByUserName(string UserName, vector<sUser> vUsers, sUser& User) {

    for (sUser& U : vUsers)
        if (U.UserName == UserName) {

            User = U;
            return true;
        }

    return false;
}

void GivePermisions(sUser& User) {

    char Access = 'n';
    cout << "Do you want to give full access ? Y/N ? ";
    cin >> Access;

    if (Access == 'Y' || Access == 'y') {
        
        User.UserPermisions = enPermisionsValue::allpermisions;
        return;
    }

    cout << "\nDo you want give access to :\n\n";

    cout << "\nShow clients list ? y/n ? ";
    cin >> Access;
    if (Access == 'Y' || Access == 'y')
        User.UserPermisions = (User.UserPermisions | enPermisionsValue::showclients);

    cout << "Add new clients ? y/n ? ";
    cin >> Access;
    if (Access == 'Y' || Access == 'y')
        User.UserPermisions = (User.UserPermisions | enPermisionsValue::addnewclients);

    cout << "Delete clients ? y/n ? ";
    cin >> Access;
    if (Access == 'Y' || Access == 'y')
        User.UserPermisions = (User.UserPermisions | enPermisionsValue::deleteclient);

    cout << "Update clients ? y/n ? ";
    cin >> Access;
    if (Access == 'Y' || Access == 'y')
        User.UserPermisions = (User.UserPermisions | enPermisionsValue::updateclient);

    cout << "Find clients ? y/n ? ";
    cin >> Access;
    if (Access == 'Y' || Access == 'y')
        User.UserPermisions = (User.UserPermisions | enPermisionsValue::findclient);

    cout << "Transactions ? y/n ? ";
    cin >> Access;
    if (Access == 'Y' || Access == 'y')
        User.UserPermisions = (User.UserPermisions | enPermisionsValue::transaction);

    cout << "Manage users ? y/n ? ";
    cin >> Access;
    if (Access == 'Y' || Access == 'y')
        User.UserPermisions = (User.UserPermisions | enPermisionsValue::manageusers);
}

void GoBackToManageUsersScreen(sUser User)
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ManageUsersScreen(User);

}

void PrintUserRecordLine(sUser User) {

    cout << "| " << setw(20) << left << User.UserName;
    cout << "| " << setw(20) << left << User.UserPassword;
    cout << "| " << setw(45) << left << User.UserPermisions;
}

void ShowAllUsersScreen() {

    vector <sUser> vUsers = LoadDataUsersFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n\n";

    cout << "| " << left << setw(20) << "User Nmae";
    cout << "| " << left << setw(20) << "Password";
    cout << "| " << left << setw(45) << "Permisions";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n\n";

    for (sUser& User : vUsers) {

        PrintUserRecordLine(User);
        cout << endl;
    }

    cout << "\_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void PerformManageUsersOption(enManageUsers ManageUsersOption, sUser User)
{
    switch (ManageUsersOption)
    {
    case enManageUsers::eListUsers:

        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersScreen(User);
        break;

    case enManageUsers::eAddUser:
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersScreen(User);
        break;

    case enManageUsers::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen(User);
        GoBackToManageUsersScreen(User);
        break;

    case enManageUsers::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersScreen(User);
        break;

    case enManageUsers::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersScreen(User);
        break;

    case enManageUsers::eMainMenu:
        system("cls");
        ShowMainMenue(User);
        break;
    }

}

short ReadManageUsersOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ManageUsersScreen(sUser User) {

    if (CheckPermisions(User, enPermisionsValue::manageusers)) {

        system("cls");
        cout << "===========================================\n";
        cout << "\tManage Users Menu Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] List Users.\n";
        cout << "\t[2] Add New User.\n";
        cout << "\t[3] Delete Usert.\n";
        cout << "\t[4] Update User.\n";
        cout << "\t[5] Find User.\n";
        cout << "\t[6] Main Menu .\n";
        cout << "===========================================\n";
        PerformManageUsersOption((enManageUsers)ReadManageUsersOption(), User);
    }
    else
        NoAccessScreen();
}

void HeaderLogin() {

    cout << "-----------------------------\n";
    cout << "\tLogin Screen\n";
    cout << "-----------------------------\n";
}

void Login() {

    HeaderLogin();

    vector<sUser> vUsers = LoadDataUsersFromFile(UsersFileName);

    sUser User;
    cout << "Enter Username ? ";
    getline(cin >> ws, User.UserName);

    cout << "Enter Password ? ";
    getline(cin >> ws, User.UserPassword);

    while(!FindUserByUserNameAndPassword(User.UserName, User.UserPassword, vUsers, User)) {

        system("cls");
        HeaderLogin();
        cout << "Invalid Username/Password!\n";
        cout << "Enter username ? ";
        getline(cin >> ws, User.UserName);
        cout << "Enter Password ? ";
        getline(cin >> ws, User.UserPassword);
    }

    ShowMainMenue(User);
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

bool UserExistsByUserName(string UserNaem, string FileName)
{

    vector <sUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {

            User = ConvertLineToRecordUsers(Line);
            if (User.UserName == UserNaem)
            {
                MyFile.close();
                return true;
            }


            vUsers.push_back(User);
        }

        MyFile.close();

    }

    return false;


}

sUser ReadNewUser() {

    sUser User;

    cout << "Enter User Name ? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, User.UserName);

    while (UserExistsByUserName(User.UserName, UsersFileName))
    {
        cout << "\nUser with [" << User.UserName << "] already exists, Enter another User Name ? ";
        getline(cin >> ws, User.UserName);
    }


    cout << "Enter Password ? ";
    getline(cin, User.UserPassword);

    GivePermisions(User);

    return User;
}

void AddNewUser() {

    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLine(User));
}

void AddNewUsers() {

    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Users :\n\n";

        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more Users ? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewUsersScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    AddNewUsers();
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption, sUser User)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen(User);
        GoBackToMainMenue(User);
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen(User);
        GoBackToMainMenue(User);
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen(User);
        GoBackToMainMenue(User);
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen(User);
        GoBackToMainMenue(User);
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen(User);
        GoBackToMainMenue(User);
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        ShowTransactionsMenue(User);
        GoBackToMainMenue(User);
        break;

    case enMainMenueOptions::eManageUsers:
        system("cls");
        ManageUsersScreen(User);
        GoBackToMainMenue(User);
        break;

    case enMainMenueOptions::eLogout:
        system("cls");
        Login();
        break;
    }

}

void ShowMainMenue(sUser User)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption(), User);
}

int main() {

    Login();
    system("pause>0");
    return 0;
}