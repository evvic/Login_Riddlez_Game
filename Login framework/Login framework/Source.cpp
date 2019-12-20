//Eric Brown programming project
//
//If trying this program for the first time, create an accout with the Login part
//Then proceed to try the riddle game out
//You can quit the program then restart it and login with the same username/password to save riddles progress
//Reach the last level of the riddles and you will have the option to prestige
//prestiging changes the color of the text


//THIS IS THE MAIN WHERE THE ACTUAL PROGRAM WOULD BE
//THE HEADER WILL BE WHERE THE SIGN ON/GAME GOES
#include <iostream>
#include "login.h"
#include "RiddleGame.h"
#include <Windows.h>

using namespace std;

int main() {
	string username = LoginIntro();

	if (username != "q") { //quitting the program if username = q
		Game(username);
	}
}


/*WHERE I LEFT OFF:
	-currently working on the class functions for reading and writing to binary files
	-read users and passwords onto vectors
	   -maybe make a struct/class datatype of users (groups username & password)
	-once the reading and writing is completed, work on getting user input to check if what they entered is correct
	-bonus: have a counter for when a person enters the password incorrectly 10 times
	
	-OKAY, kinda forgot how classes worked but I remember now, gotta think about it before I start going off

	-OKAYOKAY, work on the loadVector() function to populate both user and password vectors, then finish the credentials which needs
		the vectors to be populated

	-HELL YEAH, login is working! Time to find out how to do the riddles... extracting sentences from files
	-FOUND OUT HOW TO GET FULL SENTENCES YES!!!! ok so use void FilePractice(string username) for reference

	-TO-DO: implement the stats struct everywhere, fix the randomize function, make leaderboard and prestige functions
	*/