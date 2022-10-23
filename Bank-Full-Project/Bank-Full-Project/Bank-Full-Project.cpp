#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>
using namespace std;

const string filename = "MyClients.txt";

const string usersFile = "SystemUsers.txt";

struct sClient {
	string accountNumber;
	string pincode;
	string name;
	string phone;
	double accountBalance;
	bool markForDeletion = false;
};

struct sUser {
	string username;
	string password;
	string permission;
	bool markToDelete = false;
};

enum enOption {
	ShowClientList = 1 ,
	AddNewClient = 2 ,
	DeleteClient = 3 , 
	UpdateClientInfo = 4 , 
	FindClient = 5 , 
	Transactions = 6 , 
	ManageUsers = 7 , 
	Logout = 8
};

enum enTransaction {
	Deposit = 1 , 
	Withdraw = 2 , 
	TotalBalances = 3 , 
	MainMenu = 4
};

enum enManageOperation {
	ListUsers = 1 , 
	AddNewUser = 2 , 
	DeleteUser = 3 , 
	UpdateUser = 4 , 
	FindUser = 5 , 
	Menu = 6
};

//Dec.

void clearScreen(void);
void goBackToMainMenu(sUser);

vector<sClient> loadClientsFromFile(string,string);
sClient convertLineToClientRecord(string,string);
vector<string> getStringTokens(string,string);

void printMainMenuScreen(void);
void printScreenTitleHeader(string);

short readUserChoice(void);
bool isValidChoice(short);



void performOperation(enOption,sUser);

void showClientList(void);
void printClientListTableHeader(short);
void printClientRowInfo(sClient);
void printTableFooter(void);


void addNewClients(void);
void addNewClient(void);
bool isClientExist(string, string);
string convertClientDataToRecordLine(sClient,string);
void addLineToFile(string, string);
sClient readClient(void);

void deleteClient(void);
bool deleteClientByAccountNumber(string , vector<sClient>&);
bool findClientByAccountNumber(string, vector<sClient>, sClient&);
void printClientDetailsCard(sClient);
bool markClientForDelete(string, vector<sClient>&);
vector<sClient> saveClientDataToFile(string, vector<sClient>);

void updateClientInfo(void);
void updateClient(string, vector<sClient>&, sClient);
sClient readUpdatedClientInfo(string);

void findClient(void);


void transactions(sUser);
void printTransactionsMenuScreen(void);
short readTransactionChoice(void);
bool isValidTransactionChoice(bool);
void performTransaction(enTransaction,sUser);
void goBackToTransactionsMenu(sUser);
void deposit(void);
void withdraw(void);
void updateBalance(vector<sClient>&,string,double,short);

void totalBalances(void);
void printTotalBalancesTableHeader(short);
void printTotalBalancesTableRow(sClient);
double getTotalBalances(vector<sClient>);

void mainMenu(sUser);


void manageUsers(sUser);
void printManageUsersMenu(void);
short readManageUsersChoice(void);
bool isValidManageUsersChoice(short);

void performManageUsersOperation(enManageOperation,sUser);
void goBackToManageUsersMenu(sUser);
void printUsersListTableHeader(short);
void printUserRowInfo(sUser);

void listUsers(void);

void addNewUser(void);
sUser readNewUser(void);
string readUsername(vector<sUser>);
string readPassword(void);
short readPermission(void);
bool isUserNameExist(string, vector<sUser>);
short getPermission(short);

void deleteUser(void);
bool findUserByUsername(string,vector<sUser>,sUser&);
bool markUserForDeletion(string, vector<sUser>&);
void printUserInfoCard(sUser);

void updateUser(void);
void updateUser(sUser, vector<sUser>&);

void logout(void);

void findUser(void);

void runProgram(sUser);


void login(void);
vector<sUser> loadUsersFromFile(string, string);
sUser convertLineToUser(string, string);
string convertUserToLine(sUser,string);
void saveUsersToFile(vector<sUser>, string);
sUser readUser(void);
bool isUserExist(sUser&, vector<sUser>);

bool haveAccess(sUser,enOption);
void printAccessDeniedMessgae(void);


int main() {

	//runProgram();

	login();

	return 0;
}





//Def.

