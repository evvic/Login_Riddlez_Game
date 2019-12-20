#include "login.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

string USERNAMES = "usernames.txt";
string PASSWORDS = "passwords.txt";

string LoginIntro() { //MAKE THIS PRETTIER LATER
	string username;
	char choice;
	system("CLS");

	cout << endl;
	Title(); //displays text art
	cout << endl << endl;
	CenterString2("Are you an existing user?");
	cout << "\n\n\t-'n' no (create an account)\n\t-'y' yes\n\t-'q' quit";
	cout << "\n\n\t> ";
	cin >> choice;
	choice = toupper(choice);

	while (choice != 'Y' && choice != 'N' && choice != 'Q') {
		system("CLS");

		cout << endl;
		Title(); //displays text art
		cout << endl << "\tInput was invalid." << endl;
		CenterString2("Are you an existing user?");
		cout << "\n\n\t-'n' no (create an account)\n\t-'y' yes\n\t-'q' quit";
		cout << "\n\n\t> ";
		cin >> choice;
		choice = toupper(choice);
	}

	switch (choice) {
	case 'Y':
		username = ExistingUser(); //change code so this will return the uername so we know who is logging when going to the next part
		break;
	case 'N':
		username = NewUser(); //change code so this will return the uername so we know who is logging when going to the next part
		break;
	case 'Q':
		cout << "\nExiting the program...\n\n";
		return "q";
		break;
	default:
		cout << "\nExiting the program...\n\n";
		return "q";
		break;
	}

	if (username == "b" || username == "B") {
		cout << "\n\tReturning to login....";
		username = LoginIntro();
	}

	cout << "\nWelcome " << username << endl;
	return username;

}

string ExistingUser() {
	User guy;
	guy.loadVector('u');
	guy.loadVector('p');
	
	cout << endl;

	CenterString2("EXISTING USER");

	cout << endl;

	cout << "\n\t-'b' to go back.";
	cout << endl;

	guy.setUsername(); //asks for username
	if (guy.getUsername() == "b" || guy.getUsername() == "B") return "b"; //go back to main login
	guy.setPassword(); //asks for password

	
	/*while ((guy.CorrectUsername() && guy.CorrectPassword())); {//%%%%% CHECK THE LOGIC HERE %%%%%%
		cout << "\nUsername and password did not match.";
		cout << "\nEnter 'q' to quit back into the main menu";

		guy.setUsername(); //asks for username
		guy.setPassword(); //asks for password

		if (guy.getUsername() == "q") {
			LoginIntro();
		}
	} */

	while (!(guy.CorrectUsername('f' /*prompt if taken*/) && guy.CorrectPassword())) {
		cout << "\n\t-'b' to go back.";
		cout << endl;
		guy.setUsername(); //asks for username
		if (guy.getUsername() == "b" || guy.getUsername() == "B") return "b"; //breaks out of loop
		guy.setPassword(); //asks for password
	}

	/*AT THIS POINT THE USER CAN MOVE BACK TO MAIN*/

	return guy.getUsername();
}

string NewUser() {
	User newguy;
	newguy.loadVector('u');
	newguy.loadVector('p');

	string name;

	cout << endl;
	CenterString2("NEW USER");
	cout << endl;

	do {
		cout << "\n\t-'b' to go back.";
		cout << "\n\tCreate a username: ";
		cin >> name;
		if (name == "b" || name == "B") return "b";
		newguy.setUsername(name);
	} while (!NameIsLongEnough(newguy.getUsername()) || newguy.CorrectUsername('t' /*username TAKEN?*/) || !newguy.NoSpaces());

	newguy.setPassword(CreateAPassword()); //creates password

	cout << "\n\tusername: " << newguy.getUsername() << "\n\tpassword: " << newguy.getPassword(); //checkpoint

	newguy.AppendNewUser(newguy.getUsername(), newguy.getPassword());

	/*AT THIS POINT THE USER CAN MOVE BACK TO MAIN*/

	return newguy.getUsername();
}

