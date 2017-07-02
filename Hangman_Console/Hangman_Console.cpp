// Hangman_Console.cpp : Defines the entry point for the console application. //

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
void startGame(int gameType);
void startGuess(string playerName, string assignedWord, string hidden, int failed_inputs, int char_exposed, int spaces_in_word, int gameType);
void startTwoPlayer(int gameType);
void chooseGameType();
void clearScreen();
void changeTextColor(int colorNumber);

int main()
{
	chooseGameType();
    return 0;
}

void changeTextColor(int colorNumber) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, colorNumber);
}

void clearScreen() {
	system("cls");
}

void chooseGameType() {
	clearScreen();
	changeTextColor(15);
	readFile("hangman_title.txt");
	int gameChoice; 
	cout << "Please enter which mode you would like to play";
	cout << "\n1. Single Player \n2. Two Players \n3. Exit" << endl;
	cout << "Enter number for game: ";
	cin >> gameChoice;

		while (gameChoice != 3) {
			switch (gameChoice) {
			case 1: 
				startGame(gameChoice);
				break;
			case 2: 
				startTwoPlayer(gameChoice);
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


void startGame(int gameType) {
	clearScreen();
	readFile("singlePlayer.txt");
	int failed_inputs = 7;
	int char_exposed = 0;
	int spaces_in_word = 0;


	string player_name;

	cout << "Enter Player Name: ";

	cin >> player_name;

	cout << "Player Name: " << player_name << endl;


	cout << "\nStarting Game......" << endl;

	srand(time(NULL));

	//Create a list of words in an array
	string words[] = { "Hello", "Quarter", "Champion", "Million", "Persona", "Giant", "Phantom", "Programming is Fun" };
	//string letters_guessed[] = {};
	string word_guessed = "";

	//Display a random word with * numbers of letters
	int random_index = rand() % sizeof(words) / sizeof(words[0]);

	string assignedWord = "";

	for (int i = 0; i < sizeof(words) / sizeof(words[0]); i++) {
		if (i == random_index) {
			assignedWord = words[random_index];
		}
	}

	string display = assignedWord;

	for (int i = 0; i < display.size(); i++) {
		if (display[i] == ' ') {
			spaces_in_word++;
			continue;
		}
		else {
			display[i] = '*';
		}
	}

	startGuess(player_name, assignedWord, display, failed_inputs, char_exposed, spaces_in_word, gameType);
}

void startTwoPlayer(int gameType) {
	clearScreen();
	cout << gameType;
	readFile("twoPlayers.txt");
	cout << "\nIn this mode, one player will display a secret word for the other to guess, If the other player cannot guess it they lose!!" << endl;
	cout << "\nStarting Two Player Game....." << endl;
	string player1;
	string player2;

	cout << "\nEnter Player 1's Name: ";
	cin >> player1;

	cout << "\nEnter Player 2's Name: ";
	cin >> player2;

	cout << "\nPlayers in this round: " << player1 << " & " << player2 << endl;

	string secret_word;
	int spaces_in_word = 0;

	cout << "\nPlayer 1 Please enter a word for Player 2 to guess: ";
	cin.ignore();
	getline(cin, secret_word);

	int failed_inputs;

	do {
		cout << "\nPlease enter amount of tries the user gets [Between 1 - 7]: ";
		cin >> failed_inputs;
		if (failed_inputs < 1 || failed_inputs > 7) {
			cout << "Sorry thats not valid, Please try again" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	} while (failed_inputs < 1 || failed_inputs > 7);


	clearScreen();

	cout << "Player 1 has given you " << failed_inputs << " tries" << endl;
	cout << "Your Word is: ";

	string display = secret_word;

	for (int i = 0; i < display.size(); i++) {
		if (display[i] == ' ') {
			spaces_in_word++;
			continue;
		}
		else {
			display[i] = '*';
		}
	}
	
	cout << display;
	cout << "\n";
	readFile("twoPlayers.txt");
	int char_exposed = 0;

	cout << "\nStarting Game......" << endl;

	srand(time(NULL));

	startGuess(player2, secret_word, display, failed_inputs, char_exposed, spaces_in_word, gameType);
}

void startGuess(string playerName, string word, string hidden, int failed_inputs, int char_exposed, int spaces_in_word, int gameType)
{
	string guess;
	string start_new_game;
	string gameTitle;

	if (gameType == 1) {
		gameTitle = "singlePlayer.txt";
	}
	else {
		gameTitle = "twoPlayers.txt";
	}

	while (char_exposed < word.length() - spaces_in_word) {
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
						startGuess(playerName, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType);
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
			startGuess(playerName, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType);
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
				readFile(gameTitle);
				readFile("showRLeg.txt");
				cout << "\nYou have one more chance left!!\n" << endl;
				break;
			case 2:
				clearScreen();
				readFile(gameTitle);
				readFile("showLLeg.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 3:
				clearScreen();
				readFile(gameTitle);
				readFile("showRArm.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 4:
				clearScreen();
				readFile(gameTitle);
				readFile("showLArm.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 5:
				clearScreen();
				readFile(gameTitle);
				readFile("showBody.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 6:
				clearScreen();
				readFile(gameTitle);
				readFile("showHead.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			default:
				cout << "Something has gone wrong.." << endl;
				break;
			}
		}
	}

	if (char_exposed == hidden.length() - spaces_in_word) {
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