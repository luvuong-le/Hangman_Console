// Hangman_Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <ctime>

using namespace std;

void readFile(string filename);
void startGame();

int main()
{
	startGame();
    return 0;
}


void startGame() {
	//cout << "Welcome to hangman!" << endl;
	cout << "---------------------------" << endl;

	string player_name;

	cout << "Enter Player Name: ";

	cin >> player_name;

	cout << "Player Name: " << player_name << endl;


	readFile("hangman_title.txt");
	int failed_inputs = 7;
	int char_exposed = 0;

	cout << "\nStarting Game......" << endl;

	srand(time(NULL));

	//Create a list of words in an array
	string words[] = { "Hello", "Quarter", "Champion", "Million", "Persona", "Giant", "Phantom" };
	//string letters_guessed[] = {};
	string word_guessed = "";

	//Display a random word with * numbers of letters
	int random_index = rand() % 7;

	string assignedWord = "";

	for (int i = 0; i < sizeof(words) / sizeof(words[0]); i++) {
		if (i == random_index) {
			//cout << "Youre word is: " << words[random_index] << endl;
			assignedWord = words[random_index];
		}
	}

	string display = assignedWord;

	for (int i = 0; i < display.size(); i++) {
		display[i] = '*';
	}

	//startGuess(player_name, assignedWord, display, failed_inputs, char_exposed);
}

void readFile(string filename) {
	//Function for reading the ASCII Title in the hangman.txt file
	//http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
	string readLine;

	ifstream myfile(filename.c_str());

	if (myfile.is_open()) {
		while (getline(myfile, readLine)) {
			cout << readLine << endl;
		}
	}
	else {
		cout << "Cannot open file" << endl;
	}
}