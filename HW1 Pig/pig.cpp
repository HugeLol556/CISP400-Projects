// pig.cpp
// 9/6/20
// Joshua Peterson

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

// Function Prototypes
void ProgramGreeting();
void Unittest();
void Pig();
int D6();
int RandomNumber(int,int);
int pTurn(int);
int cTurn(int);
int getInt(int,int);
void clrScreen();

int main() {
	// Specification C1 - Fixed Seed
	// Define and then use a constant seed for srand()
	const int SRAND_SEED = 12345;
	srand(SRAND_SEED);

	ProgramGreeting();
	// Unittest();

	Pig();
}


void ProgramGreeting() {
	cout << "Welcome to " << "homework 1, Pig" << "." << endl;
	//Specification B2 - Display Due Date
	cout << "This assignment is due 9/6/20." << endl;
	cout << "This is a simple game where each player takes turns rolling a die.";
	cout << " Choose to either roll the dice, in which you lose your turn on a 1";
	cout << " or 2-6 is added to your turn total.";
	cout << " Hold to add your turn total to your score.";
	cout << " Firt person to reach 100 or more points wins." << endl << endl;
}

void Unittest() {
	cout << "Testing D6(): " << D6() << endl << endl;

	cout << "Testing RandomNumber(1,100): " << RandomNumber(1,100) << endl << endl;

	cout << "Testing Error RandomNumber(100,1): " << RandomNumber(100,1) << endl << endl;

	cout << "Testing Error RandomNumber(0,100): " << RandomNumber(0,100) << endl << endl;

	cout << "Testing Error RandomNumber(1,101): " << RandomNumber(1,101) << endl << endl;
}

// The Pig Game
void Pig(){
	// Specification B3 - Hi Score on Heap
	int* highScore = new int(0);
	int pScore = 0;
	int cScore = 0;
	int turnCount = 0;
	int turn = 1;
	string records = "";
	string summary = "";
	bool playing = true;

	// Specification C2 - Student Name
	string name;
	cout << "Please enter your name: ";
	getline(cin, name);

	cout << endl << "Good Luck!" << endl << endl;
	// Keep playing until someone wins
	while(pScore < 100 && cScore < 100 && playing)
	{
		turnCount++;
		summary = "";

		if(turn == 1)
		{
			turn = 0;
			// Player turn
			int val = pTurn(pScore);
			if(val == -2)
			{
				playing = false;
			}
			else
			{
				pScore += val;
				summary += "Turn " + to_string(turnCount) + ": Player scored " + to_string(val) + ".";
				summary += " Total scores - P: " + to_string(pScore) + " C: " + to_string(cScore) + "\n";
			}
		}
		else
		{
			turn = 1;
			// Computer turn
			int val = cTurn(cScore);
			cScore += val;
			summary += "Turn " + to_string(turnCount) + ": Computer scored " + to_string(val) + ".";
			summary += " Total scores - P: " + to_string(pScore) + " C: " + to_string(cScore) + "\n";
			cout << "The computer scored " << val << " points." << endl << endl;
		}

		if(pScore > *highScore)
			*highScore = pScore;
		if(cScore > *highScore)
			*highScore = cScore;

		cout << summary << endl << endl;
		records += summary;
	}

	cout << "Game Over! Press enter to see turn stats and results." << endl;
	string temp;
	getline(cin,temp);

	clrScreen();
	// Specification B1 - Display Turn Stats
	cout << records << endl;

	if(playing == false)
	{
		cout << "No contest." << endl;
	}
	else if(pScore >= 100)
	{
		cout << "Congrats, " << name << "! You win!" << endl;
	}
	else
	{
		cout << "The computer won. Better luck next time " << name << "." << endl;
	}

	cout << endl;

	// Specification B4 – Display High Score
	cout << "Highscore: " << *highScore << endl << endl;
	cout << "Good bye!" << endl;

	delete highScore;
	highScore = nullptr;
}

// Specification A1 - D6() function
// Returns a random value 1-6
int D6(){
	return RandomNumber(1,6);
}

// Specification A2 - RandomNumber() function
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

// Completes the players turn and returns score to be added
int pTurn(int currentScore){
	int turnScore = 0;
	int returnVal = -1;

	// Run until turn is skipped or finished
	while(returnVal == -1)
	{
		// Specification C3 - Numeric Menu
		// Build Menu
		cout << "Please choose an option by inputting its number." << endl;
		cout << "1. Roll" << endl;
		cout << "2. Hold" << endl;
		cout << "3. Quit" << endl;

		// Specification C4 - Bulletproof Menu
		//Get input
		int input = getInt(1,3);

		int num = 0;
		// Handle Decision
		switch(input)
		{
			// Roll
			case 1:
				clrScreen();
				num = D6();
				if(num == 1)
				{
					returnVal = 0;
					cout << "You rolled a 1 and your turn is skipped!" << endl << endl;
				}
				else
				{
					turnScore += num;
					cout << "You rolled " << num << ". ";
					cout << "Current turn score is: " << turnScore << endl;
					cout << "Current total score is: " << currentScore << endl << endl;
				}
				break;
			// Hold
			case 2:
				clrScreen();
				cout << "You held and added " << turnScore << " to your total." << endl;
				returnVal = turnScore;
				cout << endl;
				break;
			// Quit
			case 3:
				returnVal = -2;
				break;
			default:
				cout << "Error: You shouldn't see this default switch case.";
		}
	}

	return returnVal;
}

// Completes computer's turn and returns score to be added
int cTurn(int currentScore){
	int turnScore = 0;
	int returnVal = -1;

	// Run until the turn is skipped or finished
	while(returnVal == -1)
	{

		if(D6() <= 3)
		{
			// Hold
			returnVal = turnScore;
		}
		else
		{
			// Roll
			int roll = D6();

			if(roll == 1)
			{
				// Skip Turn
				returnVal = 0;
			}
			else
			{
				turnScore += roll;

				// Returns if the computer will win
				if(turnScore >= 100 - currentScore)
					returnVal = turnScore;
			}
		}
	}

	return returnVal;
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

void clrScreen(){
	cout << "\033[2J\033[1;1H";
}
