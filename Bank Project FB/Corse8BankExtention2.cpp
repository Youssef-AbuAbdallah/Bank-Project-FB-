# include <iostream>
# include <fstream>
# include <iomanip>
# include <algorithm>
# include <string>
# include <cmath>
# include <vector>

using namespace std;

const string red = "\033[31m";
const string green = "\033[32m";
const string resetDefault = "\033[0m";

struct stClient {
	string AccountNumber, PinCode, PhoneNumber, Name;
	double Balance = 0;
	bool MarkForDelete = false;
};

struct stUser {
	string UserName;
	string Password;
	int Permission=0;
	bool MarkForDelete = false;
};

stUser CurrentUser;

const string UsersFileName = "Users.txt";
const string ClientsFileName = "Clients.txt";




void ShowManageUsersMenue();
void Login();
void ShowTransactionsMenue();
void ShowMainMenue();
vector <stClient> LoadAllClientsFromAFile(string FileName);
vector<stUser> LoadAllUsersFromAFile(string FileName);




string ReadString(string Message)
{
	string Word;
	cout << Message;
	cin >> Word;
	return Word;
}

enum enMainMenueOptions { ShowClientsList = 1, AddNewClients = 2, DeleteClient = 3,
	UpdateClientInfo = 4, FindClient = 5, Transactions = 6,
	ManageUsers = 7 ,LogOut = 8 };

enum enTransactionsOptions { Deposite = 1, Withdraw = 2, TotalBalances = 3, MainMenue = 4 };

enum enManageUsersOprions { enListUsers = 1 , enAddNewUser = 2 , enDeleteUser = 3 , enUpdateUser = 4 , enFindUser = 5 , enBackToMainMenue = 6 };

enum enMainMenuePermissions {eAll = -1 , pShowClientsList = 1 , 
	pAddNewClients = 2, pDeleteClient = 4,
	pUpdateClientInfo = 8, pFindClient = 16, pTransactions = 32,
	pManageUsers = 64 };


void AccessDeniedMessage()
{
	system("cls");
	cout << red << "\n====================================================\n";
	cout << resetDefault << "Access Denied , \n You don't have permission to do this , \n Plese contact your admin " << endl;
	cout << red << "\n====================================================\n" << resetDefault << endl;;

}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
	if (Permission == enMainMenuePermissions::eAll)
	{
		return true;
	}
	else if ((CurrentUser.Permission & Permission) == Permission)
	{
		return true;
	}
	else
	{
		return false;
	}
}


vector <string> SplitString(string Line, string Delimiter = "#//#")
{
	string Word = "";
	int pos = 0;
	vector <string> vecString;

	while ((pos = Line.find(Delimiter)) != std::string::npos)
	{
		Word = Line.substr(0, pos);
		if (!Word.empty())
		{
			vecString.push_back(Word);
		}
		Line.erase(0, pos + Delimiter.size());
	}
	if (Line != "")
	{
		vecString.push_back(Line);
	}
	return vecString;
}

stClient ConvertLineToClientRecord(string Line)
{
	stClient Client;
	vector <string> vecString = SplitString(Line);
	Client.AccountNumber = vecString[0];
	Client.PinCode = vecString[1];
	Client.Name = vecString[2];
	Client.PhoneNumber = vecString[3];
	Client.Balance = stod(vecString[4]);
	return Client;
}


string ConvertRecordToString(stClient Client, string delim = "#//#")
{
	string Line;
	Line += Client.AccountNumber + delim;
	Line += Client.PinCode + delim;
	Line += Client.Name + delim;
	Line += Client.PhoneNumber + delim;
	Line += to_string(Client.Balance);

	return Line;
}


