// petersonHMon.cpp
// 11/22/20
// Joshua Peterson

#include <iostream>
#include <string>
#include <cstdlib>
#include <functional>
using namespace std;


// Specification C2 - Creature class
class HMon
{
friend ostream& operator<<(ostream& out, HMon& hmon);
public:
  // Default constructor
  HMon()
  {
    hunger = randNum(1,5);
    boredom = randNum(0,5);
    name = "No Name :(";
    unresponsive = false;
    starved = false;
  }

  // Name constructor
  HMon(string name)
  :HMon()
  {
    this-> name = name;
  }

  // Specification A2 - Copy Constructor
  HMon(const HMon& mon)
  {
    hunger = mon.hunger;
    boredom = mon.boredom;
    name = mon.name;
    unresponsive = mon.unresponsive;
    starved = mon.starved;
  }

  // virtual destructor
  virtual ~HMon(){};

  // Assignment operator overload
  // Specification B3 - Overload Assignment Operator
  HMon& operator=(const HMon& hmon)
  {
    this-> hunger = hmon.hunger;
    this-> boredom = hmon.boredom;
    this-> name = hmon.name;
    return *this;
  }

  // + operator
  //virtual HMon& operator+(const HMon& mon) = 0;

  // Abstract function for listen()
  // Specification B2 - Virtual Class Creature
  virtual void listen(bool testing = false) = 0;

  // Play function
  virtual void play(bool testing = false)
  {
    if(check(testing)) // If Hokeemon is unresponsive, do nothing
      return;

    boredom -= randNum(4,8);
    PassTime(testing);
  }

  // Feed function
  virtual void feed(bool testing = false)
  {
    if(check(testing)) // If Hokeemon is unresponsive, do nothing
      return;

    hunger += randNum(3,6);
    PassTime(testing);
  }

  // Get functions for testing
  string getName() const
  {
    return name;
  }

  int getHunger() const
  {
    return hunger;
  }

  int getBoredom() const
  {
    return boredom;
  }

  // Returns true if the hokeemon is unresponsive in some way
  bool check(bool testing = false)
  {
    // No terminal output if we are testing
    if(testing)
      return unresponsive || starved;

    if(unresponsive)
    {
      cout << name << " is unresponsive!" << endl;
      cout << "Remember to play with your Hokeemon!" << endl;
    }

    if(starved)
    {
      cout << name << " starved!" << endl;
      cout << "Remember to feed your Hokeemon!" << endl;
    }

    return unresponsive || starved;
  }

  // Lambda test
  void ComponentTest()
  {
    cout << "Testing random number lambda:" << endl;
    bool good = true;
    for(int i = 0; i < 1000; i++)
    {
      int val = randNum(1,10);
      if(val > 10 || val < 1)
        good = false;
      val = randNum(50,50);
      if(val != 50)
        good = false;
      val = randNum(15,20);
      if(val > 20 || val < 15)
          good = false;
    }
    if(good)
    {
      cout << "Lambda check passed." << endl;
    } else {
      cout << "Lambda check failed." << endl;
    }
  }

protected:
  int hunger;
  int boredom;
  string name;
  bool unresponsive;
  bool starved;

  // Specification C1 - PassTime()
  void PassTime(bool testing = false)
  {
    hunger -= 1;
    boredom += 1;

    if(boredom > 20)
    {
      unresponsive = true;
    }

    if(hunger < 0)
    {
      starved = true;
    }

    if(testing) // No output if testing
      return;

    if(boredom > 20)
    {
      unresponsive = true;
      cout << "Oh no! " << name << " is unresponsive!" << endl;
      cout << "Remember to play with your Hokeemon!" << endl;
    }

    if(hunger < 0)
    {
      starved = true;
      cout << "Oh no! " << name << " starved!" << endl;
      cout << "Remember to feed your Hokeemon!" << endl;
    }
  }


  // Specification A4 - Write a Lambda
  function< int (int, int )> randNum = [](int min, int max){return rand() % (max - min + 1) + min;};
};

