// ec3.cpp
// 10/11/20
// Joshua Peterson

#include <iostream>
#include <string>

using namespace std;



class DayOfYear {
public:
	DayOfYear(int day)
	{
		int month;

		if(day > 365){
			cout << "Day cannot be greater than 365" << endl;
			throw;
		}

		if(day < 1){
			cout << "Day cannot be less than 1" << endl;
			throw;
		}

		for(int i = 0; i < 12; i++)
		{
			month = i + 1;
			if(day <= mDays[i])
			{
				break;
			}
			day -= mDays[i];
		}

		this->day = day;
		this->month = static_cast<months>(month);
	}

	DayOfYear(int day, string month)
	{
		if(month == "January")
		{
			this-> month = Janurary;
		}
		else if(month == "February")
		{
			this->month = February;
		}
		else if(month == "March")
		{
			this->month = March;
		}
		else if(month == "April")
		{
			this->month = April;
		}
		else if(month == "May")
		{
			this->month = May;
		}
		else if(month == "June")
		{
			this->month = June;
		}
		else if(month == "July")
		{
			this->month = July;
		}
		else if(month == "August")
		{
			this->month = August;
		}
		else if(month == "September")
		{
			this->month = September;
		}
		else if(month == "October")
		{
			this->month = October;
		}
		else if(month == "November")
		{
			this->month = November;
		}
		else if(month == "December")
		{
			this->month = December;
		}
		else
		{
			cout << "Error: Incorrect month" << endl;
			throw;
		}

		if(day < 1){
			cout << "Error: Day cannot be less than 1" << endl;
			throw;
		}

		if(day > mDays[this->month - 1]){
			cout << "Error: Day cannot be greater than days in the month chosen" << endl;
			throw;
		}

		this->day = day;
	}

	void print()
	{
		cout << mNames[month - 1] << " - " << day << endl;
	}

	// Pre-fix
	DayOfYear operator++()
	{
		if(day + 1 > mDays[month - 1])
		{
			day = 1;
			if((int)month + 1 == 13)
			{
				month = static_cast<months>(1);
			} else {
				month = static_cast<months>(month + 1);
			}
		} else
		{
			day++;
		}

		return *this;
	}

	// Post-fix
	DayOfYear operator++(int)
	{
		if(day + 1 > mDays[month - 1])
		{
			day = 1;
			if((int)month + 1 == 13)
			{
				month = static_cast<months>(1);
			} else {
				month = static_cast<months>(month + 1);
			}
		} else
		{
			day++;
		}

		return *this;
	}

	// Pre-fix
	DayOfYear operator--()
	{
		if(day - 1 == 0)
		{
			if((int)month - 1 == 0)
			{
				month = static_cast<months>(12);
			} else {
				month = static_cast<months>(month - 1);
			}

			day = mDays[month - 1];
		} else
		{
			day--;
		}

		return *this;
	}

	// Post-fix
	DayOfYear operator--(int)
	{
		if(day - 1 == 0)
		{
			if((int)month - 1 == 0)
			{
				month = static_cast<months>(12);
			} else {
				month = static_cast<months>(month - 1);
			}

			day = mDays[month - 1];
		} else
		{
			day--;
		}

		return *this;
	}

private:
	enum months {Janurary=1,February,March,April,
							May,June,July,August,September,October,November,December};

	int day;
	months month;
	int mDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	string mNames[12] = {"Janurary","February","March","April","May","June",
					"July","August","September","October","November","December"};

};

// Function Prototypes
void ProgramGreeting();

int main() {
	ProgramGreeting();

	int nums[] = {10,365,100,200,300,50,32};

	cout << "Testing multiple inputs for first constructor" << endl;
	cout << endl;
	for(int i = 0; i < int(sizeof(nums) / 4); i++)
	{
		cout << "Input: " << nums[i] << " Result: ";
		DayOfYear* t1 = new DayOfYear(nums[i]);
		t1->print();
		delete t1;
	}
	cout << endl;

	cout << "Testing second constructor" << endl;
	DayOfYear t2(29,"December");
	DayOfYear t3(5, "January");
	cout << "Input: 29, December Result : ";
	t2.print();
	cout << "Input: 5, January Result : ";
	t3.print();
	cout << endl;

	cout << "Testing ++ and --" << endl;
	cout << "++ once to both previous dates" << endl;
	t2++;
	t3++;
	t2.print();
	t3.print();
	cout << "-- twice to both previous dates" << endl;
	t2--;
	t3--;
	t2--;
	t3--;
	t2.print();
	t3.print();
	cout << "Underflowing below Janurary date, and overflowing above December date" << endl;
	for(int i = 0;i < 4; i++)
	{
		t2++;
	}
	for(int i = 0;i < 5; i++)
	{
		t3--;
	}
	t2.print();
	t3.print();
	cout << endl;

	cout << "Testing error handling for day outside of months range" << endl;
	cout << "Input 30, February" << endl;
	DayOfYear t4(30,"February");
}


void ProgramGreeting() {
	cout << "Welcome to " << "Extra Credit 3" << "." << endl;
	cout << "Simple class that stores day and month ";
	cout << "and can be added or subtracted a day" << endl << endl;
}
