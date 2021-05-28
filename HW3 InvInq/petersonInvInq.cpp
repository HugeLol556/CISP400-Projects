// petersonInvInq.cpp
// 10/3/20
// Joshua Peterson

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Specification B1 - Date class
class Date {
public:

	// Constructor
	Date(int month = 1, int day = 1, int year = 1){
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

	//Specification B2 - ComponentTest method in Date
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

struct Item{
	int ID;
	int quantity;
	int wholesaleCost;
	int retailCost;
	Date* date;

	friend istream& operator >> (istream& in, Item& i);
};

class Inventory
{
	friend ostream& operator << (ostream& out, const Inventory& i);
	friend istream& operator >> (istream& in, Item& i);
  public:
    // Constructor
    Inventory()
    {
			// Specification C2 - Dynamic Array
      inventory = new Item[0];
      size = 0;
    }

    // Destructor
    ~Inventory(){
      for(int i = 0; i < size; i++)
      {
        delete inventory[i].date;
      }
    }

		void tAdd();
    void add();
    void pop();
    void edit();
    void display();
		static void ComponentTest();

  private:
	  Item* inventory;
	  int size;

	  static int getID();
	  static int getQuantity();
	  static int getWholeCost();
		static int getInt(int,int);
		void createHeader(string, char = '=');

};

// Add a single item to the end of the inventory
  void Inventory::add()
  {
			// Specification C3 - Resize Array - add to end
      Item* temp = new Item[size + 1];

      for(int i = 0; i < size; i++)
      {
        temp[i] = inventory[i];
      }

      cout << "Adding new item to inventory." << endl;
      cout << "Please provide accurate information for all inputs." << endl;
      cout << "ID is 5 digits." << endl;
      cout << "Both quantity and wholesale cost cannot be negative "; cout << "and have a max of 1 billion." << endl;
      cout << endl;

      // Get the new item
      Item tempItem = {};
			cin >> tempItem;

      createHeader("New Item");
      cout << "ID: " << tempItem.ID << endl;
      cout << "Quantity: " << tempItem.quantity << endl;
      cout << "Wholesale Cost: " << tempItem.wholesaleCost << endl;
      cout << "Retail Cost: " << tempItem.retailCost << endl;
      cout << "Date: " << tempItem.date->format() << endl;
      cout << endl;

      // Add new item to end of inventory
      temp[size] = tempItem;
      size++;

			// Cleanup
      delete[] inventory;

      inventory = temp;
      temp = nullptr;
    }

	// Adds dummy data to the inventory or testing
	void Inventory::tAdd()
	{
		Item* temp = new Item[size + 1];

		for(int i = 0; i < size; i++)
		{
			temp[i] = inventory[i];
		}

		Item tempItem = {size + 1,(size + 1) * 10 ,(size + 1) * 100, (size + 1) * 200, new Date(9,9,99)};

		temp[size] = tempItem;
		size++;

		delete[] inventory;

		inventory = temp;
		temp = nullptr;
	}

    // Remove the last item in the inventory
  void Inventory::pop()
  {
			if(size == 0)
				{
					cout << "There is no data to delete!" << endl;
					return;
				}

			// Specification C4 - Resize Array - subtract from end
      Item* temp = new Item[size - 1];

      for(int i = 0; i < size - 1; i++)
      {
        temp[i] = inventory[i];
      }

      size--;

			// Cleanup the struct that we are deleting
			delete inventory[size].date;
      delete[] inventory;

      inventory = temp;
      temp = nullptr;

			cout << "Successfully deleted the last item in the inventory." << endl;
    }

  // Edit any values of any item in the inventory
	// Specification A1 - Edit Inventory
  void Inventory::edit()
  {
			if(size == 0)
				{
					cout << "There is no data to edit!" << endl;
					return;
				}

      cout << "Please choose the index value of the item you wish to change: " << endl;
      int index = getInt(0,size - 1);
      cout << endl;

      // Edit menu
      createHeader("Editable Values");
      cout << "1. Edit ID (Must be 5 digits)" << endl;
      cout << "2. Edit Quantity (Between 1 and 1 billion)" << endl;
      cout << "3. Edit Wholesale Cost (Between 1 and 1 billion)" << endl;
      cout << endl << "Please select an option: ";

      int choice;
      choice = getInt(1,3);

      int before;
      int after;

      switch(choice)
      {
        case 1:
          before = inventory[index].ID;
          after = getID();
          inventory[index].ID = after;
          cout << "Item's ID changed from ";
					cout << setfill('0') << setw(5) << before << " to ";
          cout << setfill('0') << setw(5) << after << "." << endl;
          break;
        case 2:
          before = inventory[index].quantity;
          after = getQuantity();
          inventory[index].quantity = after;
          cout << "Item's quantity changed from " << before << " to ";
          cout << after << "." << endl;
          break;
        case 3:
          before = inventory[index].wholesaleCost;
          after = getWholeCost();
          inventory[index].wholesaleCost = after;
          cout << "Item's wholesale cost changed from " << before << " to ";
          cout << after << "." << endl;
          break;
        default:
          cout << "Error in edit values switch" << endl;
      }
    }

  void Inventory::display()
  {
			if(size == 0)
			{
				cout << "No data to display!" << endl;
				return;
			}

      for(int i = 0; i < size; i++)
      {
        cout << "Item " << i + 1 << endl;
        cout << string(20, '=') << endl;
        cout << "ID: " << setfill('0') << setw(5) << inventory[i].ID << endl;
        cout << "Quantity: " << inventory[i].quantity << endl;
        cout << "Wholesale Cost: " << inventory[i].wholesaleCost << endl;
        cout << "Retail Cost: " << inventory[i].retailCost << endl;
        cout << "Date: " << inventory[i].date->format() << endl;
        cout << endl;
      }
    }

	// Returns a valid ID
	// Specification B4 - Inventory Entry Input Validation
  int Inventory::getID()
  {
		return getInt(0,99999);
  }

  // Returns a valid quantity
	// Specification B4 - Inventory Entry Input Validation
  int Inventory::getQuantity()
  {
    return getInt(0, 1000000000);
  }

  // Returns a valid wholesale Cost
	// Specification B4 - Inventory Entry Input Validation
  int Inventory::getWholeCost()
  {
    return getInt(0, 1000000000);
  }

	// Accepts a string and prints said string to the console along
	// with a line of inputted characters below it, defaulted to '='
	void Inventory::createHeader(string header, char character)
	{
	  cout << header << endl;
	  cout << string(header.length(), character) << endl;
	}

	// Only accepts integer input between lo and hi, inclusive, and returns it
	int Inventory::getInt(int min, int max){
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

	void Inventory::ComponentTest()
	{
		Inventory inv;

		cout << "Testing inventory delete and display" << endl;
		cout << "No data" << endl;
		inv.display();
		inv.pop();
		cout << "Adding 1 item and displaying then deleting" << endl;
		inv.tAdd();
		inv.display();
		inv.pop();
		cout << "Testing functions again after deletion" << endl;
		inv.display();
		inv.pop();
	}

	// Overloads << for Inventory class output
	// Specification A2 - Overload operator«
	ostream& operator << (ostream& out, const Inventory& d)
	{
		if(d.size == 0)
		{
			out << "No data to display!" << endl;
			return out;
		}

		for(int i = 0; i < d.size; i++)
		{
			out << "Item " << i + 1 << endl;
			out << string(20, '=') << endl;
			out << "ID: " << setfill('0') << setw(5) << d.inventory[i].ID << endl;
			out << "Quantity: " << d.inventory[i].quantity << endl;
			out << "Wholesale Cost: " << d.inventory[i].wholesaleCost << endl;
			out << "Retail Cost: " << d.inventory[i].retailCost << endl;
			out << "Date: " << d.inventory[i].date->format() << endl;
			out << endl;
		}

		return out;
	}

	// Overloads >> for Item struct input
	// Specification A3 - Overload operator»
	istream& operator >> (istream& in, Item& i)
	{
		cout << "ID" << endl;
		i.ID = Inventory::getID();
		cout << endl;
		cout << "Quantity" << endl;
		i.quantity = Inventory::getQuantity();
		cout << endl;
		cout << "Wholesale Cost" << endl;
		i.wholesaleCost = Inventory::getWholeCost();
		i.retailCost = i.wholesaleCost * 2;
		i.date = new Date(9,9,99);
		cout << endl;

		return in;
	}

// Prototypes
int getInt(int,int);
void clrScreen();
void createHeader(string, char = '=');
void ProgramGreeting();
void UnitTest();

int main() {
	ProgramGreeting();
	cout << endl;
	UnitTest();
	cout << endl;

  Inventory* inventory = new Inventory();
  string input;
	char choice;
  bool exit = false;
  while(!exit)
  {
    // Menu
		// Specification C1 - Alpha Menu
    createHeader("Menu");
    cout << "A. Add Inventory" << endl;
    cout << "B. Delete Inventory" << endl;
    cout << "C. Edit Inventory" << endl;
    cout << "D. Display Inventory" << endl;
    cout << "E. Exit" << endl;

    cout << endl << "Please select an option: ";

		getline(cin, input);

		if(input.length() > 1)
		{
			choice = 'F';
		} else {
			choice = input[0];
		}

    clrScreen();
		// Specification B3 - Menu Input Validation
    switch(choice)
    {
      case 'A':
        inventory->add();
        break;
      case 'B':
        inventory->pop();
        break;
      case 'C':
        inventory->edit();
        break;
      case 'D':
				cout << *inventory;
        break;
      case 'E':
        exit = true;
        break;
      default:
				clrScreen();
        cout << "Incorrect input!" << endl;
    }
		cout << endl;
  }

  clrScreen();
  cout << "Good bye!" << endl;
}


// Clears terminal
void clrScreen(){
	cout << "\033[2J\033[1;1H";
}

// Accepts a string and prints said string to the console along
// with a line of inputted characters below it, defaulted to '='
void createHeader(string header, char character)
{
  cout << header << endl;
  cout << string(header.length(), character) << endl;
}

void ProgramGreeting() {
	cout << "Welcome to " << "homework 3, Inentory Inquisitor" << "." << endl;
	cout << "This program is a simple inventory management system" << endl << endl;
}

// Specification A4 - UnitTest() method in main()
void UnitTest()
{
	Date::ComponentTest();
	cout << endl;
	Inventory::ComponentTest();
}
