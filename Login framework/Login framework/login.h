#ifndef LOGIN_H_INCLUDED
#define LOGIN_H_INCLUDED

#include <vector>
#include <ctime>
#include <iostream>

using namespace std;

class User {
	public:
		void setUsername(string assignName); //just sets username
		void setUsername(); //bigger function that asks for username

		void setPassword(string assignPass); //just sets pssword
		void setPassword(); //bigger function that asks for password

		string getUsername();
		string getPassword();

		bool CorrectUsername(char CantFindOrTaken /* 'f' or 't' */); //checks that the username entered is real, char checks if f or t
		bool CorrectPassword(); //cheacks that the password entered is coreect with the username
		bool NoSpaces(); //checks that the username entered has no spaces

		void AppendNewUser(string user, string pass); //new acct has ussername & password added to files
		void ExistingUser();
		void NewUser();
		void GetUserList();
		void GetPasswordList();
		bool IsTaken(string name, char x); //char x determines if it is checking the password or name vector to 
		bool CredentialsPass();
		void loadVector(char userOrPass); //

	protected:
	private:
		string username;
		string password;
		vector<string> users;
		vector<string> passwords;
		int positionInVector;

};

string ExistingUser();
string NewUser();
string LoginIntro();
string CreateAPassword();
bool NameIsLongEnough(string name);

void Title();

void CenterString2(string s);


#endif