//THIS IS THE MAIN WHERE THE ACTUAL PROGRAM WOULD BE
//THE HEADER WILL BE WHERE THE SIGN ON GOES
#include <iostream>

#include "login.h"
#include "RiddleGame.h"
#include <Windows.h>

using namespace std;

void CenterString(string s); //prints the string argument centered


int main() {
	string username = LoginIntro();

	if (username != "q") { //quitting the program if username = q
		Game(username);
	}

	//make some qquick easy bs game or basic final idea
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
	*/