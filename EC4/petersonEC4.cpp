// NAME.cpp
// MONTH/DAY/YEAR
// Joshua Peterson

#include <iostream>
#include <string>

using namespace std;


class Pstring : public string{
public:
	// Constructor, passes string to string constructor
	Pstring(const string& str)
	: string(str)
	{}

	bool isPalindrome()
	{
		if(this->size() == 0)
			return false;
		int size = this->size();
		bool isP = true;
		for(int i = 0; i < size; i++)
		{
			if((*this)[i] != (*this)[size-i-1])
				isP = false;
		}

		return isP;
	}
};

// Global Variables

// Function Prototypes
void ProgramGreeting();
void Unittest();


int main() {
	Unittest();
	cout << endl;
	ProgramGreeting();
	cout << endl;
	Pstring test1 = string("Hello World");
	Pstring test2 = string("a");
	Pstring test3 = string("racecar");
	Pstring test4 = string("cbbc");
	Pstring test5 = string("aabbcaa");
	Pstring test6 = string("");

	Pstring arr[] = {test1,test2,test3,test4,test5,test6};

	for(Pstring str: arr)
	{
		if(str.isPalindrome())
			cout << str << " is a palindrome." << endl;
		else
			cout << str << " is not a palindrome." << endl;
	}
}


void ProgramGreeting() {
	cout << "Welcome to " << "EC4, palindrome testing" << "." << endl;
	cout << "This program derives a class from STL string ";
	cout << "and adds a check to see if the string is a palindrome." << endl;
}

void Unittest() {
// All test code here
}
