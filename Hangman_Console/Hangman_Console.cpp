// Hangman_Console.cpp : Defines the entry point for the console application. //
//Words.txt sourced from https://github.com/Xethron/Hangman/blob/master/words.txt // 

#include "stdafx.h"
#include <iostream>
#include <string>
#include <string.h>
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
void startGuess(string playerName, vector<char> letters_guessed, string assignedWord, string hidden, int failed_inputs, int char_exposed, int spaces_in_word, int gameType, int hints);
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
bool strcasecmp(string compare1, string compare2);
void useHint(string playerName, vector<char> letters_guessed, string word, string hidden, int failed_inputs, int char_exposed, int spaces_in_word, int gameType, int hints);
int checkSpecialCharacter(string word);

int main()
{
	displayFullScreen();
	chooseGameType();
    return 0;
}

bool strcasecmp(string compare1, string compare2) {
	string lowerString1;
	string lowerString2;
	for (int i = 0; i < compare1.length(); i++) {
		lowerString1 = tolower(compare1[i]);
	}

	for (int i = 0; i < compare2.length(); i++) {
		lowerString2 = tolower(compare2[i]);
	}

	if (lowerString1 == lowerString2) {
		return true;
	}
	else {
		return false;
	}
}

void displayFullScreen() {
	//For Full Screen: SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

int checkSpecialCharacter(string word) {
	int specialCharacters = 0;
	for (int i = 0; i < word.length(); i++) {
		if (!isalnum(word[i])) {
			specialCharacters++;
		}
		else {
			continue;
		}
	}
	return specialCharacters;
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
			changeTextColor(3);
			cout << readLine << endl;
		}
	}
	else {
		cout << "Cannot open file" << endl;
	}
	changeTextColor(7);
}

