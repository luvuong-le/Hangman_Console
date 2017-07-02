// Hangman_Console.cpp : Defines the entry point for the console application. //
//Words.txt sourced from https://github.com/Xethron/Hangman/blob/master/words.txt // 

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include <algorithm>
#include <limits.h>
#include <vector>
#undef min
#undef max

using namespace std;

void readFile(string filename);
void startGame(int gameType);
void startGuess(string playerName, vector<char> letters_guessed, string assignedWord, string hidden, int failed_inputs, int char_exposed, int spaces_in_word, int gameType);
void startTwoPlayer(int gameType);
void chooseGameType();
void clearScreen();
void changeTextColor(int colorNumber);
vector<string> loadWords(string filename);
void displayLettersGuessed(vector<char> letters_guessed);
void removeStringBuffer();
void displayErrorMessage(string message);
void displaySuccessMessage(string message);
void displayFullScreen();

int main()
{
	displayFullScreen();
	chooseGameType();
    return 0;
}

void displayFullScreen() {
	//For Full Screen: SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

void displayErrorMessage(string message) {
	changeTextColor(004);
	cout << "ERROR: " << message;
	changeTextColor(15);
}

void displaySuccessMessage(string message) {
	changeTextColor(005);
	cout << "SUCCESS: " << message;
	changeTextColor(15);
}

void removeStringBuffer() {
	cin.clear();
	cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
	Sleep(0700);
}

void changeTextColor(int colorNumber) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, colorNumber);
}

void clearScreen() {
	system("cls");
}

vector<string> loadWords(string filename) {
	//Create a vector to hold the vector of words
	vector<string> words;
	string readLine;
	ifstream myfile(filename.c_str());

	if (myfile.is_open()) {
		while (getline(myfile, readLine)) {
			words.push_back(readLine);
		}
	}
	return words;
}

void displayLettersGuessed(vector<char> letters_guessed) {
	for (int i = 0; i < letters_guessed.size(); i++) {
		cout << " " << letters_guessed[i];
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

void chooseGameType() {
	clearScreen();
	changeTextColor(15);
	readFile("hangman_title.txt");
	int gameChoice = 0; 
	cout << "Please enter which mode you would like to play";
	cout << "\n1. Single Player \n2. Two Players \n3. Exit" << endl;
	cout << "Enter number for game: ";
	cin >> gameChoice;
		while (gameChoice != 3) {
			switch (gameChoice) {
			case 1: 
				removeStringBuffer();
				startGame(gameChoice);
				break;
			case 2: 
				removeStringBuffer();
				startTwoPlayer(gameChoice);
				break;
			case 3:
				exit(EXIT_SUCCESS);
				break;
			default:
				displayErrorMessage("\nInvalid Input, You must enter a number");
				removeStringBuffer();
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

	getline(cin, player_name);

	cout << "Player Name: " << player_name << endl;


	cout << "\nStarting Game......" << endl;

	srand(time(NULL));

	//Create a list of words in an vector
	//C++ Doesnt allow additions to arrays
	vector<string> words = loadWords("words.txt");

	//Create a string vector to hold list of letters guessed
	vector<char> letters_guessed;

	//Display a random word with * numbers of letters
	int random_index = rand() % words.size();

	string assignedWord = "";

	for (int i = 0; i < words.size(); i++) {
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

	startGuess(player_name, letters_guessed, assignedWord, display, failed_inputs, char_exposed, spaces_in_word, gameType);
}

void startTwoPlayer(int gameType) {
	clearScreen();
	readFile("twoPlayers.txt");
	cout << "\nIn this mode, one player will display a secret word for the other to guess, If the other player cannot guess it they lose!!" << endl;
	cout << "\nStarting Two Player Game....." << endl;
	string player1;
	string player2;


	do {
		cout << "\nEnter Player 1's Name: ";
		getline(cin, player1);

		cout << "\nEnter Player 2's Name: ";
		getline(cin, player2);

		if (player1 == "" || player2 == ""){
			displayErrorMessage("\nBoth Players must have a name");
			Sleep(0700);
			startTwoPlayer(gameType);
		}

	} while (player1 == "" || player2 == "");


	cout << "\nPlayers in this round: " << player1 << " & " << player2 << endl;

	string secret_word = "";
	int spaces_in_word = 0;
	int failed_inputs = 0;

	do {
		cout << "\nPlayer 1 Please enter a word for Player 2 to guess: ";
		getline(cin, secret_word);
		for(int i = 0; i < secret_word.length(); i++){
			if (isdigit(secret_word[i])) {
				displayErrorMessage("No Numbers are allowed in the word\n");
				Sleep(0700);
			}
			else if (secret_word == "") {
				displayErrorMessage("\nWord Cannot be Empty");
				Sleep(0700);
			}
		}
		
	} while (any_of(secret_word.begin(), secret_word.end(), isdigit) || secret_word == "");

	do {
		cout << "\nPlease enter amount of tries the user gets [Between 1 - 7]: ";
		cin >> failed_inputs;
		if (failed_inputs < 1 || failed_inputs > 7) {
			displayErrorMessage("Sorry thats not valid, Please try again\n");
			removeStringBuffer();
			continue;
		}
	} while (failed_inputs < 1 || failed_inputs > 7);


	clearScreen();

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
	
	readFile("twoPlayers.txt");
	cout << "Player 1 has given you " << failed_inputs << " tries" << endl;
	cout << "Your Word is: ";
	cout << display;
	cout << "\n";
	int char_exposed = 0;

	cout << "\nStarting Game......" << endl;

	srand(time(NULL));

	//Create a string vector to hold list of letters guessed
	vector<char> letters_guessed;

	startGuess(player2, letters_guessed, secret_word, display, failed_inputs, char_exposed, spaces_in_word, gameType);
}

void startGuess(string playerName, vector<char> letters_guessed, string word, string hidden, int failed_inputs, int char_exposed, int spaces_in_word, int gameType)
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
		cout << "\nLetters guessed so far: ";
		displayLettersGuessed(letters_guessed);
		cout << "\nEnter Letter: ";

		cin >> guess;

		if (guess.length() == 1) {
			//Check if the letter the user guessed is in the vector already, if not add it
			if (find(letters_guessed.begin(), letters_guessed.end(), tolower(guess[0])) != letters_guessed.end()) {
				displayErrorMessage("Youve Already Guessed This Letter");
				startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType);
			}
			else {
				letters_guessed.push_back(tolower(guess[0]));

				//If guess was correct, reveal a letter in the array
				for (int i = 0; i < word.length(); i++) {
					if (tolower(guess[0]) == tolower(word[i])) {
						if (hidden[i] == word[i]) {
							displayErrorMessage("Youve Already Guessed This Letter");
							startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType);
						}
						else {
							//If letter matches, display the hidden letter
							hidden[i] = word[i];
							//Logic to display one less star if character guessed is correct
							displaySuccessMessage(">>> You guessed right!");
							cout << " {" << word[i] << "}\n";
							char_exposed++;
							correct_guess = true;
							continue;
						}
					}
				}
			}
		}
		else if(guess.length() == 1){
			displayErrorMessage("Youve Already Guessed This Letter");
			startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType);
		}

		if (correct_guess == false) {
			failed_inputs--;
			switch (failed_inputs) {
			case 0:
				readFile("gameOver.txt");
				cout << "You have run out of tries" << endl;
				cout << "The word was: " << word << endl;
				cout << "Better luck next time!" << endl;
				cout << "\nWould you like to play again? [Y/N]: " << endl;
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
		cout << "\nWould you like to play again? [Y/N]: ";
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
