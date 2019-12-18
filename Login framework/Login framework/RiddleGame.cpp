#include "RiddleGame.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

const string RIDDLE_FILE = "riddles.txt";
const string PROGRESS = "player_progress.txt";

int Game(string username) {
	int level, currLevel;
	DisplayGame(username);
	char choice;

	vector<Riddle> riddles;

	LoadVector(riddles);

	level = LoadProgress(username); //returns max level user reached
	currLevel = Menu(username, level); //returns level player chooses to start at (must be 'unlocked')
	
	if (currLevel >= 0) { //currLevel < 0 if player wants to quit
		cout << "\n\nBACK IN Game()"; //checkpoint

		//start using the class powers
		//make a i.e. riddles.at(i).displayQuestion and riddles.at(i).DisplayOPtionsAndAnswer

		//StartLevel(currLevel, level, username);

		do {
			riddles.at(currLevel).SetLevel(currLevel); //
			choice = riddles.at(currLevel).Game(level); //starts game at current level, passes the users max level
			if (choice == 'm') {
				UpdateScore(username, level);
				currLevel = Menu(username, level);
				if (currLevel < 0) {
					return 69; //quit from main menu
				}
			}
			if (choice != 'f' && choice != 'm') { //if user failed the level, they do not go to the next level
				if (currLevel >= 9) {
					DisplayGame(username);
					GameBeaten();
				}
				currLevel++;
			}
			if (currLevel >= level) {
				level = currLevel;
			}
			if (currLevel >= 9) {
				DisplayGame(username);
				GameBeaten();
				UpdateScore(username, level);
				return 0; //won, game over
			}
		} while (choice != 'q' || currLevel > 9);


		if (choice == 'q') {
			UpdateScore(username, level);
			return 1;
		}

		UpdateScore(username, level);

	}
	else {
		return 69; //entered 'q' from menu
	}
}

char GameBeaten() {
	char trash;
	cout << endl << endl;
	CenterString("CONGRADULATIONS, YOU HAVE BEATEN RIDDLEZ");
	cout << "\n\n\t> ";
	cin >> trash;
	if (trash == 'c') {
		Credits();
	}
	return trash;
}

void UpdateScore(string username, int level) {
	struct userandstats{
		string name;
		int score;
	};

	string tname;
	int tscore, i = 0;
	bool found = false; //if stays false, then its new data to write to the file

	userandstats temp;
	vector<userandstats> list;

	ifstream inProg;
	inProg.open(PROGRESS);


	if (inProg.good()) {
		while (inProg >> tname >> tscore) {
			if (tname == username) {
				tscore = level; //change the score/level that is written for the specific user
				cout << "\n\tusername: " << tname << "\tlevel: " << tscore; //checkpoint
				found = true; 
			}
			temp.name = tname;
			temp.score = tscore;
			list.push_back(temp);
			i++;
		}
	}
	else {
		cout << "\nCould not find " << PROGRESS << ", data cannot be saved.";
	}
	inProg.close();

	if (!found) {
		temp.name = username;
		temp.score = level;
		list.push_back(temp);
		i++;
	}

	for (int j = 0; j < i; j++) {
		cout << "\nuser: " << list.at(j).name << "\tscore: " << list.at(j).score;
	}

	ofstream oProg;
	oProg.open(PROGRESS);

	if (oProg.good()) {
		for (int j = 0; j < i; j++) {
			oProg << list.at(j).name << " " << list.at(j).score << endl;
		}
	}
	else {
		cout << "\nCould not find " << PROGRESS << ", data cannot be saved.";
	}
	oProg.close();
}

int Menu(string username, int level) {
	char option;
	DisplayGame(username);
	do {
		cout << "\n\n\tMax level reached: " << level << endl;
		cout << "\n\tEnter:\n\t -the number of the level you would like to resume between 0 - " << level << '.';
		cout << "\n\t -'q' to quit.";
		cout << "\n\t -'c' for credits.";
		cout << "\n\n > ";

		cin >> option;
		DisplayGame(username);
	} while (!CheckMenuOption(option, level, username));	

	if (option == 'q') {
		return -1;
	}

	//cout << "\nMenu() option = " << option << "\toption - 48 = " << option - 48; //checkpoint

	int currLevel = option - 48;

	/* cout << "\nMenu() currLevel = " << currLevel << "\nchar> "; //checkpoint
	cin >> option; */
	
	return currLevel;
	
	
}

