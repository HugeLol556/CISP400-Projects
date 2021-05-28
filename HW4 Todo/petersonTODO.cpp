// petersonTODO.cpp
// 11/1/20
// Joshua Peterson

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

// Dynamic array class for any data
// Requirements: using namepspace std and #include <iostream>
template <typename type>
class dArray{
public:
	// Default contructor
	dArray()
	{
		array = nullptr;
		size = 0;
		used = 0;
	}

	// Initalizes array of size but no data
	dArray(int size)
	{
		array = new type[size];
		this->size = size;
		used = 0;
	}

	// Initializes an array of size with default value data
	dArray(int size, type data)
	{
		// Do not create arrays with a size less than or equal to 0
		if(size <= 0)
		{
			cout << "\033[1;31mERROR: Size can't be less than or equal to 0.\033[0m" << endl;
			throw;
		}

		array = new type[size];
		this->size = size;
		used = size;

		for(int i = 0; i < size; i++)
		{
			array[i] = data;
		}
	}

	// Copy constructor
	dArray(const dArray<type> &arr)
	{
		int s = arr.len();
		array = new type[s];
		this->size = s;
		this->used = s;
		for(int i = 0; i < s;i++)
		{
			array[i] = arr[i];
		}
	}

	// Assignment overload
	dArray<type>& operator=(const dArray<type> &arr)
	{
		if(size > 0)
			delete[] array;
		int s = arr.len();
		array = new type[s];
		this->size = s;
		this->used = s;
		for(int i = 0; i < s;i++)
		{
			array[i] = arr[i];
		}

		return *this;
	}

	// Default destructor
	~dArray()
	{
		if(size > 0)
		{
			delete[] array;
		}
	}

	// Add data to end of array
	void add(type data)
	{
		if(used >= int(A_CHECK * size))
		{
			addMem();
		}
		used++;

		array[used - 1] = data;
	}

	// Deletes last element of array.
	// Does nothing if there is nothing to delete.
	void del()
	{
		if(used > 0)
		{
			used--;

			// Shorten allocated memory if used is low enough
			if(used <= int(D_CHECK * size))
			{
				delMem();
			}
		}
	}

	// Removes the item at index
	// by shifting all data over and then deleting last item
	void remove(int index)
	{
		// Out of bounds checking
		if(index >= used || index < 0)
		{
			cout << "\033[1;31mERROR: Index out of bounds.\033[0m" << endl;
			throw;
		}

		for(int i = index; i < used - 1; i++)
		{
			array[i] = array[i + 1];
		}

		del();
	}

	// Returns the index of the first item that equals the input
	// else returns -1
	int find(type item)
	{
		for(int i = 0; i < used; i++)
		{
			if(array[i] == item)
			{
				return i;
			}
		}

		return -1;
	}

	// Returns the amount of used array
	int len() const
	{
		return used;
	}

	// Returns data at index. Throws out of bound error
	type &operator[](const int i)
	{
		if(i >= used || i < 0)
		{
			cout << "\033[1;31mERROR: Index out of bounds.\033[0m" << endl;
			throw;
		}
		return array[i];
	}

	type operator[](const int i) const
	{
		if(i >= used || i < 0)
		{
			cout << "\033[1;31mERROR: Index out of bounds.\033[0m" << endl;
			throw;
		}
		return array[i];
	}