void User::AppendNewUser(string user, string pass) {
	ofstream outUsers, outPasswords;

	cout << "\n\tUser::AppendNewUser()"; //checkpoint

	outUsers.open(USERNAMES, ios::binary | ios::app); //open and check usernames text

	//cout << "\n\toutUsers.open(ios::binary, ios::app)"; //checkpoint

	if (outUsers.fail()) {
		cout << "\nCannot open " << USERNAMES;
		//return 0; //void so can't return anything
	}

	//cout << "\n\toutUsers.open()"; //checkpoint

	outPasswords.open(PASSWORDS, ios::binary | ios::app); //open and check passwords text
	if (outPasswords.fail()) {
		cout << "\nCannot open " << PASSWORDS;
		//return 0; //void so can't return anything
	}
	//cout << "\n\toutPasswords.open()"; //checkpoint

	outUsers << user << ' ';
	outPasswords << pass << ' '; 
	cout << "\nwriting new user info to files..."; //checkpoint

	User::users.push_back(user);
	User::passwords.push_back(pass);

	outUsers.close();
	outPasswords.close();
}

bool User::NoSpaces() {
	if (User::username.find(" ") == -1) {
		return true;
	}
	else {
		cout << endl;
		CenterString2("Cannot include spaces in the username.");
		return false;
	}

}

bool User::CorrectUsername(char CantFindOrTaken /* 'f' or 't' */) {
	if (User::users.size() == 0) {
		cout << "\nList of users is empty.\nReloading vectors...";
		User::loadVector('u');
		User::loadVector('p');
		if (User::users.size() == 0) {
			cout << "\n\t*error* list of users is still empty.";
		}
	}

	if (User::username == "b" || User::username == "B") {
		return true;
	}

	for (int i = 0; i < User::users.size(); i++) { //run through users vector and check for the given username
		if (User::username == User::users.at(i)) {
			User::positionInVector = i;

			//checkpoint..
			//cout << "\n\tFOUND USERNAME in vector position " << User::positionInVector << " and username is in vector: " << User::users.at(User::positionInVector);
			//cout << "\n\tCorrectUsername() returned true";
			cout << endl;
			if (CantFindOrTaken == 't') CenterString2("Username is already taken."); //cout << "\nUsername is already taken.";
			//cout << "\nUsername is already taken.";
			return true;
		}else if (i == User::users.size() - 1) {
			switch (CantFindOrTaken) {
			case 'f': //F = can't FIND
				//cout << "\nCould not find username.";
				cout << endl;
				CenterString2("Could not find username.");
				break;
			case 't': //T = username TAKEN
				//cout << "\nUsername is already taken.";
				//cout << endl;
				//CenterString2("Username is already taken.");
				break;
			default:
				break;
			}
			return false;
		}
	}

	return false; //if vector is empty
}

bool User::CorrectPassword() {
	//checkpoint
	//cout << "\n\tPASSWORD entered was: " << User::password << " and password at postion " << User::positionInVector << " is " << User::passwords.at(User::positionInVector);

	if (User::passwords.at(User::positionInVector) == User::password) {
		//cout << "\n\tCorrectPassword()\n\tusername " << User::username << "\n\tpassword " << User::password;
		//cout << "\n\tCorrectPassword() returned true";
		return true;
	}
	else {
		cout << "\nIncorrect password for " << User::username << '.';
		return false;
	}
}

void User::loadVector(char userOrPass) {
	ifstream inFS;
	ofstream oFS;
	string temp, text_file;
	vector<string> values;

	switch (userOrPass) { //changes depending on if its retrieving the usernames or the passwords
	case 'u':
		text_file = USERNAMES;
		break;
	case 'p':
		text_file = PASSWORDS;
		break;
	default: 
		cout << "\nerror in loadVector switch\n";
		//return 0; //void so can't return anything
		break;
	}

	inFS.open(text_file, ios::binary); //open text b text file for usernames, specify it's binary.... fOr ENcRypTiOn

	if (inFS.fail()) {
		cout << "\nCannot open " << text_file;
		//return 0; //void so can't return anything

		cout << "\nCreating " << text_file << "...";
		oFS.open(text_file, ios::binary); //create text files
		oFS.close();
	}

	while (!inFS.fail() && !inFS.eof()) { //loops 
		inFS >> temp;
		values.push_back(temp);
	}

	switch (userOrPass) { //check if its for users or passwords and store in the right vector
	case 'u':
		User::users.reserve(values.size());
		copy(values.begin(), values.end(), back_inserter(User::users));
		break;
	case 'p':
		User::passwords.reserve(values.size());
		copy(values.begin(), values.end(), back_inserter(User::passwords));
		break;
	default:
		cout << "\nerror in loadVector SECOND switch\n";
		//return 0; //void so can't return anything
		break;
	}

	inFS.close();
}