void chooseGameType() {
	clearScreen();
	readFile("hangman_title.txt");
	readFile("showRLeg.txt");
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
	int failed_inputs = 9;
	int char_exposed = 0;
	int spaces_in_word = 0;
	int hints = 3;


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
	int random_index = 0;
	if (words.size() > 0) {
		random_index = rand() % words.size();
	}
	else {
		displayErrorMessage("No words were found...Words.txt was not found\n");
		Sleep(1500);
		chooseGameType();
	}

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
			display[i] = '_';
		}
	}

	startGuess(player_name, letters_guessed, assignedWord, display, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
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
	int hints = 3;

	do {
		cout << "\n" << player1 << " please enter a word for " << player2 << " to guess: ";
		getline(cin, secret_word);
		for(int i = 0; i < secret_word.length(); i++){
			if (isdigit(secret_word[i])) {
				displayErrorMessage("No Numbers are allowed in the word\n");
				Sleep(0700);
				break;
			}
			else if (secret_word == "") {
				displayErrorMessage("Word Cannot be Empty\n");
				Sleep(0700);
				break;
			}
			else if (!isalnum(secret_word[i])) {
				displayErrorMessage("No Special Characters are allowed\n");
				Sleep(0700);
				break;
			}
		}
		
	} while (any_of(secret_word.begin(), secret_word.end(), isdigit) || secret_word == "" || checkSpecialCharacter(secret_word) > 0);

	do {
		cout << "\nPlease enter amount of tries " << player2 << "gets [Between 1 - 9]: ";
		cin >> failed_inputs;
		if (failed_inputs < 1 || failed_inputs > 9) {
			displayErrorMessage("Sorry thats not valid, Please try again\n");
			removeStringBuffer();
			continue;
		}
	} while (failed_inputs < 1 || failed_inputs > 9);


	clearScreen();

	string display = secret_word;

	for (int i = 0; i < display.size(); i++) {
		if (display[i] == ' ') {
			spaces_in_word++;
			continue;
		}
		else {
			display[i] = '_';
		}
	}
	
	readFile("twoPlayers.txt");
	cout << player1 << " has given you " << failed_inputs << " tries" << endl;
	cout << "Your Word is: ";
	cout << display;
	cout << "\nContains: " << secret_word.length() << " Characters";
	cout << "\n";
	int char_exposed = 0;

	cout << "\nStarting Game......" << endl;

	srand(time(NULL));

	//Create a string vector to hold list of letters guessed
	vector<char> letters_guessed;

	startGuess(player2, letters_guessed, secret_word, display, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
}

void startGuess(string playerName, vector<char> letters_guessed, string word, string hidden, int failed_inputs, int char_exposed, int spaces_in_word, int gameType, int hints)
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
		if (gameType == 1) {
			Sleep(1000);
			clearScreen();
			readFile("singlePlayer.txt");
		}
		else {
			Sleep(1000);
			clearScreen();
			readFile("twoPlayers.txt");
		}

		bool correct_guess = false;
		//Begin Letting player guess the character one by one
		cout << "\n\nPlease enter a character to guess in: " << hidden;
		cout << "\nLetters guessed so far: ";
		displayLettersGuessed(letters_guessed);
		cout << "\nEnter [HELP] for a Hint: [You have: " << hints << " Left]";
		cout << "\nEnter Letter: ";

		cin >> guess;

		if (guess.length() == 1) {
			if (!isalpha(guess[0]) && !isdigit(guess[0])) {
				displayErrorMessage("No Special Characters are allowed");
				startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
			}
			//Check if the letter the user guessed is in the vector already, if not add it
			if (find(letters_guessed.begin(), letters_guessed.end(), tolower(guess[0])) != letters_guessed.end()) {
				displayErrorMessage("Youve Already Guessed This Letter");
				startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
			}
			else if (isdigit(guess[0])) {
				displayErrorMessage("Cant Guess Numbers");
				startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
			}
			else {
				letters_guessed.push_back(tolower(guess[0]));

				//If guess was correct, reveal a letter in the array
				for (int i = 0; i < word.length(); i++) {
					if (tolower(guess[0]) == tolower(word[i])) {
						if (hidden[i] == word[i]) {
							displayErrorMessage("Youve Already Guessed This Letter");
							startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
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
		else if (guess.length() > 1 && strcasecmp(guess, "help") == false){
			displayErrorMessage("Enter One Letter Only");
			startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
		}
		else if (guess.length() > 1 && strcasecmp(guess, "help") == true) {
			if (hints > 0){
				hints = hints - 1;
				useHint(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
			}
			else {
				displayErrorMessage("You have no hints left\n");
				startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
			}
		}

		if (correct_guess == false) {
			failed_inputs--;
			switch (failed_inputs) {
			case 0:
				clearScreen();
				readFile(gameTitle);
				readFile("showRLeg.txt");
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
				readFile("showLLeg.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				cout << "\nYou have one more chance left!!\n" << endl;
				break;
			case 2:
				clearScreen();
				readFile(gameTitle);
				readFile("showRArm.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 3:
				clearScreen();
				readFile(gameTitle);
				readFile("showLArm.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 4:
				clearScreen();
				readFile(gameTitle);
				readFile("showBody.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 5:
				clearScreen();
				readFile(gameTitle);
				readFile("showHead.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 6:
				clearScreen();
				readFile(gameTitle);
				readFile("showRope.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 7:
				clearScreen();
				readFile(gameTitle);
				readFile("showStandTop.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			case 8:
				clearScreen();
				readFile(gameTitle);
				readFile("showStand.txt");
				cout << "You have: " << failed_inputs << " tries left" << endl;
				break;
			default:
				cout << "Something has gone wrong.." << endl;
				break;
			}
		}
	}

	if (char_exposed == hidden.length() - spaces_in_word && hints > 0) {
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
	else if (char_exposed == hidden.length() - spaces_in_word && hints == 0) {
		cout << "\nThe word was: " << hidden << endl;
		cout << "\nTry not to use hints next time!: " << playerName << endl;
		readFile("gameOver.txt");
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

void useHint(string playerName, vector<char> letters_guessed, string word, string hidden, int failed_inputs, int char_exposed, int spaces_in_word, int gameType, int hints) {
	if (hints < 0) {
		displayErrorMessage("You have no more hints\n");
		startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
	}
	else {
		int random_index = rand() % word.size();
		if (hidden[random_index] == '_') {
			for (int i = 0; i < word.length(); i++) {
				if (word[random_index] == word[i]) {
					hidden[i] = word[i];
					char_exposed++;
				}
				else {
					continue;
				}
			}
			displaySuccessMessage("Hint Given\n");
			startGuess(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
		}
		else {
			useHint(playerName, letters_guessed, word, hidden, failed_inputs, char_exposed, spaces_in_word, gameType, hints);
		}
	}
}