	// Class test function
	static void ComponentTest()
	{
		dArray<int> test1(50,99);
		dArray<int> test2;
		dArray<int>* test3 = new dArray<int>(1,1);
		dArray<int> test4(test1);
		dArray<int> test5 = *test3;
		dArray<int> test6(5,5);
		test6 = test1;
		dArray<int> test7(20);

		dArray<int> tests[] = {test1,test2,*test3, test4, test5, test6,test7};

		cout << "Testing dynamic array class" << endl;
		cout << "Testing initial inputs" << endl;
		if(test1.len() == 50 && test2.len() == 0 && test3->len() == 1)
			cout << "All initial sizes are correct." << endl;
		else
			cout << "\033[1;31mERROR: Wrong initial sizes.\033[0m" << endl;
		if(test1[0] == 99 && (*test3)[0] == 1)
			cout << "Initial values are correct." << endl;
		else
			cout << "\033[1;31mERROR: Wrong initial values.\033[0m" << endl;

		cout << "Testing input/output" << endl;
		bool good = true;
		for(dArray<int> test : tests)
		{
			int initialSize = test.len();
			for(int i = 0;i < 100; i++)
			{
				test.add(5);
			}

			for(int i = initialSize;i < 100; i++)
			{
				if(test[i] != 5){
					good = false;
					cout << "Failed test[i] = 5" << endl;
				}
			}

			if(test.len() != initialSize + 100){
				good = false;
				cout << "Failed size + 100" << endl;
			}

			test[10] = 1000;
			if(test[10] != 1000){
				good = false;
				cout << "Failed test[10] = 1000" << endl;
			}

			int tempSize = test.len();
			for(int i = 0;i < tempSize; i++)
			{
				test.del();
			}

			if(test.len() != 0){
				good = false;
				cout << "Failed len == 0" << endl;
			}

			for(int i = 0; i < 50; i++)
			{
				test.add(35);
			}

			if(test.len() != 50 || test[test.len() - 1] != 35){
				good = false;
				cout << "Failed len == 50 or final value is not 35" << endl;
			}

			test[1] = 40;
			test.remove(test.find(35));
			if(test[0] != 40)
			{
				cout << "Failed test[0] == 40 after remove" << endl;
				good = false;
			}

			for(int i = 0; i < 29; i++)
			{
				test.remove(0);
			}

			if(test.len() != 20)
			{
				cout << "Failed test.len() == 20 after many removes" << endl;
				good = false;
			}
		}

		if(good)
			cout << "All I/O tests passed" << endl;
		else
			cout << "\033[1;31mERROR: Failed I/O tests.\033[0m" << endl;

		delete test3;
	}

private:
	type* array;
	int size; // Size of allocated memory
	int used; // Amount of array actually being used
	// Multiplier used to increase array size. Multiplies current size by value
	double A_MULT = 1.5;
	// Fraction of size filled required to increase size of array
	double A_CHECK = 0.75;
	// Fraction of size filled required to decrease size of array
	double D_CHECK = 0.5;


	// Increases size of allocated memory
	void addMem()
	{
		// If size equals itself after multiplication, manually increase size
		if(int(size * A_MULT) == size)
		{
			size += 1;
		}
		else
		{
			size *= A_MULT;
		}

		// New array of new size
		type* temp = new type[size];

		// Copy over current contents to new array
		for(int i = 0; i < used; i++)
		{
			temp[i] = array[i];
		}

		// Assign current array to new array
		delete[] array;
		array = temp;
		temp = nullptr;
	}

	// Decreases size of allocated memory. Size will equal used
	void delMem()
	{
		size = used;

		// If array is empty, ignore temp array
		if(size == 0)
		{
			delete[] array;
			array = nullptr;
		}

		// New array of new size
		type* temp = new type[size];

		// Copy over current contents to new array
		for(int i = 0; i < used; i++)
		{
			temp[i] = array[i];
		}

		// Assign current array to new array
		delete[] array;
		array = temp;
		temp = nullptr;
	}
};

// Date class
// Requirements: using namepspace std
// #include <string>
// #include <iostream>
// #include <ctime>
class Date {
public:

	// Default constructor
	Date()
	{
		// Specification A3 - System Date.
		time_t t = time(0);
		tm* now = localtime(&t);
		month = now->tm_mon + 1;
		day = now->tm_mday;
		year = now->tm_year + 1900;
	}

	// Constructor
	Date(int month, int day, int year){
		if(month > 12 || month < 1){
			cout << "Incorrect month, defaulting to 1" << endl;
			this->month = 1;
		} else {
			this->month = month;
		}

		if(day > 31 || day < 1){
			cout << "Incorrect day, defaulting to 1" << endl;
			this->day = 1;
		} else {
			this->day = day;
		}

		if(year < 1){
			cout << "Incorrect year, defaulting to 1" << endl;
			this->year = 1;
		} else {
			this->year = year;
		}
	}

	// Returns a correctly formatted string using internal data
	string format(){
		string temp = to_string(month) + "/" + to_string(day);
		temp += "/" + to_string(year);
		return temp;
	}