// Prints out basic information
// Specification C4 - Overload «
ostream& operator<<(ostream& out, HMon& hmon)
{
  out << "Hunger: " << hmon.hunger << endl;
  out << "Boredom: " << hmon.boredom << endl;
  out << "Name: " << hmon.name << endl;
  return out;
}

// Specification B1 - Child Class
class Pipachu: public HMon
{
public:
  using HMon::HMon;

  // destructor
  ~Pipachu(){};

  void listen(bool testing = false)
  {
    bool state = check(true);

    printImage(state);

    cout << *this;
    if(check()) // If Hokeemon is unresponsive, do nothing
      return;

    cout << endl;
    if(hunger > 5 && boredom <= 15)
    {
      cout << name << " is feeling great!" << endl;
    }

    if(boredom > 15)
    {
      cout << name << " is feeling bored." << endl;
      cout << "Pipachus need lots of activity!" << endl;
      return;
    }

    PassTime();
  }

  // + operator overload
  //Specification B4 - Overload + Operator
  Pipachu operator+(const HMon& hmon)
  {
    Pipachu temp;
    temp.hunger = this->hunger / 2 + hmon.getHunger() / 2;
    temp.boredom = this->boredom / 2 + hmon.getBoredom() / 2;
    temp.name = this->name.substr(0,this->name.length() / 2)
    + hmon.getName().substr(hmon.getName().length()/2,hmon.getName().length()/2);

    return temp;
  }

  static void ComponentTest()
  {
    Pipachu test1;
    Pipachu test2("Test 2");
    Pipachu* test3 = new Pipachu("Test 3");
    Pipachu test4;
    test4 = test1;
    HMon* test5 = new Pipachu(test1);
    Pipachu temp1("aaaa");
    HMon* temp2 = new Pipachu("bbbb");
    Pipachu test6 = temp1 + *temp2;

    cout << "Testing hokeemon:" << endl;
    cout << "Testing initial values:" << endl;
    if(test1.getName() == "No Name :(" && test2.getName() == "Test 2"
    && test3->getName() == "Test 3" && test4.getName() == "No Name :("
    && test5->getName() == "No Name :(" && test6.getName() == "aabb")
    {
      cout << "All initial values are correct" << endl;
    } else {
      cout << "Initial values not correct" << endl;
    }

    cout << "Testing interaction:" << endl;
    HMon* arr[] = {&test1,&test2,test3,&test4,test5,&test6};

    bool good = true;
    for(HMon* mon: arr)
    {
      bool flip = false;
      for(int i = 0; i < 10; i++)
      {
        if(flip)
        {
          mon->play(true);
          flip = false;
        } else {
          mon->feed(true);
          flip = true;
        }
      }

      if(mon->check(true))
      {
        good = false;
        cout << "Failed first check" << endl;
      }

      for(int i = 0; i < 100; i ++)
      {
        mon->feed(true);
      }

      if(!mon->check(true))
      {
        good = false;
        cout << "Failed second check" << endl;
      }

      for(int i = 0; i < 100; i++)
      {
        if(flip)
        {
          mon->play(true);
          flip = false;
        } else {
          mon->feed(true);
          flip = true;
        }
      }

      if(!mon->check(true))
      {
        good = false;
        cout << "Failed third check" << endl;
      }
    }

    if(good)
    {
      cout << "Interaction testing succeeded." << endl;
    } else {
      cout << "Interaction testing failed." << endl;
    }

    delete test3;
    delete test5;
    delete temp2;
  }

private:
  void printImage(bool state)
  {
    if(!state)
      cout << R"(

              ##
            @@@@@@
            @@@@@@
          @@##::@@                                    @@++@@
          @@::::@@                @@@@@@@@@@@@@@    @@::..@@
          ++::::@@            @@++::::@@@@@@@@    @@::....##
          ++::++@@@@@@@@  @@++::::::++@@@@@@    ##::::..::++
          ++::::....::::++::::::::::@@@@      ##::::::::::::
        @@++......::++::::::::++##@@        @@::::::::::::::..
        ##::..::::##@@::::::++@@          @@::::::::::::::::..
      @@##::::::::::::::++::::@@        @@::::::::::::::##@@
      ##::::++++++::::++++::::++        ++::::::::::++@@
      @@++::::##++::::++++::::::@@      ##::::::::##@@
      @@++::::++++::::::::::::::##      @@++::++@@
        @@++::::::::::::::++::::::@@      @@++@@
      @@##++::::::::::::::::::::::##    @@##++##
  ++++::::::::::::::::::::::++::::::@@@@++++++@@
  ::::::::::::::::::::::::::::::::::::@@++++@@
  ++@@##++++++::::::::::::::::::::::::##@@##@@
              ++::::::::::::::::::::::::######@@
              ##::::::::::::::::::::::::++@@@@
              ##::::::::::::::::::::::::++##@@
              @@::::::::::::::::::::::::++
              @@::::::::::::::::::::::::++
                @@::::::++######++::::++@@
                  @@##++@@      @@@@##++@@
                    @@++@@          @@++##
                      ::              ::::

                                                                )" << endl;
    else
      cout << R"(....................................................................................
....................................................................................
......................................::::@@########::..............................
....................................##++++++::::::::##..............................
....................................##::::::::::::::##..............................
....................................##++::@@##++::++##..............................
..................................++++++::##::++##++++..............................
..............................::##++::++####....::##++..............................
..............................##::++##++............................................
..............................++++++++..............................................
....................++::........++######::..........................................
....................@@@@@@++....++####@@::..........................................
....................::@@@@##::++++++++++####::......................................
......................##@@::++++++++++++++++++++......++++++++++....................
........................@@++::++##++############++++##::::++@@@@@@##................
........................##++::::++##++::::::::::::++::::::::@@@@@@::................
......................::++++++::::++::::::::..::::::::######++++....................
....................++++::::++++::::::::::::..::::::::##............................
....................##::::::::##++::::::::::::::::::::++::..........................
..................++++::::::::##::::::::::::::::::::::++++..........................
..................##::::::::++##::::::::::::::::::::::::++::........................
..................##::::::::##::::::::::::::::::::##++::++++........................
................++++++::::::##::::::####++::::::++::++::++++........................
..............::##++++::::::##++++::::::::::::++::::::::++##::......................
................++##########::####++::::::::::++::::::##++::++......................
..........................++++::++####++::::::::::::::##::::++++::..................
............................::##++++##@@################++++++##::..................
................................::::::::................::::::::....................
....................................................................................
....................................................................................)" << endl;
  }
};