void LoadVector(vector<Riddle> &riddles) {
	string quest, ans;
	string opt[3];
	Riddle temp;

	int i; //count of how many riddles read from file

	cout << "\nLoading riddles...";

	ifstream inRiddle;
	inRiddle.open(RIDDLE_FILE);

	if (inRiddle.good()) {
		for (i = 0; getline(inRiddle, quest); i++) {
			temp.SetQuestion(quest);

			//cout << "\n\tQuestion of i = " << i << ": " << temp.GetQuestion(); //checkpoint

			getline(inRiddle, ans);
			temp.SetAnswer(ans);

			//cout << "\n\tAnswer of i = " << i << ": " << temp.GetAnswer(); //checkpoint

			for (int j = 0; j < 3; j++) {
				getline(inRiddle, opt[j]);
			}
			temp.SetOptions(opt);

			riddles.push_back(temp);
		}
	}
	inRiddle.close();

	//cout << endl << i + 1 << " riddles have been read and file " << RIDDLE_FILE << " has been closed."; //checkpoint
}

int LoadProgress(string username) {
	string name;
	int level;

	ifstream inProg;
	inProg.open(PROGRESS);

	if (inProg.good()) {
		while (inProg >> name >> level) {
			if (name == username) {
				cout << "\n\tusername: " << name << "\tlevel: " << level; //checkpoint
				inProg.close();
				return level;
			}
		}
	}

	cout << "\nCould not find saved progress, starting at level 0.";
	inProg.close();
	return 0;
}

void SaveProgress(string username, int level) {

}

void DisplayGame(string username) {
	system("CLS");
	string welcome = "Welcome " + username;

	//cout << "\n\t\tRiddle Quiz\n";
	cout << endl;

	string first = "   ____                ____     ____     _     U _____ u  _____   ";
	string second = "U |  _\"\\ u     ___    |  _\"\\   |  _\"\\   |\"|    \\| ___\"|/ |\"_   /u ";
	string third = " \\| |_) |/    |_\"_|  /| | | | /| | | |U | | u   | _ | \"  U / //  x";
	string fourth = "   |  _ <       | |   U| |_| |\\U| |_| |\\\\| |/__  | |___   \\/ /_     ";
	string fifth = "  |_| \\_\\    U/| |\\u  |____/ u |____/ u |_____| |_____|  /____|    ";
	string sixth = "  //   \\\\_.-,_|___|_,-.|||_     |||_    //  \\\\  <<   >>  _//<<,-   ";
	string seventh = "v (__)  (__)\\_)-' '-(_/(__)_)   (__)_)  (_\")(\"_)(__) (__)(__) (_/ ";

	string title[7] = { first, second, third, fourth, fifth, sixth, seventh };

	for (int i = 0; i < 7; i++) {
		cout << endl;
		CenterString(title[i]);
	}

	cout << endl << endl;
	CenterString(welcome);
	//cout << "\n\t\tWelcome " << username << endl;
}

void FilePractice(string username) {
	string test;
	int count = 0;

	ifstream inRiddle;
	inRiddle.open(RIDDLE_FILE);

	if (inRiddle.good()) {
		while (getline(inRiddle, test)) {
			cout << endl << ++count << ". " << test;
		}
	}
	inRiddle.close();
}

bool CheckMenuOption(char option, int level, string username) {
	/*allowed: c, q, 0 - level*/

	char clevel = 48 + level; //convert level to char
	//cout << "\n\n\tclevel = " << clevel; //checkpoint

	//int currLevel = option - 48;
	
	switch (option) {
	case 'c':
		Credits();
		system("CLS");
		DisplayGame(username);
		//Menu(username, level);
		return false; //'c' is a valid option but must return false to continue the loop
		break;
	case 'q':
		return true;
		break;
	default:
		for (char i = '0'; i <= clevel; i++) {
			if (i == option) {
				cout << "\nLoading level " << clevel << "...";

				//StartLevel(currLevel, level, username);
				return true;
			}
		}
		break;
	}
	cout << "\n\nCould not recognize input.\n\n";
	return false;
}

void StartLevel(int currLevel, int maxLevel, string username) {

	cout << "\n\ncurrent level: " << currLevel << "\tmax level: " << maxLevel << "\tusername: " << username; //checkpoint



}