/*bool User::CredentialsPass() {
	int position; 

	for (int i = 0; i < User::users.size(); i++) { //run through users vector and check for the given username
		if (User::username == User::users.at(i)) {
			position = i;
			break;
		}

		if (i == User::users.size() - 1) {
			cout << "Could not find "
		}
	}

	//CHECK IF GIVEN PASSWORD MATCHES 



	return true; //if credentials pass
} */

void User::setUsername() {
	string temp;

	cout << "\n\tusername: ";
	cin >> User::username;
}

void User::setPassword() {
	string temp;

	cout << "\n\tpassword: ";
	cin >> User::password;
}

void User::setUsername(string assignName) {
	User::username = assignName;
}

void User::setPassword(string assignPass) {
	User::password = assignPass;
}

string User::getUsername() {
	return User::username;
}

string User::getPassword() {
	return User::password;
}

string CreateAPassword() {
	string pass, pass2;

	cout << "\nCreate a password: ";
	cin >> pass;
	cout << "Re-enter password: ";
	cin >> pass2;

	while (pass != pass2) {
		cout << "\nPasswords did not match.";
		cout << "\nCreate a password: ";
		cin >> pass;
		cout << "Re-enter password: ";
		cin >> pass2;
	}
	return pass;
}

bool NameIsLongEnough(string name) {
	if (name.length() > 2) {
		return true;
	}
	else if (name.length() <= 2) {
		//cout << "\nUsername entered is too short, must be at east 3 characters.";
		cout << endl;
		CenterString2("Username entered is too short, must be at east 3 characters.");
		return false;
	}
}

void CenterString2(string s) {

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
	{
		// an error occourred
		cerr << "Cannot determine console size." << endl;
	}
	else
	{
		//cout << "\n\nThe console is " << csbi.srWindow.Right - csbi.srWindow.Left << " wide." << endl; //checkpoint
	}


	int c_width = csbi.srWindow.Right - csbi.srWindow.Left;
	int length = s.size();
	int position = (int)((c_width - length) / 2);
	for (int i = 0; i < position; i++) {
		cout << ' ';
	}
	cout << s;
}

void Title() {
	string first = "  _                _       ";
	string second = " | |    ___   __ _(_)_ __  ";
	string third = " | |   / _ \\ / _` | | '_ \\ ";
	string fourth = " | |__| (_) | (_| | | | | |";
	string fifth = " |_____\\___/ \\__, |_|_| |_|";
	string sixth = "             |___/         ";

	string login[6] = { first, second, third, fourth, fifth, sixth };

	for (int i = 0; i < 6; i++) {
		CenterString2(login[i]);
		cout << endl;
	}
}

/*

void User::GetUserList() {
	FILE* inFile = fopen("usernames.txt", "rb");
	while (fread(&users, sizeof(string), 1, inFile));
	
}

string User::getUsername(string ) {
	do {
		cout << "Username: ";
		cin >> username;
	} while (username == username);
	return username;
}

string User::getPassword(){
	return password;
}

void User::ExistingUser() {
	cout << "Let's get your username & password\n";

}

void User::NewUser() {
	User guy;
	string name, password;
	cout << "Let's create a username and password\n";

	do {
		cout << "\nEnter a username: ";
		cin >> name;
		guy.getUsername(name);
	} while (User.IsTaken(name, 'n'));
	
}

bool User::IsTaken(string name, char x) {
	switch (x) {
	case 'n':
			//fopen(USERNAMES, "rb");
			//fread()
			break;
	default:
			break;
	}

	return true; //means that the name is taken
} */