bool ClientExsistsByAccountNumber(string AccountNumber, string ClientsFileName)
{
	vector <stClient> vAllClients;
	fstream MyFile;
	MyFile.open(ClientsFileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToClientRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vAllClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vAllClients, stClient& Client)
{
	for (stClient& C : vAllClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

stClient ReadNewClient()
{
	stClient Client;
	cout << "Please enter Account Number : ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExsistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "Client with ( " << Client.AccountNumber << " ) is already exists Please enter Account Number : ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Please enter Account Pin Code : ";
	getline(cin >> ws, Client.PinCode);


	cout << "Please enter Account Name : ";
	getline(cin >> ws, Client.Name);


	cout << "Please enter Account Phone : ";
	getline(cin >> ws, Client.PhoneNumber);


	cout << "Please enter Balance : ";
	cin >> Client.Balance;

	return Client;

}





bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vAllClients)
{
	for (stClient& Client : vAllClients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			Client.MarkForDelete = true;
			return true;
		}
	}
	return false;
}





short ReadMenuOption(string Message , int MaximumOptionNumber)
{
	short OptionNumber;
	do {
		cout << Message;
		cin >> OptionNumber;
	} while (OptionNumber > MaximumOptionNumber || OptionNumber < 1);
	return OptionNumber;
}



void PrintClientRecord(stClient Client)
{
	cout << "| " << setw(20) << left << Client.AccountNumber << " ";
	cout << "| " << setw(10) << left << Client.PinCode << " ";
	cout << "| " << setw(30) << left << Client.Name << " ";
	cout << "| " << setw(15) << left << Client.PhoneNumber << " ";
	cout << "| " << setw(10) << left << Client.Balance << endl;

}

void PressAnyKeyToGoBack()
{
	cout << "Press any key to go back to main menue .... ";
	system("pause>0");
	ShowMainMenue();
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe following are client's info : " << endl;
	cout << "------------------------------------------------------\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Pin Code       : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone Number   : " << Client.PhoneNumber << endl;
	cout << "Balance        : " << Client.Balance << endl;
	cout << "------------------------------------------------------\n";
}



vector <stClient> LoadAllClientsFromAFile(string FileName)
{
	vector <stClient> vAllClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line = "";
		stClient Client;
		while (getline(MyFile, Line))
		{
			if (!Line.empty())
			{
				Client = ConvertLineToClientRecord(Line);
				vAllClients.push_back(Client);
			}
		}
		MyFile.close();
	}
	return vAllClients;
}

void ShowAllClientsList()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pShowClientsList))
	{
		AccessDeniedMessage();
		return;
	}

	system("cls");
	vector <stClient> vAllClients = LoadAllClientsFromAFile(ClientsFileName);
	cout << " \t\tClients list ( " << vAllClients.size() << " ) Client (s) " << endl;
	cout << "---------------------------------------------------------------------------------------------------\n";
	cout << "| " << setw(20) << left << "Account Number" << " ";
	cout << "| " << setw(10) << left << "Pin Code" << " ";
	cout << "| " << setw(30) << left << "Client Name" << " ";
	cout << "| " << setw(15) << left << "Phone" << " ";
	cout << "| " << setw(10) << left << "Balance" << endl;
	cout << "---------------------------------------------------------------------------------------------------\n";

	if (vAllClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Avaliable in the system! ";
	}
	else
	{
		for (stClient& Client : vAllClients)
		{
			PrintClientRecord(Client);
		}
	}

	cout << "---------------------------------------------------------------------------------------------------\n";
}

void AddDataLineToFile(string FileName, string Line)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << Line << endl;
		MyFile.close();
	}

}

void AddNewClientToFile()
{
	stClient Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToString(Client));
}

void PrintAddClientsScreen()
{
	system("cls");
	cout << green<<"============================================================\n";
	cout <<resetDefault<< "                 Adding New Client                            \n";
	cout << green<<"============================================================\n";
	cout <<resetDefault<< "Add new client \n\n";

}


void ShowFindClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
	{
		AccessDeniedMessage();
		return;
	}
	system("cls");
	cout << green<<"============================================================\n";
	cout <<resetDefault<< "                 Finding Client                            \n";
	cout << green<<"============================================================\n"<<resetDefault;

	vector <stClient> vAllClients = LoadAllClientsFromAFile(ClientsFileName);
	stClient Client;
	string AccountNumber = ReadString("Please enter account number : ");
	if (FindClientByAccountNumber(AccountNumber, vAllClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with account number ( " << AccountNumber << " ) is not found ! " << endl;
	}

}

void AddClients()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClients))
	{
		AccessDeniedMessage();
		return;
	}
	char Question = 'y';
	do
	{
		PrintAddClientsScreen();
		AddNewClientToFile();

		cout << "Client added successfully , Do you want to add more clients [ Y/N ] ? : ";
		cin >> Question;
	} while (Question == 'y' || Question == 'Y');
}

