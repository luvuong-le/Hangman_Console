// Hangman_Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include <limits.h>
#undef min
#undef max

using namespace std;

void readFile(string filename);
void startGame();
void startGuess(string playerName, string assignedWord, string hidden, int failed_inputs, int char_exposed);
void startTwoPlayer();
void chooseGameType();
void clearScreen();

int main()
{
	chooseGameType();
    return 0;
}

void clearScreen() {
	system("cls");
}

void chooseGameType() {
	clearScreen();
	readFile("hangman_title.txt");
	int gameChoice = 0; 
	cout << "Please enter which mode you would like to play";
	cout << "\n1. Single Player \n2. Two Players \n3. Exit" << endl;
	cout << "Enter number for game: ";
	cin >> gameChoice;

		while (gameChoice != 3) {
			switch (gameChoice) {
			case 1: 
				startGame();
				break;
			case 2: 
				startTwoPlayer();
				break;
			case 3:
				exit(EXIT_SUCCESS);
				break;
			default:
				cout << "Invalid Input, You must enter a number";
				cin.clear();
				cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
				Sleep(0700);
				chooseGameType();
				break;
			}
		}
}


void startGame() {
	clearScreen();

	readFile("hangman_title.txt");
	int failed_inputs = 7;
	int char_exposed = 0;


	string player_name;

	cout << "Enter Player Name: ";

	cin >> player_name;

	cout << "Player Name: " << player_name << endl;


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

	startGuess(player_name, assignedWord, display, failed_inputs, char_exposed);
}

void startTwoPlayer() {
	clearScreen();
}

void startGuess(string playerName, string word, string hidden, int failed_inputs, int char_exposed)
{
	//Identify a list of max guesses = 5
	string guess;
	string start_new_game;

	while (char_exposed < word.length()) {
		bool correct_guess = false;
		//Begin Letting player guess the character one by one
		cout << "\n\nPlease enter a character to guess in: " << hidden;
		cout << "\nEnter Letter: ";

		cin >> guess;

		if (guess.length() == 1) {
			//If guess was correct, reveal a letter in the array
			for (int i = 0; i < word.length(); i++) {
				if (tolower(guess[0]) == tolower(word[i])) {
					if (hidden[i] == word[i]) {
						cout << "\nYouve already guessed this letter!" << endl;
						startGuess(playerName, word, hidden, failed_inputs, char_exposed);
					}
					else {
						//If letter matches, display the hidden letter
						hidden[i] = word[i];
						//Logic to display one less star if character guessed is correct
						cout << ">>> You guessed right! {" << word[i] << "}";
						char_exposed++;
						correct_guess = true;
						continue;
					}
				}
			}
		}
		else {
			cout << "Please enter one letter only" << endl;
			startGuess(playerName, word, hidden, failed_inputs, char_exposed);
		}

		if (correct_guess == false) {
			failed_inputs--;
			switch (failed_inputs) {
			case 0:
				readFile("gameOver.txt");
				cout << "You have run out of tries" << endl;
				cout << "The word was: " << word << endl;
				cout << "Better luck next time!" << endl;
				cout << "\nWould you like to play again? [Y/N]" << endl;
				cin >> start_new_game;
				if (start_new_game == "Y" || start_new_game == "y") {
					chooseGameType();
				}
				else if (start_new_game == "N" || start_new_game == "n") {
					cout << "See you next time!!" << endl;
					exit(EXIT_SUCCESS);
				}
				break;
			case 1:
				clearScreen();
				readFile("showRLeg.txt");
				cout << "\nYou have one more chance left!!\n" << endl;
				break;
			case 2:
				clearScreen();
				readFile("showLLeg.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 3:
				clearScreen();
				readFile("showRArm.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 4:
				clearScreen();
				readFile("showLArm.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 5:
				clearScreen();
				readFile("showBody.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 6:
				clearScreen();
				readFile("showHead.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			default:
				cout << "Error" << endl;
				break;
			}
		}
	}

	if (char_exposed == hidden.length()) {
		cout << "\nThe word was: " << hidden << endl;
		cout << "\nCongratulations: " << playerName << endl;
		readFile("win.txt");
		cout << "\nWould you like to play again? [Y/N]";
		cin >> start_new_game;
		if (start_new_game == "Y" || start_new_game == "y") {
			chooseGameType();
		}
		else {
			cout << "See you next time!!" << endl;
			exit(EXIT_SUCCESS);
		}
	}
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