// Specification A3 - Second Child Class
class Kirbo: public HMon
{
public:
  using HMon::HMon;

  void listen(bool testing = false)
  {
    bool state = check(true);

    printImage(state);

    cout << *this;
    if(check()) // If Hokeemon is unresponsive, do nothing
      return;

    cout << endl;
    if(hunger >= 5 && boredom < 15)
    {
      cout << name << " is feeling great!" << endl;
    }

    if(hunger < 5)
    {
      cout << name << " is hungry." << endl;
      cout << "Kirbos need lots of food!" << endl;
    }

    PassTime();
  }

private:
  void printImage(bool state)
  {
    if(!state)
    cout << R"(                @@++::++@@
              @@::......::@@
            @@::..........@@
            ++............##
          @@..............++##@@
          ++....................::##@@
          ::........................::##@@
          ..................::::........++@@
        ##................::..::..........++@@
      @@::................##++++....::++....##
      ++..................@@@@++....::..::..::@@
    @@..........::::....::####....::##++::....##
    ++............::::....++::....++@@##......++@@
    ::............................++##++....::::@@
  @@....................::####::..::++......::..++@@
  @@....................::++++##++....::::::::..::::@@
  ##....................::++++++##....::::::::::::::++
  @@......................::++++::..::::::::::::::::::@@
  @@............................::::::++++++++::::::++@@
    ::......................::::::++++++++++++++++++@@
    ++................::::::::++++++++++++++++++++##@@
    @@........::::::::::::::++++++++++++++++++++++++@@
      ##....::::::::::::::::++++++++++++++++++++++++@@
      ##::..::::::::::::::++++++++++++++++++++++++##@@
      ##::::..::::::::::::++++++++++++++++++++######@@
      ##++++::::::::::::::++++++++++++++++########@@
      @@++++++++::::::::::++++++++++++##########@@@@
      @@++++++++++++++++++++++++++##########++@@@@
        ##++++++++++######@@++++++########++@@@@
        @@++++++++++####@@@@@@##++++++####@@@@
          ##++++++++####@@      @@@@@@@@
          @@##++++++####@@
            @@##++####@@@@
      @@@@@@@@@@@@##@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
      @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@)" << endl;
    else
      cout << R"(++::::++
::..::::
::::::++
++::++++
  ++++++++++          @@@@@@@@@@@@@@@@
    ++::..++    @@@@@@##++........::##@@@@
    ++::::++  @@##::::++++++::::::::++++##@@@@
      ++++##@@##++++++++++++++++++::::::::++@@@@
  ++::++  @@##::..............::++++......::++@@@@
  ::::++@@++::....................::++::....++++@@
  ++++##@@++::..................::::::++::..++++##@@
      @@++..::++++++++++++::..::::::::::++++##++::@@
      @@::++::::::::::::::::++++::::::::::####++..##@@
      ##++::++::::::++++++::::::++++::++::++##++..++@@
      @@::::::::::::::::::::::::::::++::++++####::++@@@@
    @@##::::::::++++::::::::::::::::::##++++....++##++@@
  @@##++::::::::::::::::::::::::::++++::++::....::##++##@@
  @@++::++::::::::::::::::::::::++::::::::++::::::####@@@@
    @@++++++::::::::::::::::::::++::::::::++::::++@@@@@@
      @@@@@@@@@@####++++++++++##@@##++++@@@@@@@@@@
                  @@@@@@@@@@@@@@    @@@@                  )" << endl;
  }
};


