#include "RiddleGame.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iomanip>
#include <ios>


using namespace std;

const string RIDDLE_FILE = "riddles.txt";
const string PROGRESS = "player_progress.txt";

int Game(string username) {
	char choice, trash;
	int tcurrentlvl;

	vector<Riddle> riddles;
	stats player;

	player.username = username;

	LoadVector(riddles);
	player = LoadProgress(player); //returns max level user reached
	player.curLevel = 0; //initilize curLevel

	DisplayGame(player);
	tcurrentlvl = Menu(player); //returns level player chooses to start at (must be 'unlocked')
	
	//cout << "\nEntering game for the first time." << player.prestige; //checkpoint
	//cout << "\nPlayer curlevel = " << player.curLevel; //checkpoint
	//cin >> trash;



	player = LoadProgress(player); //just incase guy prestiges in first menu
	player.curLevel = tcurrentlvl;
	if (player.maxLevel > 0) {
		player.curLevel = tcurrentlvl; //prestige/current level fix
	}

	//cout << "\nPast prestige if statement.\n player.curLevel = " << player.curLevel; //checkpoint
	//cin >> trash;

	if (player.curLevel >= 0) { //currLevel < 0 if player wants to quit

		//cout << "\n\nBACK IN Game()"; //checkpoint
		//cin >> trash;

		//start using the class powers
		//make a i.e. riddles.at(i).displayQuestion and riddles.at(i).DisplayOPtionsAndAnswer

		//StartLevel(currLevel, level, username);

		do {
			riddles.at(player.curLevel).SetLevel(player.curLevel); //
			choice = riddles.at(player.curLevel).Game(player.maxLevel); //starts game at current level, passes the users max level

			//cout << "\nINSIDE do while loop, past the riddles"; //checkpoint
			//cin >> trash;

			if (choice == 'm') {
				UpdateScore(player);
				tcurrentlvl = Menu(player);

				//cout << "Loading menu through do while loop of riddles, line 54"; //checkpoint
				//cin >> trash;

				player = LoadProgress(player); //just incase the guy prestiged
				player.curLevel = tcurrentlvl; //prestige/current level fix
				if (player.curLevel < 0) {
					return 69; //quit from main menu
				}
			}
			//cout << "\nexit from game back to menu, prestige: " << player.prestige; //checkpoint
			//cin >> trash;
			if (choice != 'f' && choice != 'm' && choice != 'q') { //if user failed the level, they do not go to the next level
				if (player.curLevel >= 9) {
					DisplayGame(player);
					GameBeaten();
				}
				player.curLevel++;
			}
			if (player.curLevel >= player.maxLevel) {
				player.maxLevel = player.curLevel;
			}
			if (player.curLevel >= 9) {
				DisplayGame(player);
				GameBeaten();
				UpdateScore(player);
				return 0; //won, game over
			}
		} while (choice != 'q' || player.curLevel > 9);


		if (choice == 'q') {
			UpdateScore(player);
			return 1;
		}

		UpdateScore(player);

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
	if (trash == 'm') {
		cout << "\nmenu?\nmaybe i'll code this later...";
	}
	return trash;
}

void UpdateScore(stats player) {

	string tname;
	int tscore, tprestige, i = 0;
	bool found = false; //if stays false, then its new data to write to the file

	stats temp;
	vector<stats> list;

	ifstream inProg;
	inProg.open(PROGRESS);


	if (inProg.good()) {
		while (inProg >> tname >> tscore >> tprestige) {
			if (tname == player.username) {
				tscore = player.maxLevel; //change the score/level that is written for the specific user
				tprestige = player.prestige;

				//cout << "\n\tusername: " << tname << "\tlevel: " << tscore << "\tprestige: " << tprestige; //checkpoint
				found = true; 
			}
			temp.username = tname;
			temp.maxLevel = tscore;
			temp.prestige = tprestige;
			list.push_back(temp);
			i++;
		}
	}
	else {
		cout << "\nCould not find " << PROGRESS << ", data cannot be saved.";
	}
	inProg.close();

	if (!found) {
		temp.username = player.username;
		temp.maxLevel = player.maxLevel;
		temp.prestige = player.prestige;
		list.push_back(temp);
		i++;
	}

	/*for (int j = 0; j < i; j++) { //checkpoint
		cout << "\nuser: " << list.at(j).username << "\tscore: " << list.at(j).maxLevel;
	} */

	cout << endl; //leaves a space at the very end when the program terminates

	ofstream oProg;
	oProg.open(PROGRESS);

	if (oProg.good()) {
		for (int j = 0; j < i; j++) {
			oProg << list.at(j).username << " " << list.at(j).maxLevel << " " << list.at(j).prestige << endl;
		}
	}
	else {
		cout << "\nCould not find " << PROGRESS << ", data cannot be saved.";
	}
	oProg.close();
}

int Menu(stats player) {

	char option;
	DisplayGame(player);
	do {
		cout << "\n\n\tMax level reached: " << player.maxLevel << endl;
		cout << "\n\tEnter:\n\t -the number of the level you would like to resume between 0 - " << player.maxLevel << ".\n";
		cout << "\n\t -'q' to quit.";
		cout << "\n\t -'c' for credits.";
		cout << "\n\t -'s' scoreboard.";
		if (player.maxLevel >= 9) {
			cout << "\n\t -'p' ...to prestige.";
		}
		cout << "\n\n\t > ";
		cin >> option;
		if ('p' == option || 'P' == option) {
			//must reset maxLevel & curLevel and update prestige 
			if (Prestige(player)) { //if true then currLevel should also be reset
				player = LoadProgress(player);
				player.curLevel = 0;
				player.maxLevel = 0;
				system("CLS");
			}
		}
		DisplayGame(player);
	} while (!CheckMenuOption(option, player));	

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

stats LoadProgress(stats player) {
	string name;
	int level;
	int prestige;
	stats temp;
	temp.username = player.username;
	temp.curLevel = 0; //better to initialize to 0 than to pass garbage
	temp.maxLevel = 0;
	temp.prestige = 0;

	ifstream inProg;
	inProg.open(PROGRESS);

	if (inProg.good()) {
		while (inProg >> name >> level >> prestige) {
			if (name == player.username) {

				//cout << "\n\tusername: " << name << "\tlevel: " << level <<"\tprestige: " << prestige; //checkpoint
				
				inProg.close();
				temp.prestige = prestige;
				temp.maxLevel = level;
				return temp;
			}
		}
	}

	cout << "\nCould not find saved progress, starting at level 0.";
	inProg.close();
	temp.curLevel = 0;
	return temp;
}

void DisplayGame(stats player) {
	system("CLS");
	/*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// you can loop k higher to see more color choices
	for (int k = 1; k < 255; k++)
	{
		// pick the colorattribute k you want
		SetConsoleTextAttribute(hConsole, k);
		cout << k << " I want to be nice today!" << endl;
	} */

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player.prestige);

	string welcome = "Welcome " + player.username; //change color to simulate prestige!

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
	//system("COLOR 5");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player.prestige);
	CenterString(welcome);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	//system("COLOR 0");
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

bool CheckMenuOption(char option, stats player) {
	/*allowed: c, q, 0 - level, s, p*/

	char clevel = 48 + player.maxLevel; //convert level to char
	//cout << "\n\n\tclevel = " << clevel; //checkpoint

	//int currLevel = option - 48;
	
	switch (option) {
	case 'c':
		Credits();
		system("CLS");
		DisplayGame(player);
		//Menu(username, level);
		return false; //'c' is a valid option but must return false to continue the loop
		break;
	case 'q':
		return true;
		break;
	case 's':
		Leaderboard();
		system("CLS");
		DisplayGame(player);
		return false; //'s' is a valid option but must return false to continue the loop
		break;
	case 'p':
		/*if (Prestige(player)) { //if true then currLevel should also be reset
			system("CLS");
			player = LoadProgress(player);
			player.curLevel = 0;
		}*/
		DisplayGame(player);
		return false; //'p' is a valid option but must return false to continue the loop
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

void Leaderboard() {
	int level, prestige, count = 0;
	string name;
	char trash;
	system("CLS");
	/*cout << "\n\tIn my eyes you are number one";
	cout << "\n\t >";5
	cin >> trash; */

	vector<stats> players;
	stats temp;
	
	ifstream inProg;
	inProg.open(PROGRESS);

	if (inProg.good()) {
		while (inProg >> temp.username >> temp.maxLevel >> temp.prestige) {
			count++;
			temp.curLevel = 0;
			players.push_back(temp);
		}

		int i, j, key;
		stats nkey;

		for (i = 1; i < count; i++) {
			key = players.at(i).maxLevel;
			nkey = players.at(i);

			for (j = i - 1; (j >= 0) && (players.at(j).maxLevel < key); j--) {
				players.at(j + 1) = players.at(j);
			}
			players.at(j + 1).maxLevel = key;
			players.at(j + 1) = nkey;
		} 

		cout << endl;
		CenterString("Leaderboard");

		string top = "Level     Username     Prestige";
		cout << endl << endl;
		CenterString(top);
		cout << endl;
		for (int k = 0; k < count && k < 5; k++) { //MAKE THIS FOR LOOP DISPLAY BEAUTY
			cout << endl;
			CenterStringStats(players.at(k));
		}
		cout << endl;

	}
	else {
		cout << endl;
		CenterString("Leaderboard");
		cout << endl << endl;
		CenterString("Error.");
		cout << "\n\n\tCould not open file " << PROGRESS;
	}
	cout << "\n\n\tPress any key + \"enter\" to exit scoreboard.";
	cout << "\n\n\t> ";
	cin >> trash;
	inProg.close();
}

void CenterStringStats(stats player) {

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		// an error occourred
		cerr << "Cannot determine console size." << endl;
	}
	else {
		//cout << "\n\nThe console is " << csbi.srWindow.Right - csbi.srWindow.Left << " wide." << endl; //checkpoint
	}

	int c_width = csbi.srWindow.Right - csbi.srWindow.Left;
	int length = 22; //s.size();
	int position = (int)((c_width - length) / 2);
	for (int i = 0; i < position; i++) {
		cout << ' ';
	}
	cout << setw(4) << setfill('.') << left << player.maxLevel << setw(14) << left << setfill(' ') << player.username;
	cout << setw(4) << right << player.prestige;
}

bool Prestige(stats king) { //return true if player prestiged, false if not
	bool choice = false; //assume they are smart and don't do it
	char dontdoit;
	system("CLS");
	cout << endl;
	CenterString("Prestige Menu");
	cout << endl << endl;
	CenterString("Congradulations " + king.username + ", you played this shitty game so much you've hit the max level.");
	if (king.prestige > 0) {
		cout << endl;
		CenterString("In fact, this isn't even you're first time.");
		cout << endl;
		CenterString("Why are you doing this to yourself XD");
	}
	if (king.prestige == 5) {
		cout << endl << endl;
		CenterString("I'm gonna throw up, you're actually at prestige 5.");
	}
	else if (king.prestige > 6) {
		cout << endl << endl;
		CenterString("Okay, at this point you must be just changing your prestige in the file.");
	}
	cout << endl << endl;
	CenterString("To prestige means to lose all progress...");
	cout << endl;
	CenterString("for practically nothing but to brag that you've played this game so much.");

	do {
		cout << "\n\n\tSo... would you like to prestige?\n\t-'y' yes.\n\t-'n' no, I'm sane.";
		cout << "\n\n\t >";
		cin >> dontdoit;
		tolower(dontdoit);
	} while (dontdoit != 'y' && dontdoit != 'n');
	if (dontdoit == 'y') {
		choice = true;
		system("CLS");
		cout << "deleting progress...";
		king.curLevel = 0;
		king.maxLevel = 0;
		king.prestige++;

		cout << endl << endl << endl << endl;
		string well = "Congradulations " + king.username + ", you are now prestige " + to_string(king.prestige) + "!";
		CenterString(well);

		//cout << endl << king.username << " prestiged to " << king.prestige; //checkpoint

		UpdateScore(king);
		cout << "\n\n\tPress any key + \"enter\" to return to menu.";
		cout << "\n\n\t> ";
		cin >> dontdoit;
	}
	else {
		system("CLS");
		choice = false;
		cout << "good choice haha";
	}

	return choice;

}

char Riddle::Game(int maxLevel) {
	vector<string> fouroptions{ Riddle::answer, Riddle::options[0], Riddle::options[1], Riddle::options[2] };

	char answerPos = RandomizeOptions(fouroptions); //~~~~~~~~~~~~~~~~~RANDOMIZER not working
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

	/* temp = fourChoices.at(randOpt); //one more little switch //~~~~~~~~~~~~~~THIS IS WHERE THE ERROR IS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	fourChoices.at(randOpt) = fourChoices.at(0);
	fourChoices.at(0) = temp; */

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
	cout << "\n\teric.brown@tuni.fi";
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