vector <stClient> SaveClientDataToFile(string FileName, vector <stClient> vAllClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;
		for (stClient Client : vAllClients)
		{
			if (Client.MarkForDelete == false)
			{
				Line = ConvertRecordToString(Client);
				MyFile << Line << endl;
			}
		}
		MyFile.close();
	}
	return vAllClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vAllClients)
{

	char Answer = 'n';
	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vAllClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nDo you want to delete this client [y/n] ? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vAllClients);
			SaveClientDataToFile(ClientsFileName, vAllClients);

			cout << "\nClient deleted successfully" << endl;
		}
		vAllClients = LoadAllClientsFromAFile(ClientsFileName);
		return true;
	}
	else
	{
		cout << "\nClient with account number ( " << AccountNumber << " ) is not exist" << endl;
		return false;
	}

}

void ShowDeleteScreen() {
	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
	{
		AccessDeniedMessage();
		return;
	}
	system("cls");
	cout << green<<"===================================\n";
	cout <<resetDefault<< "       Delete Client          \n";
	cout << green<< "===================================\n"<<resetDefault;

	vector <stClient> vAllClients = LoadAllClientsFromAFile(ClientsFileName);
	string AccountNumber = ReadString("Please enter account number : ");

	DeleteClientByAccountNumber(AccountNumber, vAllClients);

}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;
	Client.AccountNumber = AccountNumber;

	cout << "Please enter Account Pin Code : ";
	getline(cin >> ws, Client.PinCode);


	cout << "Please enter Account Name : ";
	getline(cin >> ws, Client.Name);


	cout << "Please enter Account Phone : ";
	getline(cin >> ws, Client.PhoneNumber);


	cout << "Please enter Balance : ";
	cin >> Client.Balance;

	return Client;

}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vAllClients)
{
	stClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vAllClients, Client))
	{
		PrintClientCard(Client);
		cout << "Are you sure you want to update this Client [Y/N] : ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			for (stClient& C : vAllClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientDataToFile(ClientsFileName, vAllClients);
			cout << "\nClient updated successfully" << endl;
			return true;
		}
	}
	else
	{
		cout << "\nClient with account number ( " << AccountNumber << " ) is not exist" << endl;
		return false;
	}

}

void ShowUpdateScreen() {
	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClientInfo))
	{
		AccessDeniedMessage();
		return;
	}
	system("cls");
	cout <<green<< "===================================\n";
	cout <<resetDefault<< "       Update Client          \n";
	cout <<green<<"===================================\n"<<resetDefault;

	vector<stClient> vAllClients = LoadAllClientsFromAFile(ClientsFileName);
	string AccountNumber = ReadString("Please enter account number : ");

	UpdateClientByAccountNumber(AccountNumber, vAllClients);
}

double ReadAmountOfTransaction(string Message)
{
	double Amount;
	cout << Message;
	cin >> Amount;
	return Amount;
}

void PressAnyKeyToGoBackTransactionsMenue()
{
	cout << "\nPress any key to go back to transactions menue .... ";
	system("pause>0");
	ShowTransactionsMenue();
}

bool DepositeBalanceByAccountNumber(string AccountNumber, double Amount, vector<stClient>& vAllClients)
{
	char Answer = 'n';
	cout << "\nAre you sure you to confirm this transaction [y/n] ?  ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : vAllClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.Balance += Amount;
				SaveClientDataToFile(ClientsFileName, vAllClients);
				cout << "\nTransaction is done successfully and the new balance is " << C.Balance << "\n\n" << endl;
				return true;
			}
		}
	}
	return false;
}

void ShowDepositeScreen()
{

	system("cls");
	cout << "===================================\n";
	cout << "    Deposite Transaction Screen          \n";
	cout << "===================================\n";


	stClient Client;
	string AccountNumber = ReadString("Please enter account Number : ");
	vector<stClient> vAllClients = LoadAllClientsFromAFile(ClientsFileName);


	while (!FindClientByAccountNumber(AccountNumber, vAllClients, Client))
	{
		cout << "\nClient with account number ( " << AccountNumber << " ) is not found " << endl;
		AccountNumber = ReadString("Please enter account Number : ");
	}

	PrintClientCard(Client);
	double AmountOfDeposite = ReadAmountOfTransaction("Please enter the amount of deposite : ");

	//////////////////////////////////////////////////////////////////////////////////////

	DepositeBalanceByAccountNumber(AccountNumber, AmountOfDeposite, vAllClients);

}