void login() {

	printScreenTitleHeader("Login Screen");

    vector<sUser> vUsers = loadUsersFromFile(usersFile, "#//#");

	sUser enteredUser = readUser();

	while (!isUserExist(enteredUser, vUsers)) {
		cout << "\n****** Invalid User ******" << endl;
		enteredUser = readUser();
	} 

	// pass enetred user to runProgram() to determine permissions.

	runProgram(enteredUser);

}

vector<sUser> loadUsersFromFile(string filename , string separator = "#//#") {

	vector<sUser> vUsers;

	fstream myFile;
	myFile.open(filename, ios::in);

	if (myFile.is_open()) {

		string line = "";
		sUser user;

		while (getline(myFile, line)) {

			user = convertLineToUser(line, separator);
			vUsers.push_back(user);

		}

		myFile.close();

	}
	
	return vUsers;
}

sUser convertLineToUser(string line, string separator = "#//#") {

	vector<string> vString = getStringTokens(line, separator);

	sUser user;
	user.username = vString[0];
	user.password = vString[1];
	user.permission = vString[2];

	return user;
}

sUser readUser() {
	
	string username = "", password = "";

	cout << "Enter Username: ";
	getline(cin>>ws,username);

	cout << "\nEnter Password: ";
	getline(cin>>ws,password);

	return sUser({ username,password,"no-persmission-yet"});
}

bool isUserExist(sUser& enteredUser, vector<sUser> vUsers) {

	for (sUser& user : vUsers) {
		if (user.username == enteredUser.username && user.password == enteredUser.password) {
			enteredUser = user;
			return true;
		}
	}
	return false;
}

void runProgram(sUser user) {
	
	clearScreen();

	printMainMenuScreen();

	performOperation(enOption(readUserChoice()),user);
}

void printMainMenuScreen() {
	printScreenTitleHeader("Main Menu Screen");
	cout << "\t" << "[1] Show Client List." << endl;
	cout << "\t" << "[2] Add New Cleint." << endl;
	cout << "\t" << "[3] Delete Client." << endl;
	cout << "\t" << "[4] Update Client Info." << endl;
	cout << "\t" << "[5] Find Client." << endl;
	cout << "\t" << "[6] Transactions." << endl;
	cout << "\t" << "[7] Manage Users." << endl;
	cout << "\t" << "[8] Logout." << endl;
	cout << "========================================================================" << endl;
}

void printScreenTitleHeader(string title) {
	cout << "========================================================================" << endl;
	cout << "\t\t" << title << endl;
	cout << "========================================================================" << endl;
}

short readUserChoice() {
	short choice = 0;
	cout << "Choose What You Want To Do ? [1 to 8] ? ";
	cin >> choice;

	while (!isValidChoice(choice)) {
		cout << "\nInvalid Input , Choose What You Want To Do ? [1 to 8] ? ";
		cin >> choice;
	}

	return choice;
}

bool isValidChoice(short number) {
	return (number >= 1) ? (number <= 8) : false;
}

void performOperation(enOption opertion , sUser user) {
	
	clearScreen();

	switch(opertion) {
	 
	case enOption::ShowClientList:
	   haveAccess(user, enOption::ShowClientList) ? showClientList() : printAccessDeniedMessgae();
	   break;

	case enOption::AddNewClient:
		haveAccess(user, enOption::ShowClientList) ? addNewClients() : printAccessDeniedMessgae();
		break;

	case enOption::DeleteClient:
		haveAccess(user, enOption::ShowClientList) ? deleteClient() : printAccessDeniedMessgae();
		break;


	case enOption::UpdateClientInfo:
		haveAccess(user, enOption::ShowClientList) ? updateClientInfo() : printAccessDeniedMessgae();
		break;


	case enOption::FindClient:
		haveAccess(user, enOption::ShowClientList) ? findClient() : printAccessDeniedMessgae();
		break;


	case enOption::Transactions:
		haveAccess(user, enOption::ShowClientList) ? transactions(user) : printAccessDeniedMessgae();
		break;

	case enOption::ManageUsers:
		haveAccess(user, enOption::ShowClientList) ? manageUsers(user) : printAccessDeniedMessgae();
		break;

	case enOption::Logout:
		logout();
		break;
	}

	goBackToMainMenu(user);
}

