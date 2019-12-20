#ifndef RIDDLEGAME_H_INCLUDED
#define RIDDLEGAME_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct stats {
	string username;
	int curLevel;
	int maxLevel;
	int prestige;
};

class Riddle {
public:
	char Game(int maxLevel); //returns user input of choice, after level is played

	void SetQuestion(string ques);
	void SetAnswer(string ans);
	void SetOptions(string opt[3]);
	void SetLevel(int l);

	string GetQuestion();
	string GetAnswer();
	string * GetOptions(); //returns address of array of options
	int GetLevel(); //returns the level of this riddle

protected:
private:
	string question;
	string answer;
	string options[3];
	int level;
};

int Game(string username);
char RandomizeOptions(vector<string> &fourChoices); //returns the letter the answer is equal to


void DisplayGame(stats plaeyr);
//void FilePractice(stats player);
void LoadVector(vector<Riddle>& riddles);
stats LoadProgress(stats player);
//void SaveProgress(stats player);

int Menu(stats player); //returns the level the player chooses
bool CheckMenuOption(char option, stats player);

void CenterString(string s);
void RightString(string s);
void CenterStringStats(stats player);

void UpdateScore(stats player);
bool Prestige(stats king); //returns true if the player decided to prestige
void Leaderboard();


void Credits();
char GameBeaten();

#endif