void ShowWithdrawScreen()
{
	system("cls");
	cout << "===================================\n";
	cout << "    Withdraw Transaction Screen          \n";
	cout << "===================================\n";

	char Answer = 'n';
	stClient Client;
	string AccountNumber = ReadString("Please enter account Number : ");
	vector<stClient> vAllClients = LoadAllClientsFromAFile(ClientsFileName);


	while (!FindClientByAccountNumber(AccountNumber, vAllClients, Client))
	{
		cout << "\nClient with account number ( " << AccountNumber << " ) is not found " << endl;
		AccountNumber = ReadString("Please enter account Number : ");
	}
	PrintClientCard(Client);
	double AmountOfWithdraw = ReadAmountOfTransaction("Please enter the amount of withdraw : ");

	// Validate that the amount of withdraw doesn't exceed the balance 
	while (AmountOfWithdraw > Client.Balance)
	{
		cout << "\nAmount exceeds the balance , you can withdraw up to : " << Client.Balance << " only" << endl;
		AmountOfWithdraw = ReadAmountOfTransaction("Please enter another amount of withdraw : ");
	}

	DepositeBalanceByAccountNumber(AccountNumber, AmountOfWithdraw * -1, vAllClients);

}




void PrintClientRecordTransactionMenue(stClient Client) {
	cout << "| " << setw(20) << left << Client.AccountNumber << " ";
	cout << "| " << setw(30) << left << Client.Name << " ";
	cout << "| " << setw(10) << left << Client.Balance << endl;
}

void ShowAllClientsBalances()
{
	system("cls");
	vector <stClient> vAllClients = LoadAllClientsFromAFile(ClientsFileName);
	cout << " \t\tClients list ( " << vAllClients.size() << " ) Client (s) " << endl;
	cout << "---------------------------------------------------------------------------------------------------\n";
	cout << "| " << setw(20) << left << "Account Number" << " ";
	cout << "| " << setw(30) << left << "Client Name" << " ";
	cout << "| " << setw(10) << left << "Balance" << endl;
	cout << "---------------------------------------------------------------------------------------------------\n";
	double SumOfTotaBalances = 0;

	if (vAllClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Avaliable in the system! ";
	}
	else
	{
		for (stClient& Client : vAllClients)
		{
			PrintClientRecordTransactionMenue(Client);
			SumOfTotaBalances += Client.Balance;
		}
	}

	cout << "---------------------------------------------------------------------------------------------------\n";
	cout << " \t\t\t TotalBalances = " << SumOfTotaBalances << endl << endl;
}

void PerformTransactionsMenueOption(enTransactionsOptions Option)
{
	switch (Option)
	{
	case enTransactionsOptions::Deposite:
	{
		ShowDepositeScreen();
		PressAnyKeyToGoBackTransactionsMenue();
		break;
	}

	case enTransactionsOptions::Withdraw:
	{
		ShowWithdrawScreen();
		PressAnyKeyToGoBackTransactionsMenue();
		break;
	}

	case enTransactionsOptions::TotalBalances:
	{
		ShowAllClientsBalances();
		PressAnyKeyToGoBackTransactionsMenue();
		break;
	}

	case enTransactionsOptions::MainMenue:
	{
		ShowMainMenue();
		break;
	}

	}
}

void ShowTransactionsMenue()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pTransactions))
	{
		AccessDeniedMessage();
		PressAnyKeyToGoBack();
		return;
	}
	system("cls");
	cout <<green<< "============================================================\n";
	cout <<resetDefault<< "                 Transactions Menue screen         \n";
	cout <<green<< "============================================================\n";

	cout << " \t[1] Deposite.\n";
	cout << " \t[2] WithDraw.\n";
	cout << " \t[3] TotalBalances.\n";
	cout << " \t[4] Main Menue.\n";

	cout << green<<"============================================================\n"<<resetDefault;

	PerformTransactionsMenueOption((enTransactionsOptions)ReadMenuOption("Choose what do you want to do [ 1 to 4 ] ? ",4));
}