void clearScreen() {
	system("cls");
}

void goBackToMainMenu(sUser user) {
	cout << "\n\nPress Any Key To Contiune...";
	system("pause>0");
	runProgram(user);
}

void showClientList() {
	
	vector<sClient> vClients = loadClientsFromFile(filename,"#//#");

	printClientListTableHeader(vClients.size());

	for (sClient& client : vClients) {
		printClientRowInfo(client);
		cout << endl;
	}

	printTableFooter();
}

void printClientListTableHeader(short number) {

	cout << "\n\n\t\t\tClient List ( " << number << " ) " << "Client(s)" << endl;

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << setw(15) << left << "Acount Number";
	cout << "| " << setw(10) << left << "Pincode";
	cout << "| " << setw(40) << left << "Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void printClientRowInfo(sClient client) {
	cout << "| " << setw(15) << left << client.accountNumber;
	cout << "| " << setw(10) << left << client.pincode;
	cout << "| " << setw(40) << left << client.name;
	cout << "| " << setw(12) << left << client.phone;
	cout << "| " << setw(12) << left << client.accountBalance;
}

void printTableFooter() {
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

vector<sClient> loadClientsFromFile(string filename, string separator = "#//#") {

	vector<sClient> vClients;

	fstream myFile;
	myFile.open(filename, ios::in);

	if (myFile.is_open()) {

		string line;
		sClient client;

		while (getline(myFile, line)) {

			client = convertLineToClientRecord(line,separator);

			vClients.push_back(client);

		}
		// When You Open File , Close It Because it may be opened later.(Without Closing Error Occures)
		myFile.close();
	}
	

	return vClients;
}

sClient convertLineToClientRecord(string line , string separator = "#//#") {

	sClient client;
	vector<string> vString = getStringTokens(line,separator);

	client.accountNumber = vString[0];
	client.pincode = vString[1];
	client.name = vString[2];
	client.phone = vString[3];
	client.accountBalance = stod(vString[4]);

	return client;
}

vector<string> getStringTokens(string s, string separator = "#//#") {

	vector<string> vString;

	short position = 0;

	string token = "";

	while ((position = s.find(separator)) != std::string::npos) {

		token = s.substr(0, position);
		if (token != "") {
			vString.push_back(token);
		}

		s.erase(0, position + separator.length());
	}

	if (s != "") {
		vString.push_back(s);
	}

	return vString;
}

void addNewClients() {
	
	printScreenTitleHeader("Adding New Client");
	cout << "Adding New Client: " << endl;
	
	char addAnotherClient = 'N';

	do {

		addNewClient();
		

		cout << "\n\nDo You Want To Add Another Client ? [ Y | N ] ? ";
		cin >> addAnotherClient;

	} while (toupper(addAnotherClient) == 'Y');

}

void addNewClient() {
	sClient newClient = readClient();
	addLineToFile(convertClientDataToRecordLine(newClient, "#//#"), filename);
	cout << "Client Added Succesfully.." << endl;
}

string convertClientDataToRecordLine(sClient client, string separator = "#//#") {
	string line = "";
	
	line += client.accountNumber + separator;

	line += client.pincode + separator;

	line += client.name += separator;

	line += client.phone += separator;

	line += to_string(client.accountBalance);

	return line;
}

void addLineToFile(string line, string filename) {

	fstream myFile;
	myFile.open(filename, ios::out | ios::app);

	if (myFile.is_open()) {
		myFile << line << endl;
		myFile.close();
	}

}

sClient readClient() {
	sClient client;

	cout << "Enter Account Number: ";
	getline(cin >> ws, client.accountNumber); // Usage of std::ws will extract allthe whitespace character

	while(isClientExist(client.accountNumber,filename)){
		cout << "Client With Account Number [ " << client.accountNumber << " ] Exists , Enter Another Account Number: ";
		getline(cin >> ws, client.accountNumber);
		cout << "\n";
	}

	cout << "\nEnter Pincode: ";
	getline(cin >> ws, client.pincode);

	cout << "\nEnter Name: ";
	getline(cin >> ws, client.name);

	cout << "\nPhone: ";
	getline(cin >> ws, client.phone);

	cout << "\nEnter Balance: ";
	cin >> client.accountBalance;

	return client;
}

bool isClientExist(string accountNumber, string filename) {

	fstream myFile;
	myFile.open(filename, ios::in);

	if (myFile.is_open()) {

		string line = "";
		sClient client;

		while(getline(myFile,line)){

			client = convertLineToClientRecord(line, "#//#");

			if (client.accountNumber == accountNumber) {
				myFile.close();
				return true;
			}
				
		}

		myFile.close();

	}
	return false;
}

void deleteClient() {

	sClient client;
	printScreenTitleHeader("Delete Client Screen");

	string accountNumber = "";
	cout << "Enter Account Number: ";
	cin >> accountNumber;

	vector<sClient> vClients = loadClientsFromFile(filename, "#//#");
	if (findClientByAccountNumber(accountNumber, vClients, client)) {
		
		printClientDetailsCard(client);

		markClientForDelete(accountNumber,vClients);
		
		if (deleteClientByAccountNumber(accountNumber, vClients)) {
			// Refresh Vector.
			vClients = loadClientsFromFile(filename, "#//#");
			cout << "Client Deleted Successfully..." << endl;
		}
	}
	else {
		cout << "Client With Account Number [ " << accountNumber << " ] Does NOT Exist ! " << endl;
	}

}

bool findClientByAccountNumber(string accountNumebr, vector<sClient> vClients, sClient& client1) {

	for (sClient& client : vClients) {
		if (client.accountNumber == accountNumebr) {
			client1 = client;
			return true;
		}
	}
	return false;
}

bool deleteClientByAccountNumber(string accountNumber, vector<sClient>& vClients) {
	char answer = 'N';
	cout << "Are You Sure You Want To Delete This Client ? [ Y | N ] ? ";
	cin >> answer;

	if (toupper(answer) == 'Y') {

		saveClientDataToFile(filename, vClients);

		return true;
	}

	return false;
}

void printClientDetailsCard(sClient client) {
	cout << "\nThe Following Are The Clients Info: " << endl;
	cout << "-----------------------------------" << endl;
	cout << "Account Number :" << client.accountNumber << endl;
	cout << "Pincode        :" << client.pincode << endl;
	cout << "Name           :" << client.name << endl;
	cout << "Phone          :" << client.phone << endl;
	cout << "Balance        :" << client.accountBalance << endl;
	cout << "-----------------------------------" << endl;
}

bool markClientForDelete(string accountNumber, vector<sClient>& vClients) {

	for (sClient& client : vClients) {
		if (client.accountNumber == accountNumber) {
			client.markForDeletion = true;
			return true;
		}
	}
	return false;
}

vector<sClient> saveClientDataToFile(string filename, vector<sClient> vClients) {

	fstream myFile;
	myFile.open(filename, ios::out);

	string line = "";

	if (myFile.is_open()) {

		for (sClient& client : vClients) {

			if (client.markForDeletion == false) {
				line = convertClientDataToRecordLine(client, "#//#");
				myFile << line << endl;
			}

		}

		myFile.close();
	}

	return vClients;
}

void findClient() {

	vector<sClient> vClients = loadClientsFromFile(filename, "#//#");
	sClient client;

	printScreenTitleHeader("Find Client Screen");

	string accountNumber = "";
	cout << "Enter Account Number: ";
	cin >> accountNumber;

	cout << "\n\n";

	if (findClientByAccountNumber(accountNumber,vClients,client)) {
		printClientDetailsCard(client);
	}
	else {
		cout << "Client With Account Number [ " << accountNumber << " ]  Is NOT Found ! " << endl;
	}


}

void updateClientInfo() {

	vector<sClient> vClients = loadClientsFromFile(filename, "#//#");

	sClient client;

	printScreenTitleHeader("Update Client Screen");

	string accountNumber;
	cout << "Enter Account Number ? ";
	getline(cin >> ws, accountNumber);

	if(findClientByAccountNumber(accountNumber,vClients,client)) {

		printClientDetailsCard(client);

		char answer = 'N';
		cout << "Are You Sure You Want To Update This Client Info ? [ Y | N ] ? ";
		cin >> answer;

		if (toupper(answer) == 'Y') {

			sClient newClientInfo = readUpdatedClientInfo(accountNumber);

			updateClient(accountNumber, vClients, newClientInfo);

			// save To File
			saveClientDataToFile(filename, vClients);

			cout << "\n\nClient Updated Successfully.." << endl;
		}

	}
	else {
		cout << "Client Is Not Found ! " << endl;
	}

}

sClient readUpdatedClientInfo(string accountNumber) {

	sClient client;

	client.accountNumber = accountNumber;

	cout << "\nEnter Pincode: ";
	getline(cin >> ws, client.pincode);

	cout << "\nEnter Name: ";
	getline(cin >> ws, client.name);

	cout << "\nEnter Phone: ";
	getline(cin >> ws, client.phone);

	cout << "\nEnter Balance: ";
	cin >> client.accountBalance;

	return client;
}

void updateClient(string accountNumber , vector<sClient>& vClients, sClient newClientInfo) {

	for (sClient& client : vClients) {
		if (accountNumber == client.accountNumber) {
			client = newClientInfo;
			break;
		}
	}

}


void transactions(sUser user) {

	clearScreen();

	printTransactionsMenuScreen();
	
	performTransaction(enTransaction(readTransactionChoice()),user);
	
}

void printTransactionsMenuScreen() {
	printScreenTitleHeader("Transactions Menu Screen");
	cout << "\t" << "[1] Deposit." << endl;
	cout << "\t" << "[2] Withdraw." << endl;
	cout << "\t" << "[3] Total Balances." << endl;
	cout << "\t" << "[4] Main Menu." << endl;
	cout << "========================================================================" << endl;
}

short readTransactionChoice() {
	short number;
	cout << "Choose What You Want To Do ? [ 1 to 4 ] ? ";
	cin >> number;

	while(!isValidTransactionChoice(number)){
		cout << "Invalid Choice , What You Want To Do ? [ 1 to 4 ] ? ";
		cin >> number;
		cout << endl;
	}
	return number;
}

bool isValidTransactionChoice(bool number) {
	return (number >= 1) ? (number <= 4) : false;
}

void performTransaction(enTransaction transaction , sUser user) {

	clearScreen();

	switch(transaction) {
	  
	case enTransaction::Deposit:
		deposit();
		break;

	case enTransaction::Withdraw:
		withdraw();
		break;

	case enTransaction::TotalBalances:
		totalBalances();
		break;

	case enTransaction::MainMenu:
		mainMenu(user);
		break;
	}

	goBackToTransactionsMenu(user);
}

void deposit() {

	printScreenTitleHeader("Deposit Screen");

	vector<sClient> vClients = loadClientsFromFile(filename, "#//#");

	sClient client;

	string accountNumber;
	cout << "Please Enter Account Number ? ";
	getline(cin >> ws, accountNumber);

	while (!isClientExist(accountNumber,filename)) {
		cout << "Client with account Number [ " << accountNumber << " ] Does not exist" << endl;
		cout << "Enter Account Number ? ";
		cin >> accountNumber;
		cout << endl;
	}

	if (findClientByAccountNumber(accountNumber, vClients, client)) {

		printClientDetailsCard(client);

		double depositAmount = 0;
		cout << "Enter Deposit Amount ? ";
		cin >> depositAmount;

		char answer = 'N';
		cout << "Are You Sure You Want To Perform This Transaction ? [ Y | N ] ? ";
		cin >> answer;

		if (toupper(answer) == 'Y') {
			updateBalance(vClients, accountNumber, depositAmount,1);
			saveClientDataToFile(filename, vClients);
			cout << "Deposited Succsessfully... New Balance Is " << client.accountBalance + depositAmount << endl;
		}

	}

}

void withdraw() {
	printScreenTitleHeader("Withdraw Screen");

	vector<sClient> vClients = loadClientsFromFile(filename, "#//#");

	sClient client;

	string accountNumber;
	cout << "Please Enter Account Number ? ";
	getline(cin >> ws, accountNumber);

	while (!isClientExist(accountNumber, filename)) {
		cout << "Client with account Number [ " << accountNumber << " ] Does not exist" << endl;
		cout << "Enter Account Number ? ";
		cin >> accountNumber;
		cout << endl;
	}

	if (findClientByAccountNumber(accountNumber, vClients, client)) {

		printClientDetailsCard(client);

		double depositAmount = 0;
		cout << "Enter Withdraw Amount ? ";
		cin >> depositAmount;

		char answer = 'N';
		cout << "Are You Sure You Want To Perform This Transaction ? [ Y | N ] ? ";
		cin >> answer;

		if (toupper(answer) == 'Y') {
			updateBalance(vClients, accountNumber, depositAmount, -1);
			saveClientDataToFile(filename, vClients);
			cout << "Deposited Succsessfully... New Balance Is " << client.accountBalance - depositAmount << endl;
		}

	}
}

void updateBalance(vector<sClient>& vClients, string accountNumber , double amount , short action) {
	// action = 1 ---> Deposit , action = 2 -----> Withdraw.
	for (sClient& client : vClients) {
		if (accountNumber == client.accountNumber) {
			client.accountBalance += (amount * action);
			break;
		}
	}
}

void totalBalances() {

	vector<sClient> vClients = loadClientsFromFile(filename, "#//#");

	printScreenTitleHeader("Total Balances Screen");

	printTotalBalancesTableHeader(vClients.size());

	for (sClient& client : vClients) {
		printTotalBalancesTableRow(client);
		cout << endl;
	}

	cout << "\n\n\t\tTotal Balnces = " << getTotalBalances(vClients) << endl;

}

void printTotalBalancesTableHeader(short number) {
	cout << "\n\n\t\t\tClient List ( " << number << " ) " << "Client(s)" << endl;

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(40) << left << "Name";
	cout << "| " << setw(12) << left << "Balance";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void printTotalBalancesTableRow(sClient client) {
	cout << "| " << setw(15) << left << client.accountNumber;
	cout << "| " << setw(40) << left << client.name;
	cout << "| " << setw(12) << left << client.accountBalance;
}

double getTotalBalances(vector<sClient> vClients) {
	double totalBalances = 0;

	for (sClient& client : vClients) {
		totalBalances += client.accountBalance;
	}

	return totalBalances;
}

void mainMenu(sUser user) {
	runProgram(user);
}

void goBackToTransactionsMenu(sUser user) {
	cout <<"Press Any Key To Key To Continue... " << endl;
	system("pause>0");
	transactions(user);
}

void manageUsers(sUser user) {

	clearScreen();

	printManageUsersMenu();

	short choice = readManageUsersChoice();

	performManageUsersOperation(enManageOperation(choice),user);
}

short readManageUsersChoice() {
	short choice = 0;
	cout << "Choose What You Want To Do ? [1 to 6] ? ";
	cin >> choice;

	while (!isValidManageUsersChoice(choice)) {
		cout << "\nInvalid Input , Choose What You Want To Do ? [1 to 6] ? ";
		cin >> choice;
	}

	return choice;
}

bool isValidManageUsersChoice(short number) {
	return (number >= 1) ? (number <= 6) : false;

}

void printManageUsersMenu() {
	printScreenTitleHeader("Manage Users Screen");
	cout << "\t" << "[1] List Users." << endl;
	cout << "\t" << "[2] Add New User." << endl;
	cout << "\t" << "[3] Delete User." << endl;
	cout << "\t" << "[4] Update User." << endl;
	cout << "\t" << "[5] Find User." << endl;
	cout << "\t" << "[6] Main Menu." << endl;
	cout << "========================================================================" << endl;
}

void performManageUsersOperation(enManageOperation operation , sUser user) {

	clearScreen();

	switch (operation) {

	case enManageOperation::ListUsers:
		listUsers();
		break;

	case enManageOperation::AddNewUser:
		addNewUser();
		break;

	case enManageOperation::DeleteUser:
		deleteUser();
		break;

	case enManageOperation::UpdateUser:
		updateUser();
		break;


	case enManageOperation::FindUser:
		findUser();
		break;

	case enManageOperation::Menu:
		runProgram(user);
		break;
	}

	goBackToManageUsersMenu(user);
}

void listUsers() {

	vector<sUser> vUsers = loadUsersFromFile(usersFile, "#//#");

	printUsersListTableHeader(vUsers.size());

	for (sUser& user : vUsers) {
		printUserRowInfo(user);
		cout << endl;
	}


}

void printUsersListTableHeader(short number) {
	cout << "\n\n\t\t\"Users List ( " << number << " ) " << "User(s)" << endl;

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << setw(15) << left << "Username";
	cout << "| " << setw(10) << left << "Password";
	cout << "| " << setw(40) << left << "Permission";
	

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void printUserRowInfo(sUser user) {
	cout << "| " << setw(15) << left << user.username;
	cout << "| " << setw(10) << left << user.password;
	cout << "| " << setw(40) << left << user.permission;

}

void addNewUser() {

	vector<sUser> vUsers = loadUsersFromFile(usersFile, "#//#");

	printScreenTitleHeader("Add New User Screen");

	char answer = 'N';

	do {
		sUser user = readNewUser();

		vUsers.push_back(user);

		cout << "\n User Added Successfully..\n";

		cout << "\nDo You Want To Add New User ? [ Y | N ] ? ";
		cin >> answer;

		

	} while(toupper(answer) == 'Y');

	
	saveUsersToFile(vUsers,usersFile);

}

sUser readNewUser() {

	vector<sUser> vUsers = loadUsersFromFile(usersFile, "#//#");

	sUser user;

	user.username = readUsername(vUsers);
	
	user.password = readPassword();

	user.permission = to_string(readPermission());

	
	return user;
	
}

string readUsername(vector<sUser> vUsers) {
	
	string username = "";

	cout << "Enter Username: ";
	cin >> username;

	while (isUserNameExist(username, vUsers)) {
		cout << "\nUsername [ " << username << " ] Already exists , please enter another username: ";
		cin >> username;
	}

	return username;
}

string readPassword() {

	string password = "";

	cout << "\nEnter Password: ";
	cin >> password;

	return password;
}

short readPermission() {

	char answer = 'N';

	cout << "Do You Want To Give This User Full Access ? [ Y | N ] ? ";
	cin >> answer;

	if (toupper(answer) == 'Y') {
		return -1;
	}

	short counter = 0;
	short permissions = 0;
	cout << "\nDo You Want To Give Access To: " << "\n\n";

	cout << "Show Client List ? [ Y | N ] ? ";
	cin >> answer;
	counter += 1;
	permissions += (toupper(answer) == 'Y') ? getPermission(counter) : 0;


	cout << "\n\nAdd New Client ? [ Y | N ] ? ";
	cin >> answer;
	counter += 1;
	permissions += (toupper(answer) == 'Y') ? getPermission(counter) : 0;


	cout << "\n\nDelete Client ? [ Y | N ] ? ";
	cin >> answer;
	counter += 1;
	permissions += (toupper(answer) == 'Y') ? getPermission(counter) : 0;


	cout << "\n\nUpdate Client ? [ Y | N ] ? ";
	cin >> answer;
	counter += 1;
	permissions += (toupper(answer) == 'Y') ? getPermission(counter) : 0;


	cout << "\n\nFind Client ? [ Y | N ] ? ";
	cin >> answer;
	counter += 1;
	permissions += (toupper(answer) == 'Y') ? getPermission(counter) : 0;


	cout << "\n\nTransactions ? [ Y | N ] ? ";
	cin >> answer;
	counter += 1;
	permissions += (toupper(answer) == 'Y') ? getPermission(counter) : 0;


	cout << "\n\nManage Users ? [ Y | N ] ? ";
	cin >> answer;
	counter += 1;
	permissions += (toupper(answer) == 'Y') ? getPermission(counter) : 0;


	return permissions;
}

short getPermission(short number) {
 // faster than writing factorial algorithm.
	short permissions[] = { 0,1,2,4,8,16,32,64,128,256,512,1024 };
	return permissions[number];
}

string convertUserToLine(sUser user , string separator = "#//#") {

	string line = "";

	line += user.username + separator;

	line += user.password + separator;

	line += user.permission;

	return line;
}

void saveUsersToFile(vector<sUser> vUsers , string filename) {

	fstream myFile;
	myFile.open(filename, ios::out);

	if (myFile.is_open()) {

		string line = "";
		
		for (sUser& user : vUsers) {

			if (user.markToDelete == false) {
				line = convertUserToLine(user, "#//#");
				myFile << line << endl;
			}
			
		}

		myFile.close();
	}
	
}

bool isUserNameExist(string username, vector<sUser> vUsers) {

	for (sUser& user : vUsers) {
		if (username == user.username)
			return true;
	}
	return false;
}

void deleteUser() {

	printScreenTitleHeader("Delete User Screen");

	vector<sUser> vUsers = loadUsersFromFile(usersFile, "#//#");

	sUser user;

	string username = "";
	cout << "Enter Username: ";
	cin >> username;

	if (findUserByUsername(username, vUsers, user)) {
		printUserInfoCard(user);

		char answer = 'N';
		cout << "Are You Sure You Want To Delete This User ? [ Y | N ] ? ";
		cin >> answer;

		if (toupper(answer) == 'Y') {
			markUserForDeletion(username, vUsers);
			saveUsersToFile(vUsers, usersFile);
			cout << "\n\nUser Deleted Successfully..." << endl;
		}
	}
	else {
		cout << "User With Username [ " << username << " ]  Is NOT Found ! " << endl;
	}
}

bool findUserByUsername(string username , vector<sUser> vUsers , sUser& userToFind) {

	for (sUser& user : vUsers) {
		if (username == user.username) {
			userToFind = user;
			return true;
		}
	}
	return false;
}

void printUserInfoCard(sUser user) {
	cout << "The Following Are The User Details: " << endl;
	cout << "-----------------------------------" << endl;
	cout << "Username: " << user.username << endl;
	cout << "Password: " << user.password << endl;
	cout << "Permissions: " << user.permission << endl;
	cout << "-----------------------------------" << endl;
}

bool markUserForDeletion(string username, vector<sUser>& vUsers) {

	for (sUser& user : vUsers) {
		if (username == user.username) {
			user.markToDelete = true;
			return true;
		}
	}

	return false;
}

void updateUser() {

	printScreenTitleHeader("Update User Screen");

	vector<sUser> vUsers = loadUsersFromFile(usersFile, "#//#");

	sUser user;

	string username = "";
	cout << "Enter Username: ";
	cin >> username;

	if (findUserByUsername(username, vUsers, user)) {
		printUserInfoCard(user);

		char answer = 'N';
		cout << "Are You Sure You Want To Update This User ? [ Y | N ] ? ";
		cin >> answer;

		if (toupper(answer) == 'Y') {
			
			cout << "\nEnter New Credientials: " << endl;
			user.password = readPassword();
			user.permission = to_string(readPermission());

			updateUser(user,vUsers);

			saveUsersToFile(vUsers, usersFile);


			cout << "\n\nUser Updated Successfully..." << endl;
		}
	}
	else {
		cout << "User With Username [ " << username << " ]  Is NOT Found ! " << endl;
	}
}

void updateUser(sUser updatedUser, vector<sUser>& vUsers) {
	
	for (sUser& user : vUsers) {
		if (user.username == updatedUser.username) {
			user = updatedUser;
			break;
		}
	}
}

void findUser() {

	printScreenTitleHeader("Find User Screen");

	vector<sUser> vUser = loadUsersFromFile(usersFile, "#//#");

	sUser user;

	string username = "";
	cout << "Enter Username: ";
	cin >> username;

	if (findUserByUsername(username,vUser,user)) {

		printUserInfoCard(user);

	}
	else {
		cout << "User With Username [ " << username << " ] Is NOT Found ! " << endl;
	}
}

bool haveAccess(sUser user, enOption option) {

	short permissions[] = { 0,1,2,4,8,16,32,64,128,256,512,1024 };

	short permission = stoi(user.permission);

	short action = getPermission(short(option));

	for (short i = 7 ; i >= 1; i--) {
		
		if (action == permission) {
			return true;
		}
		
		if (permission > permissions[i]) {
			permission -= permissions[i];
		}
	}

	return false;
}

void printAccessDeniedMessgae() {
	cout << "---------------------------------------------" << endl;
	cout << "Access Denied, " << endl;
	cout << "You Don't Have permission To Do This, " << endl;
	cout << "Please Contact Your Admin." << endl;
	cout << "---------------------------------------------" << endl;
}


void goBackToManageUsersMenu(sUser user) {
	cout << "\n\nPress Any Key To Key To Continue... " << endl;
	system("pause>0");
	manageUsers(user);
}

void logout() {
	clearScreen();
	login();
}