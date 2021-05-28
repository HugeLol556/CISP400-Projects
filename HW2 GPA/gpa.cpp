// gpa.cpp
// 9/13/20
// Joshua Peterson

#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>

using namespace std;

// Specification A1 - Date class
class Date {
public:

	// Constructor
	Date(int month = -1, int day = -1, int year = -1){
		this->day = day;
		this->month = month;
		this->year = year;
	}

	// Returns a correctly formatted string using internal data
	string format(){
		string temp = to_string(month) + "/" + to_string(day);
		temp += "/" + to_string(year);
		return temp;
	}

	// Specification A3 - Component Test Method in Date
	void testData(){
		time_t t = time(0);
		tm* now = localtime(&t);
		int month = now->tm_mon + 1;
		int day = now->tm_mday;
		int year = now->tm_year + 1900;
		Date* date = new Date(month,day,year);
		cout << "Initializing Date object with values Day: " << day;
		cout << " Month: " << month << " Year: " << year << endl;
		cout << "Outputting data within Date object: Day: " << date->day;
		cout << " Month: " << date->month << " Year: " << date->year << endl;
		if(date->year == year && date->month == month && date->day == day){
			cout << "All data is correct." << endl;
		} else {
			cout << "Data is not correct." << endl;
		}

		// Deallocate object
		delete date;
		date = nullptr;
	}

	// Outputs various internal data
	int getDay(){
		return day;
	}
	int getMonth(){
		return month;
	}
	int getYear(){
		return year;
	}


private:
	int day;
	int month;
	int year;
};


// Function Prototypes
void ProgramGreeting();
void Unittest();
int getInt(int,int);
double getDouble(double,double);
void clrScreen();
void printGrades(const double* const, int);
char getLetter(double);
double calcGPA(const double* const, int);
double* addGrade(double*, int&, double);
void createHeader(string, char = '=');

int main() {
	ProgramGreeting();
	Unittest();

	cout << endl;

	// Initalizes dynamic array
	// Specification B1 - Dynamic Array
	double* scores = new double[0];
	int size = 0;

	int choice;
	double input;
	double gpa;

	bool exit = false;

	while(!exit)
	{
		// Menu
		createHeader("Menu");
		cout << "1. Add Grade" << endl;
		cout << "2. Display Grades" << endl;
		cout << "3. Calculate GPA" << endl;
		cout << "4. Quit" << endl;
		cout << endl;

		// Specification B3 - Menu Input Validation
		choice = getInt(1,4);

		clrScreen();
		switch(choice)
		{
			// Accepts a grade from user and adds it to array
			case 1:
				cout << "Please enter a new grade between 0 and 100." << endl;
				cout << "Grades are rounded to two decimal places when shown." << endl;
				cout << "Input: ";
				input = getDouble(0,100);
				//Specification B2 - Add Elements
				scores = addGrade(scores, size, input);
				break;
			// Displays all currently input grades and their letter grade
			case 2:
				// Specification C2 - Print Scores
				printGrades(scores, size);
				break;
			// Calculates and shows overall grade and letter grade
			case 3:
				// Specification C4 - Compute GPA
				gpa = calcGPA(scores, size);
				cout << "Total Grade: " << fixed << setprecision(2);
				// Specification B4 - Print Letter Grade
				cout << gpa << " - " << getLetter(gpa) << endl;
				break;
			// User wants to quit program
			case 4:
				exit = true;
				break;
			// This should never happen. Error occured
			default:
				cout << "Error in menu switch." << endl;
		}
		cout << endl;
	}

	cout << "Good Bye!" << endl;
	// Clean up array pointer
	delete[] scores;
	scores = nullptr;

	return 0;
}

// Specification C1 - Program Greeting Function
void ProgramGreeting() {
	// Get current date
	time_t t = time(0);
	tm* now = localtime(&t);
	// Specification A2 - External date initialization
	Date* date = new Date(now->tm_mon + 1,now->tm_mday,now->tm_year + 1900);
	cout << "Welcome to " << "Homework 2, GPA Calculator" << "." << endl;
	cout << "Input any number of grades, and display grades ";
	cout << "or calculate your GPA." << endl << endl;
	cout << "Made By: Joshua Peterson" << endl;
	cout << "Date: " << date->format() << endl;

	// Clean up Date object
	delete date;
	date = nullptr;
}

