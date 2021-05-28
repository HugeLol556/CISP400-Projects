// petersonRPS.cpp
// 9/6/20
// Joshua Peterson

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;


// Function Prototypes
void ProgramGreeting();
void Unittest();
int getInt(int,int);
void clrScreen();
int RandomNumber(int,int);

int main() {
	// Constant value for srand
	const int SRAND_VAL = 12345;
	srand(SRAND_VAL);

	ProgramGreeting();
	Unittest();

	int cScore = 0;
	int pScore = 0;
	int plays = 0;
	bool playing = true;
	int choice;
	int cChoice;

	do{
		// Always add 1 to plays
		plays++;

		cout << endl;

		// Reset player choice
		choice = 0;

		// Menu
		cout << "Please choose an option by inputting its number." << endl;
		cout << "1. Rock" << endl;
		cout << "2. Paper" << endl;
		cout << "3. Scissors" << endl;
		cout << "4. Quit" << endl;
		cout << endl;
		// Player choice
		choice = getInt(1,4);

		// Computer choice
		cChoice = RandomNumber(1,3);

		cout << endl;
		switch(cChoice){
			case 1:
				cout << "Computer chose rock." << endl;
				break;
			case 2:
				cout << "Computer chose paper." << endl;
				break;
			case 3:
				cout << "Computer chose scissors." << endl;
				break;
		}

		cout << endl;
		if(cChoice == choice)
			cout << "It was a tie" << endl;

		switch(choice){
			case 1:
				if(cChoice == 2){
					cout << "You lost." << endl;
					cScore++;
				}
				else if(cChoice == 3){
					cout << "You won." << endl;
					pScore++;
				}
				break;
			case 2:
				if(cChoice == 3){
					cout << "You lost." << endl;
					cScore++;
				}
				else if(cChoice == 1){
					cout << "You won." << endl;
					pScore++;
				}
				break;
			case 3:
				if(cChoice == 1){
					cout << "You lost." << endl;
					cScore++;
				}
				else if(cChoice == 2){
					cout << "You won." << endl;
					pScore++;
				}
				break;
			case 4:
				playing = false;
				break;
		}
	}while(playing);

	// Last play didn't happen becuase player quit
	plays--;

	cout << endl;
	cout << "In " << plays <<" games, ";
	cout << "you won " << pScore << " times and the computer ";
	cout << "won " << cScore << " times." << endl;
	cout << endl;

	if(pScore > cScore)
		cout << "Congrats! You won!" << endl;
	else if(cScore == pScore)
		cout << "It was a tie." << endl;
	else
		cout << "You lost. Better luck next time." << endl;
}


void ProgramGreeting() {
	cout << "Welcome to " << "extra credit 1, Rock Paper Scissors" << "." << endl;
	cout << "Choose a move and see how many times you can win." << endl << endl;
}

void Unittest() {
// All test code here
}

// Only accepts integer input between lo and hi and returns it
int getInt(int min, int max){
	int value;
	string input;
	bool badInput = true;

	do{
    cout << "Please enter an integer between " << min;
    cout << " and " << max << ": ";

    getline(cin, input);

    try{
      value = stoi(input);
    } catch (const invalid_argument &){
      cout << "\033[1;31mError:\033[0m Input is not a number!";
      cout << endl << endl;
    }

    if(value >= min && value <= max)
      badInput = false;
    else
      cout << "\033[1;31mError:\033[0m Not within bounds." << endl;

  }while(badInput);

	return value;
}

// Clears terminal
void clrScreen(){
	cout << "\033[2J\033[1;1H";
}

// Returns a random value between lo and hi given that lo is not less
// than 1 and hi is not greater than 100 and hi is greater than lo
int RandomNumber(int lo, int hi){
	// Specification A3 - Protect RandomNumber() input
	if(hi < lo || lo < 1 || hi > 100)
		return -1;

	int randNum = rand() % hi + lo;

	// Specification A4 - Protect RandomNumber() output
	if(randNum > 100)
		return -2;

	return randNum;
}
