#ifndef RIDDLEGAME_H_INCLUDED
#define RIDDLEGAME_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
void StartLevel(int currLevel, int maxLevel, string username);
char RandomizeOptions(vector<string> &fourChoices); //returns the letter the answer is equal to


void DisplayGame(string username);
void FilePractice(string username);
void LoadVector(vector<Riddle>& riddles);
int LoadProgress(string username);
void SaveProgress(string username, int level);

int Menu(string username, int level); //returns the level the player chooses
bool CheckMenuOption(char option, int level, string username);

void CenterString(string s);
void RightString(string s);

void UpdateScore(string username, int level);


void Credits();
char GameBeaten();


#endif