// Specification A4 - Component Test Grade Conversion
void Unittest() {
	cout << endl;
	cout << "Testing getLetter() function: Returns the corresponding letter grade ";
	cout << "from an inputted grade." << endl;
	cout << "getLetter(100): Expected output: A Output : " << getLetter(100) << endl;
	cout << "getLetter(90): Expected output: A Output : " << getLetter(90) << endl;
	cout << "getLetter(89.99): Expected output: B Output : " << getLetter(89.99) << endl;
	cout << "getLetter(80): Expected output: B Output : " << getLetter(80) << endl;
	cout << "getLetter(70): Expected output: C Output : " << getLetter(70) << endl;
	cout << "getLetter(60): Expected output: D Output : " << getLetter(60) << endl;
	cout << "getLetter(59.9): Expected output: F Output : " << getLetter(59.9) << endl;
	cout << "getLetter(0): Expected output: F Output : " << getLetter(0) << endl;
	cout << "getLetter(101): Expected output: N Output : " << getLetter(101) << endl;
	cout << "getLetter(-5): Expected output: N Output : " << getLetter(-5) << endl;
	cout << endl;

	cout << "Testing Date class with Date->testData()" << endl;
	Date* testingDate = new Date();
	testingDate->testData();
	delete testingDate;
	testingDate = nullptr;
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
			if(value >= min && value <= max)
	      badInput = false;
	    else
	      cout << "\033[1;31mError:\033[0m Not within bounds." << endl;
    } catch (const invalid_argument &){
      cout << "\033[1;31mError:\033[0m Input is not a number!";
      cout << endl;
    }
  }while(badInput);

	return value;
}

// Only accepts double input between lo and hi and returns it
double getDouble(double min, double max){
	double value;
	string input;
	bool badInput = true;

	do{
    cout << "Please enter an integer between " << min;
    cout << " and " << max << ": ";

    getline(cin, input);

    try{
      value = stod(input);
			if(value >= min && value <= max)
	      badInput = false;
	    else
	      cout << "\033[1;31mError:\033[0m Not within bounds." << endl;
    } catch (const invalid_argument &){
      cout << "\033[1;31mError:\033[0m Input is not a number!";
      cout << endl;
    }
  }while(badInput);

	return value;
}

// Clears terminal
void clrScreen(){
	cout << "\033[2J\033[1;1H";
}

// Prints each grade in array
void printGrades(const double* const scores, int size){
	for(int i = 0; i < size; i++)
	{
		cout << fixed << setprecision(2);
		// Specification C3 - Letter Grades
		cout << i + 1 << ": " << scores[i] << " - " << getLetter(scores[i]) << endl;
	}
}

// Converts number grade to letter grade.
char getLetter(double score){
	if(score > 100 || score < 0)
		return 'N';

	if(score >= 90)
		return 'A';
	else if (score >= 80)
		return 'B';
	else if (score >= 70)
		return 'C';
	else if (score >= 60)
		return 'D';
	else
		return 'F';
}

// Returns double calculated GPA value
double calcGPA(const double* const scores, int size){
	double total = 0;

	for(int i = 0; i < size; i++)
	{
		total += scores[i];
	}

	return total/static_cast<double>(size);
}

// Creates a new dynamic array one larger and adds the new grade to it
// Deallocates previous array and returns new array
double* addGrade(double* scores, int& size, double score){
	size++;
	double* temp = new double[size];

	for(int i = 0; i < size - 1; i++)
	{
		temp[i] = scores[i];
	}
	temp[size - 1] = score;


	delete[] scores;
	scores = nullptr;

	return temp;
}

// Accepts a string and prints said string to the console along
// with a line of inputted characters below it, defaulted to '='
void createHeader(string header, char character)
{
  cout << header << endl;
  cout << string(header.length(), character) << endl;
}