char Riddle::Game(int maxLevel) {
	vector<string> fouroptions{ Riddle::answer, Riddle::options[0], Riddle::options[1], Riddle::options[2] };

	char answerPos = RandomizeOptions(fouroptions);
	char choice, trash;
	
	cout << "\nRiddle::Game() answer: " << Riddle::answer << "\tanswer position: " << answerPos; //checkpoint
	
	system("CLS");

	RightString("HIGHEST LEVEL " + to_string(maxLevel)); //displays highest score top right
	cout << endl;
	CenterString("LEVEL " + to_string(Riddle::level)); //displays current level
	cout << endl;
	CenterString(Riddle::question); //displays qustion
	cout << endl << endl;

	/*cout << "\nRiddle::Game() answer: " << Riddle::answer << "\tanswer position: " << answerPos; //checkpoint
	for (int i = 0; i < 4; i++) { //checkpoint
		cout << endl << fouroptions.at(i);
	} */


	for (char i = 'a'; i < 'e'; i++) {
		cout << "\n\t" << i << ". " << fouroptions.at(i - 97);
	}

	cout << "\n\n\t-'q' to quit the level.";
	cout << "\n\n\t-'m' to return to the menu.";

	cout << "\n\n\t> ";

	cin >> choice;
	choice = tolower(choice);

	while (choice != 'a' && choice != 'b' && choice != 'c' && choice != 'd' && choice != 'q' && choice != 'm') {
		cout << "\t> ";
		cin >> choice;
		choice = tolower(choice);
	}

	if (choice == 'q') {
		CenterString("Exiting the riddle...");
		return choice;
	}
	if (choice == 'm') {
		CenterString("Exiting to main menu...");
		return choice;
	}

	if (choice == answerPos) {
		cout << endl << endl;
		CenterString("CORRECT!");
		if (maxLevel == Riddle::level) {
			maxLevel++;
			cout << endl << endl;
			string lvlIncrease = "Max level has increased to " + to_string(maxLevel) + "!";
			CenterString(lvlIncrease);
		}

		cout << "\n\n\tPress any key + \"enter\" to continue.\n\t> ";
		cin >> trash;

		return choice;
	}
	else {
		cout << endl << endl;
		CenterString("Incorrect.");
		cout << "\n\n\tAnswer is " << answerPos; //checkpoint
		cout << "\n\n\tPress any key + \"enter\" to continue.\n\t> ";
		cin >> trash;
		return 'f';
	}

	return choice;
}

char RandomizeOptions(vector<string>& fourChoices) {
	srand(time(0)); //randomize seed
	int answerPos = rand() % (4);
	int randOpt;
	
	do {
		randOpt = rand() % (4);
	} while (randOpt == answerPos); //randomize another option
	
	string temp = fourChoices.at(answerPos); //answer switches positions
	fourChoices.at(answerPos) = fourChoices.at(0);
	fourChoices.at(0) = temp;

	temp = fourChoices.at(randOpt); //one more little switch
	fourChoices.at(randOpt) = fourChoices.at(0);
	fourChoices.at(0) = temp;

	//cout << "\nRandomizeOptions(): "; //HUGE CHECKPOINT
	/*for(int i = 0; i < 4; i++) {
		cout << endl << fourChoices.at(i);
	}*/
	//cout << "\npausing moment, enter an int to continue: ";
	//cin >> randOpt;

	char answrr = 97 + answerPos;
	cout << "\n\nANSWER is at position: " << answrr;

	return 97 + answerPos;
}

void Credits() {
	string EricBrown;
	system("CLS");

	cout << endl << endl;
	CenterString("Credits");

	cout << endl << endl;
	CenterString("All code has been written by Eric Brown.");

	cout << "\n\n\tStudent at Tampere University of Applied Sciences.";
	cout << "\n\tInternational Software Engineering Bachelor's Degree.";
	cout << "\n\n\tRiddles were copied from:\thttps://www.gotoquiz.com/can_you_guess_these_riddles";
	cout << "\n\n\tPress any key + \"enter\" to exit credits.";
	cout << "\n\n\t> ";
	cin >> EricBrown;
}

void CenterString(string s) {

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

void RightString(string s) {

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
	int position = (int)((c_width - length));
	for (int i = 0; i < position; i++) {
		cout << ' ';
	}
	cout << s;
}

void Riddle::SetQuestion(string ques) {
	Riddle::question = ques;
}

void Riddle::SetAnswer(string ans) {
	Riddle::answer = ans;
}

void Riddle::SetOptions(string opt[3]) {
	for (int i = 0; i < 3; i++) {
		Riddle::options[i] = opt[i];
	}
}

void Riddle::SetLevel(int l) {
	Riddle::level = l;
}

string Riddle::GetQuestion() {
	return Riddle::question;
}

string Riddle::GetAnswer() {
	return Riddle::answer;
}

string * Riddle::GetOptions() {
	return Riddle::options; //pass by pointer??
} //returns the pointer of the array of strings of options

int Riddle::GetLevel() {
	return Riddle::level;
}