void PressAnyKeyToGoBackToManageUsersMenue()
{
	cout << "\nPress any key to go back to Manage Users menue .... ";
	system("pause>0");
	ShowManageUsersMenue();
}


void PrintUserRecord(stUser User)
{
	cout << "|" << setw(20) << left << User.UserName << "|";
	cout << setw(20) << left << User.Password << "|";
	cout << setw(20) << left << User.Permission <<  endl;

}



void ShowAllUsersList()
{
	system("cls");
	vector <stUser> vAllUsers = LoadAllUsersFromAFile(UsersFileName);

	cout << " \t\tUsers list ( " << vAllUsers.size() << " ) User(s) " << endl;
	cout << "---------------------------------------------------------------------------------------------------\n";
	cout << "|" << setw(20) << left << "UserName" << "|";
	cout  << setw(20) << left << "Password" << "|";
	cout  << setw(20) << left << "Permissions" << endl;
	cout << "---------------------------------------------------------------------------------------------------\n";

	if (vAllUsers.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Avaliable in the system! ";
	}
	else
	{
		for (stUser& User : vAllUsers)
		{
			PrintUserRecord(User);
		}
	}

	cout << "---------------------------------------------------------------------------------------------------\n";

}

bool FindUserByUserName(string UserName, string FileName,  stUser& User)
{
	vector<stUser> vAllUsers = LoadAllUsersFromAFile(FileName);

	for (stUser U : vAllUsers)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}
	}
	return false;
}

int ReadPermissionsToSet()
{
	int Permissions = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access to this user ? [y/n] : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		return -1;
	}

	
		cout << "\nDo you want to give full access to Show Clients List ? [y/n] : ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			Permissions += enMainMenuePermissions::pShowClientsList;
		}

		cout << "\nDo you want to give full access to Add New Client ? [y/n] : ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			Permissions += enMainMenuePermissions::pAddNewClients;
		}

		cout << "\nDo you want to give full access to Delete Client ? [y/n] : ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			Permissions += enMainMenuePermissions::pDeleteClient;
		}

		cout << "\nDo you want to give full access to Update Client ? [y/n] : ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			Permissions += enMainMenuePermissions::pUpdateClientInfo;
		}

		cout << "\nDo you want to give full access to Find Client  ? [y/n] : ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			Permissions += enMainMenuePermissions::pFindClient;
		}

		cout << "\nDo you want to give full access to Transactions menue  ? [y/n] : ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			Permissions += enMainMenuePermissions::pTransactions;
		}

		cout << "\nDo you want to give full access to Manage Users menue  ? [y/n] : ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			Permissions += enMainMenuePermissions::pManageUsers;
		}

	return Permissions;

}