	static void ComponentTest(){
		cout << "Testing Date Class" << endl;
		cout << "Testing input/output" << endl;
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
		cout << date->format() << endl;
		if(date->year == year && date->month == month && date->day == day){
			cout << "All data is correct." << endl;
		} else {
			cout << "Data is not correct." << endl;
		}

		Date date2;
		cout << date2.format() << endl;
		Date date3 = Date();
		cout << date3.format() << endl;

		cout << "Testing error handling" << endl;
		cout << "Object with incorrect month" << endl;
		Date iM(13,10,10);
		cout << "Incorect day" << endl;
		Date iD(10,32,10);
		cout << "Incorrect year" << endl;
		Date iY(10,10,-1);
		cout << "Incorrect everything" << endl;
		Date iE(-1,-1,-1);
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

class TODO{
friend ostream& operator<<(ostream& out, TODO& item);
friend istream& operator>>(istream& in, TODO& item);
public:
	// Default Constructor
	TODO()
	{
		message = "Empty";
		this->id = -1;
		theDate = Date();
	}

	// Specification A4 - Overload Constructor
	TODO( int id, string m = "Default message. Do something fun :)")
	{
		message = m;
		this->id = id;
		theDate = Date();
	}

	// Copy Constructor
	// Specification A1 - Overload Copy Constructor
	TODO(const TODO &c)
	{
		message = c.getMessage();
		theDate = c.getDate();
		id = c.getID();
	}

	// Assignment overload
	// Specification A2 - Overload Assignment Operator
	TODO &operator=(const TODO &c)
	{
		message = c.getMessage();
		theDate = c.getDate();
		id = c.getID();
		return *this;
	}

	// Equals comparison overload
	bool operator==(const TODO &c)
	{
		return id == c.getID();
	}

	string getMessage() const
	{
		return message;
	}

	Date getDate() const
	{
		return theDate;
	}

	int getID() const
	{
		return id;
	}

	static void ComponentTest()
	{
		TODO test1;
		TODO test2 = TODO();
		TODO test3(3, "Test 3");
		TODO test4 = TODO(4, "Test 4");
		TODO test5(5);
		TODO test6 = test5;
		cout << "Testing initial values" << endl;
		if(test1.getID() == -1 && test2.getID() == -1 &&test3.getID() == 3 &&
	test4.getID() == 4 &&test5.getID() == 5 && test1.getMessage() == "Empty"
&& test2.getMessage() == "Empty"&& test3.getMessage() == "Test 3"
&& test4.getMessage() == "Test 4"&& test5.getMessage() == "Default message. Do something fun :)")
		cout << "Initial values correct" << endl;
		else
		cout << "Initial values incorrect" << endl;
		// Specification C3 - Test TODO’s
		TODO arr[] = {test1,test2,test3,test4,test5,test6};
		cout << "Testing TODO I/O" << endl;
		bool success = true;
		for(TODO item: arr)
		{
			if(test1 == test2){
				;
			}
			else{
				success = false;
				cout << "Failed test1==test2" << endl;
			}

			item = test3;
			if(item==test3)
			{
				;
			}
			else{
				success = false;
				cout << "Item failed item==test3" << endl;
			}
		}

		if(success)
			cout << "I/O Passed" << endl;
		else
			cout << "I/O Failed" << endl;
	}
private:
	string message;
	Date theDate;
	int id;

};

// Specification C1 - Overload «
ostream& operator<<(ostream& out, TODO& item)
{
	out << "ID: " << item.id << endl;
	out << "Message: " << item.message << endl;
	out << "Date: " << item.theDate.format() << endl;
	return out;
}

//Specification C2 - Overload »
istream& operator>>(istream& in, TODO& item)
{
	string id;
	getline(in, id);
	if(id == "") // If empty line, error
		cout << "Error: Empty line when reading TODO input" << endl;
	item.id = stoi(id);
	getline(in, item.message);
	//in >> item.message;
	string date;
	getline(in, date);
	//in >> date;
	size_t s1 = date.find("/"); // Position of first slash
	size_t s2 = date.find("/", s1 + 1); // Position of second slash
	// Find values between slashes, then convert to ints
	string month = date.substr(0,s1);
	string day = date.substr(s1 + 1,s2 - s1 - 1);
	string year = date.substr(s2 + 1);
	item.theDate = Date(stoi(month),stoi(day),stoi(year));
	return in;
}

// Function Prototypes
void ProgramGreeting();
void Unittest();
void parseInput(char&, string&);
void clrScreen();

int main() {
	Unittest();
	cout << endl;
	ProgramGreeting();
	cout << endl;

	//Specification C4 - TODO array
	dArray<TODO> list;
	int count = 0;
	char choice;
	string input;
	int id;
	int index;
	string file = "TODO.txt";

	cout << "Use this program in the format of command [space] parameter." << endl;
	cout << "Here are the possible commands and if they accept parameters." << endl;
	cout << "Add a TODO list item with-" << endl;
	cout << "+ [Any message you want]" << endl;
	cout << "Delete a TODO list item with-" << endl;
	cout << "- [Number representing item's ID]" << endl;
	cout << "Print all TODO list items" << endl;
	cout << "? *No parameter*" << endl;
	cout << endl;

	cout << "Checking for input file TODO.txt" << endl;
	ifstream inputFile;
	inputFile.open(file);
	if(inputFile.is_open())
	{
		int i = 0;
		while(!inputFile.eof()) // While not end of file
		{
			inputFile.get(); // Flips end of file marker
			if(!inputFile.eof()) // Extra check
			{
				inputFile.unget(); // Unget the end of file check
				list.add(TODO());
				inputFile >> list[i];
				i++;
			}
		}
		inputFile.close();
		cout << "Successfully loaded data from TODO.txt" << endl;
	}
	else
	{
		cout << "No input file found. ";
		cout << "TODO.txt will be created at the end of the program." << endl;
	}



	do{
		parseInput(choice, input);

		clrScreen();

		switch(choice)
		{
			// Specification B1 - + Symbol
			case '+':
				count++;
				if(input == "") // If no input, use dummy entry
				{
					list.add(TODO(count));
				}
				else
				{
					list.add(TODO(count, input));
				}
				break;
			// Specification B2 - ? Symbol
			case '?':
				for(int i = 0; i < list.len(); i++)
				{
					cout << list[i];
					cout << string(20, '=') << endl;
				}
				break;
			// Specification B3 - - symbol
			case '-':
				id = stoi(input);
				index = list.find(TODO(id));
				if(index != -1)
					list.remove(index);
				break;
			case 'X':
				break;
			default:
				cout << "Error in switch." << endl;

		}
	} while(choice != 'X');

	cout << "Writing TODO list to TODO.txt" << endl;

	// Specification B4 - Persistence
	ofstream outputFile(file); // Attempts to open file and/or create one
	if(outputFile.is_open())
	{
		for(int i = 0; i < list.len();i++)
		{
			outputFile << list[i].getID() << endl;
			outputFile << list[i].getMessage() << endl;
			outputFile << list[i].getDate().format();
			if(i != list.len() - 1) // While not last entry
				outputFile << endl;
		}
		cout << "Writing successful. Good bye!" << endl;
	}
	else
	{
		cout << "Failed to write to file. Good bye!" << endl;
	}
}


void ProgramGreeting() {
	cout << "Welcome to " << "HW4, TODO list" << "." << endl;
	cout << "This is a command line program for creating and viewing ";
	cout << "a list of TODO messages." << endl;
}

void Unittest() {
	dArray<int>::ComponentTest();
	cout << endl;
	Date::ComponentTest();
	cout << endl;
	TODO::ComponentTest();
}

void parseInput(char& command, string& data)
{
	bool badInput = true;
	string input;
	do{
		data = ""; // DData is nothing for default


		getline(cin, input);

		if(input.length() == 0)
		{
			cout << "Error: No input." << endl;
			continue;
		}

		if(input[0] != '+' && input[0] != '?' && input[0] != '-' && input[0] != 'X')
		{
			cout << "Error: Command must be +, -, ?, or X to exit." << endl;
			continue;
		}

		if(input.length() >= 3) // If there is enough input for both parameters
		{
			if(input[1] == ' ') // If there is a space between both parameters
			{
				command = input[0];
				data = input.substr(2);
			}
			else
			{
				cout << "Error: Needs to be a space between command and parameter." << endl;
				continue;
			}
		}

		if(input.length() == 2) // Incorrect format
		{
			cout << "Error: Incorrect format. Ex: Command [space] parameter" << endl;
			continue;
		}
		else // Else length must be 1, so use it for command
		{
			command = input[0];
		}

		// Check that command has correct parameter
		if(command == '?')
		{
			if(data != "")
			{
				cout << "Error: ? Command requires no parameter." << endl;
				continue;
			}
		}

		if(command == '-')
		{
			if(data != "") // If there is data
			{
				try{
		      stoi(data);
		    } catch (const invalid_argument &){
		      cout << "Error: Parameter must be an integer." << endl;
					continue;
		    }
			}
			else
			{
				cout << "Error: - command requires integer parameter." << endl;
				continue;
			}
		}

		badInput = false; // If everything passes then input is good
	} while(badInput);
}

// Clears terminal
void clrScreen(){
	cout << "\033[2J\033[1;1H";
}