// Function Prototypes
void ProgramGreeting();
void Unittest();
int getInt(int,int);

int main() {
	Unittest();
	cout << endl;
	ProgramGreeting();

  // Set pseudo random value
  srand(12345);

  HMon* mon = nullptr;

  cout << endl;
  cout << "Please choose a hokeemon to play with:" << endl;
  cout << "1. Pipachu" << endl;
  cout << "2. Kirbo" << endl;

  int choice = getInt(1,2);

  cout << endl;
  if(choice == 1)
  {
    cout << "You chose Pipachu!" << endl;
  }
  else
  {
    cout << "You chose Kirbo!" << endl;
  }

  // Specification A1 - Critter Name
  cout << "Please enter a name for your Hokeemon: " << endl;
  string name;
  getline(cin, name);

  if(choice == 1)
  {
    mon = new Pipachu(name);
  }
  else
  {
    mon = new Kirbo(name);
  }

  cout << endl;
  cout << "Have fun with " << name << "!" << endl;
  char c;
  string input;
  while(c != 'Q')
  {
    cout << "(L)isten" << endl;
    cout << "(P)lay" << endl;
    cout << "(F)eed" << endl;
    cout << "(Q)uit" << endl;

    getline(cin, input);
    c = input[0];

    // Specification C3 - Validate Input
    switch(c)
    {
      case 'L':
        mon->listen();
        break;
      case 'P':
        mon->play();
        break;
      case 'F':
        mon->feed();
        break;
      case 'Q':
        break;
      default:
        cout << "Not valid input." << endl;
    }

    cout << endl;
  }

  delete mon;
  cout << "Hope you had fun!" << endl;
  return 0;
}


void ProgramGreeting() {
	cout << "Welcome to " << "homework 5, Hokeemon" << "." << endl;
	cout << "Care for your very own Hokeemon! ";
  cout << "Make sure you feed and play with your Hokeemon often." << endl;
  cout << "For full functionality, ";
  cout << "please have your terminal be at least 85 characters wide." << endl;
}

void Unittest() {
  Pipachu::ComponentTest();
  cout << endl;
  Pipachu test;
  test.HMon::ComponentTest();
}

// Only accepts integer input between lo and hi, inclusive, and returns it
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