stUser ReadNewUser()
{
	stUser User;
	cout << "Please enter UserName : ";
	getline(cin >> ws, User.UserName);

	while (FindUserByUserName(User.UserName, UsersFileName, User))
	{
		cout << "\nUser with UserName [ " << User.UserName << " ] is already exsits , try another one  !  " ;
		cout << "\nPlease enter UserName : ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Please enter password : ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermissionsToSet();

	return User;
}

string ConvertUserRecordToLine(stUser User , string Seperator = "#//#")
{
	string Line = "";
	Line += User.UserName + Seperator;
	Line += User.Password + Seperator;
	Line += to_string(User.Permission);

	return Line;
}

void AddNewUser()
{
	stUser User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
	char AddMore = 'Y';
	do
	{
		cout << "Add new users " << endl;

		AddNewUser();

		cout << "\nUser added successfully , Do you want to add more ? [y/n] : ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

void ShowAddNewUserScreen()
{
	system("cls");

	cout << "============================================================\n";
	cout << "                 Adding New Users :         \n";
	cout << "============================================================\n";

	AddNewUsers();

}

void PrintUserCard(stUser User)
{
	cout << "\nThe following are User's info : " << endl;
	cout << "------------------------------------------------------\n";
	cout << "User Name           : " << User.UserName << endl;
	cout << "Password            : " << User.Password << endl;
	cout << "Permission          : " << User.Permission << endl;
	cout << "------------------------------------------------------\n";

}


bool MarkUserForDeletByUserName(string UserName, vector<stUser>& vAllUsers)
{
	for (stUser& User : vAllUsers)
	{
		if (User.UserName == UserName)
		{
			User.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector<stUser> SaveUsersDataToFile(string FileName, vector<stUser> vAllUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;
		for (stUser User : vAllUsers)
		{
			if (User.MarkForDelete == false)
			{
				Line = ConvertUserRecordToLine(User);
				MyFile << Line << endl;
			}
		}
		MyFile.close();
	}
	return vAllUsers;
}

bool DeleteUserByUserName(string UserName, vector <stUser>& vAllUsers)
{
	char Answer = 'n';
	stUser User;

	if (FindUserByUserName(UserName, UsersFileName,  User))
	{
		if (User.UserName == "Admin")
		{
			cout << red << "\n You can't delete the Admin " << resetDefault << endl;
			return false;
		}

		PrintUserCard(User);

		cout << "\nDo you want to delete this client [y/n] ? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkUserForDeletByUserName(UserName, vAllUsers);
			SaveUsersDataToFile(UsersFileName, vAllUsers);

			cout << "\nUser deleted successfully" << endl;
		}
		vAllUsers = LoadAllUsersFromAFile(UsersFileName);
		return true;
	}
	else
	{
		cout << "\nUser with UserName  ( " << UserName << " ) is not exist" << endl;
		return false;
	}

}

void ShowDeleteUserScreen()
{
	system("cls");
	cout << "===================================\n";
	cout << "       Delete User          \n";
	cout << "===================================\n";

	vector <stUser> vAllUSers = LoadAllUsersFromAFile(UsersFileName);
	string UserName = ReadString("Please enter UserName : ");

	DeleteUserByUserName(UserName, vAllUSers);

}

void FindUserScreen()
{
	system("cls");
	cout << "===================================\n";
	cout << "         Find User          \n";
	cout << "===================================\n";
	stUser User;
	string UserName = ReadString("Please enter UserName : ");
	if (FindUserByUserName(UserName, UsersFileName, User))
	{
		PrintUserCard(User);
	}
	else
	{
		cout << "\nUser with UserName [ " << UserName << " ] is not found ! " << endl;
	}
}

stUser ChangeUserRecord(string UserName)
{
	stUser User;
	User.UserName = UserName;

	cout << "Please enter Password : ";
	getline(cin >> ws, User.Password);


	User.Permission = ReadPermissionsToSet();


	return User;
}

bool UpdateUserByUserName(string UserName, vector<stUser>& vAllUsers)
{
	stUser User;
	char Answer = 'n';
	if (FindUserByUserName(UserName,UsersFileName,User))
	{
		PrintUserCard(User);
		cout << "Are you sure you want to update this User [Y/N] : ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			for (stUser& U : vAllUsers)
			{
				if (U.UserName == UserName)
				{
					U = ChangeUserRecord(UserName);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName ,vAllUsers);
			cout << "\nUser updated successfully" << endl;
			return true;
		}
	}
	else
	{
		cout << "\nUser with UserName ( " << UserName << " ) is not exist" << endl;
		return false;
	}

}


void ShowUpdateUserScreen()
{
	system("cls");
	cout << "===================================\n";
	cout << "       Update User          \n";
	cout << "===================================\n";

	vector<stUser> vAllUsers = LoadAllUsersFromAFile(UsersFileName);
	string UserName = ReadString("Please enter UserName : ");

	UpdateUserByUserName(UserName, vAllUsers);
}

void PerformManageUsersOption(enManageUsersOprions Option)
{
	switch (Option)
	{
	case enManageUsersOprions::enListUsers :
	{
		ShowAllUsersList();
		PressAnyKeyToGoBackToManageUsersMenue();
		break;
	}

	case enManageUsersOprions::enAddNewUser :
	{
		ShowAddNewUserScreen();
		PressAnyKeyToGoBackToManageUsersMenue();
		break;
	}

	case enManageUsersOprions::enDeleteUser :
	{
		ShowDeleteUserScreen();
		PressAnyKeyToGoBackToManageUsersMenue();
		break;
	}

	case enManageUsersOprions::enFindUser:
	{
		FindUserScreen();
		PressAnyKeyToGoBackToManageUsersMenue();
		break;
	}
	case enManageUsersOprions::enUpdateUser :
	{
		ShowUpdateUserScreen();
		PressAnyKeyToGoBackToManageUsersMenue();
		break;
	}

	case enManageUsersOprions::enBackToMainMenue: {
		ShowMainMenue();
	}

	}

}



void ShowManageUsersMenue()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
	{
		AccessDeniedMessage();
		PressAnyKeyToGoBack();
		return;
	}

	system("cls");
	cout <<green<< "============================================================\n";
	cout <<resetDefault<< "                 Manage Users Menue screen         \n";
	cout << green<<"============================================================\n"<<resetDefault;

	cout << " \t[1] List Users.\n";
	cout << " \t[2] Add New User.\n";
	cout << " \t[3] Delete User.\n";
	cout << " \t[4] Update User.\n";
	cout << " \t[5] Find User.\n";
	cout << " \t[6] Main Menue.\n";

	cout << green << "============================================================\n" << resetDefault;

	PerformManageUsersOption((enManageUsersOprions)ReadMenuOption("\nChoose what do you want to do [1/6] : ", 6));
}

void PerformMainMenueOption(enMainMenueOptions Option)
{

	switch (Option) {

	case(enMainMenueOptions::ShowClientsList): {
		ShowAllClientsList();
		PressAnyKeyToGoBack();
		break;
	}
	case (enMainMenueOptions::AddNewClients): {
		AddClients();
		PressAnyKeyToGoBack();
		break;
	}
	case (enMainMenueOptions::DeleteClient): {
		ShowDeleteScreen();
		PressAnyKeyToGoBack();
		break;
	}
	case (enMainMenueOptions::UpdateClientInfo): {
		ShowUpdateScreen();
		PressAnyKeyToGoBack();
		break;
	}
	case (enMainMenueOptions::FindClient): {
		system("cls");
		ShowFindClientScreen();
		PressAnyKeyToGoBack();
	}

	case enMainMenueOptions::Transactions:
	{
		ShowTransactionsMenue();
		break;
	}

	case enMainMenueOptions::ManageUsers:
	{
		system("cls");
		ShowManageUsersMenue();
		break;
	}

	case enMainMenueOptions::LogOut:
	{
		Login();
		system("Pause>0");
	}

	}
}


void ShowMainMenue()
{
	system("cls");
	cout << "============================================================\n";
	cout << "                 Main Menu screen                            \n";
	cout << "============================================================\n";

	cout << " \t[1] Show Clients List. \n";
	cout << " \t[2] Add New Client. \n";
	cout << " \t[3] Delete Client. \n";
	cout << " \t[4] Update Client Info. \n";
	cout << " \t[5] Find Client. \n";
	cout << " \t[6] Transactions. \n";
	cout << " \t[7] Manage Users. \n";
	cout << " \t[8] Logout. \n";
	cout << "============================================================\n";

	PerformMainMenueOption((enMainMenueOptions)ReadMenuOption("Choose what do you want to do [ 1 to 8 ] ? ",8));
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


stUser ConvertLineToUserRecord(string Line)
{
	stUser User;
	vector<string> UserInfo = SplitString(Line);
	
	User.UserName = UserInfo[0];
	User.Password = UserInfo.at(1);
	User.Permission = stoi(UserInfo[2]);

	return User;
}


vector <stUser> LoadAllUsersFromAFile(string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	vector <stUser> vAllUsers;
	if (MyFile.is_open())
	{
		string Line;
		stUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertLineToUserRecord(Line);
			vAllUsers.push_back(User);
		}

		MyFile.close();
	}
	return vAllUsers;
}

bool FindUserByUserNameAndPassword(string UserName, string Password, stUser& User)
{
	vector <stUser>vAllUsers;

	vAllUsers = LoadAllUsersFromAFile(UsersFileName);

	for (stUser U : vAllUsers)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}


bool LoadUserInfo(string UserName , string Password)
{
	if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Login()
{
	bool LoginFailed = false;
	string UserName, Password;

	do {
		system("cls");
		cout << "========================================================\n";
		cout << "                          Login ";
		cout << "\n========================================================\n";

		if (LoginFailed)
		{
			cout << "Invalid UserName or Password , Please try again " << endl;
		}
		cout << "\nPlease enter the UserName : ";
		getline(cin >> ws, UserName);
		cout << "Please enter the Password : ";
		getline(cin >> ws, Password);

		LoginFailed = !LoadUserInfo(UserName, Password);


	} while (LoginFailed);

	ShowMainMenue();

}






int main()
{
	Login();

}
