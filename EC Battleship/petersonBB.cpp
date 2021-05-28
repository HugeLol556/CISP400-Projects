// petersonBB.cpp
// Joshua peterson
// 12/17/20

#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <cmath>
// For sleeping
#include <chrono>
#include <thread>

using namespace std;

// Ship parent
// Specification C1 - OOP
class Ship
{
public:
  // Default Constructor
  Ship()
  {
    size = 0;
    shipKey = '!';
    cout << "Default Ship Created: On purpose???" << endl;
  }

  // Constructor
  Ship(int s, char c): size(s), shipKey(c)
  {
    hits = new bool[size];
    positions = new int*[size];
    for(int i = 0; i < size; i++)
    {
      positions[i] = new int[2];
      hits[i] = false;
    }
  }

  // Copy Constructor
  Ship(const Ship& rhs): size(rhs.size), shipKey(rhs.shipKey)
  {
    hits = new bool[size];
    positions = new int*[size];
    for(int i = 0; i < size; i++)
    {
      positions[i] = new int[2];
      hits[i] = rhs.hits[i];
    }

    fillPos(rhs.positions);
  }

  // Destructor
  ~Ship()
  {
    delete[] hits;
    for(int i = 0; i < size; i++)
    {
      delete [] positions[i];
    }

    delete[] positions;
  }

  // Executes hitting a ship, returning true if hit
  bool checkHit(const int pos[2])
  {
    for(int i = 0; i < size; i++)
    {
      if(positions[i][0] == pos[0] && positions[i][1] == pos[1])
      {
        if(hits[i])
        {
          cout << "Tried shooting ship part that was already shot." << endl;
          return false;
        }

        hits[i] = true;
        return true;
      }
    }

    return false;
  }

  // Check to see if ship destroyed, returns true on destroyed
  bool checkDestroy() const
  {
    bool destroyed = true;
    for(int i = 0; i < size; i++)
    {
        if(!hits[i])
        {
          destroyed = false;
        }
    }

    return destroyed;
  }

  // Fills the positions of the ship
  void fillPos(int** const pos)
  {
    for(int i = 0; i < size; i++)
    {
        positions[i][0] = pos[i][0];
        positions[i][1] = pos[i][1];
    }
  }

  // Get size
  int getSize() const
  {
    return size;
  }

  // Gets character representing ship
  char getKey()
  {
    return shipKey;
  }

  static void ComponentTest()
  {
    Ship test1(5,'C');
    int** arr = new int*[5];
    for(int i =0; i < 5; i++)
    {
      arr[i] = new int[2];
      arr[i][0] = i;
      arr[i][1] = i;
    }
    test1.fillPos(arr);
    for(int i =0; i < 5; i++)
    {
      delete[] arr[i];
    }
    delete[] arr;
    Ship test2(1,'B');
    arr = new int*[1];
    for(int i =0; i < 1; i++)
    {
      arr[i] = new int[2];
      arr[i][0] = i;
      arr[i][1] = i;
    }
    test2.fillPos(arr);
    for(int i =0; i < 1; i++)
    {
      delete[] arr[i];
    }
    delete[] arr;
    Ship test3 = test1;

    Ship ships[] = {test1,test2,test3};

    cout << "Testing Ship class" << endl;
    cout << "Testing all methods" << endl;

    bool good = true;
    for(Ship ship: ships)
    {
      for(int i =0; i < ship.getSize(); i++)
      {
        int pos[] = {i,i};
        if(!ship.checkHit(pos))
        {
          cout << "Failure in checkHit() checks Ship" << endl;
          good = false;
        }
      }

      if(!ship.checkDestroy())
      {
        good = false;
        cout << "Failure in checkDestroy() checks Ship" << endl;
      }
    }

    if(good)
    {
      cout << "All testing passed." << endl;
    } else {
      cout << "Ship testing failed" << endl;
    }
  }

protected:
  int size;
  char shipKey;
  bool* hits;
  int** positions;
};

// Ship  child classes
// Specification A3 - Child Classes
class Carrier: public Ship
{
public:
  using Ship::Ship;
  // constructor
  Carrier(): Ship(5,'C')
  {}
};

class Battleship: public Ship
{
public:
  using Ship::Ship;
  // constructor
  Battleship(): Ship(4,'B')
  {}
};

class Cruiser: public Ship
{
public:
  using Ship::Ship;
  // constructor
  Cruiser(): Ship(3,'R')
  {}
};

class Submarine: public Ship
{
public:
  using Ship::Ship;
  // constructor
  Submarine(): Ship(3,'S')
  {}
};

class Destroyer: public Ship
{
public:
  using Ship::Ship;
  // constructor
  Destroyer(): Ship(2,'D')
  {}
};

// 2D Grid class that allows interaction through understandable
// x,y input, in the range of 1 to size
template <class type>
class Grid{
public:

  // Print 2D array
  friend ostream& operator<<(ostream& out, const Grid& grid)
  {
    for(int i = 0; i < grid.height; i++)
    {
      for(int j = 0; j < grid.width; j++)
      {
        out << grid.gridArr[i][j];
      }
      out << endl;
    }

    return out;
  }

  // Default constructor
  Grid()
  {}

  // Square grid constructor
  Grid(int size): Grid(size,size)
  {}

  // Rectangular grid constructor
  Grid(int x, int y): width(x), height(y)
  {
    init();
    this->generate();
  }

  // Constructor with initial value
  Grid(int x, int y, type initial): width(x), height(y), initial(initial)
  {
    init();
    this->generate();
  }

  // Copy constructor
  Grid(const Grid<type>& rhs): width(rhs.width),height(rhs.height)
  {
    init();
    for(int i = 0; i < height; i++)
    {
      for(int j = 0; j < width; j++)
      {
        gridArr[i][j] = rhs.gridArr[i][j];
      }
    }
  }

  // destructor
  virtual ~Grid()
  {
    for(int i = 0; i < height; i++)
    {
      delete [] gridArr[i];
    }

    delete [] gridArr;
  }

  // Overload []
  // WARNING: Interacts with 2D array like normal in C++
  type* operator[](const int i)
  {
    if(i >= height || i < 0)
    {
      cout << "Out of Bounds Error! Grid Overloaded []" << endl;
      throw;
    }

    return gridArr[i];
  }

  // Assignment Overload
  Grid& operator=(const Grid& rhs)
  {
    // Delete our current data
    for(int i = 0; i < height; i++)
    {
      delete [] gridArr[i];
    }

    delete [] gridArr;

    width = rhs.width;
    height = rhs.height;

    // Make new 2D array and copy over all data
    init();
    for(int i = 0; i < height; i++)
    {
      for(int j = 0; j < width; j++)
      {
        gridArr[i][j] = rhs.gridArr[i][j];
      }
    }

    return *this;
  }

  // Getters for height and width
  int getW()
  {
    return width;
  }

  int getH()
  {
    return height;
  }

  // Generate grid
  virtual void generate()
  {
    for(int i = 0; i < height; i++)
    {
      for(int j = 0; j < width; j++)
      {
        gridArr[i][j] = initial;
      }
    }
  };

  // Individual getter and setter, following x,y format
  type get(int x, int y) const
  {
    if(x > width || x < 1 || y > height || y < 1)
    {
      cout << "Out of Bounds Error! Grid get()" << endl;
      throw;
    }

    return gridArr[cY(y)][cX(x)];
  }

  void set(int x, int y,const type item)
  {
    if(x > width || x < 1 || y > height || y < 1)
    {
      cout << "Out of Bounds Error! Grid set()" << endl;
      throw;
    }

    gridArr[cY(y)][cX(x)] = item;
  }



  // Component test
  static void ComponentTest()
  {
    cout << "Testing Grid Class" << endl;

    Grid<int> test1(2,2,1);
    Grid<int> test2(4,6,2);
    Grid<int> test3 = test1;
    Grid<int>* test4 = new Grid<int>(1,10,4);

    Grid<int> arr[] = {test1,test2,test3,*test4};
    int intArr[] = {1,2,1,4};
    cout << "Testing initial values" << endl;
    bool pass = true;
    int k = 0;
    for(Grid<int> grid: arr)
    {
      for(int i = 1; i <= grid.getW(); i++)
      {
        for(int j = 1; j <= grid.getH(); j++)
        {
          if(grid.get(i,j) != intArr[k])
            pass = false;
        }
      }

      k++;
    }

    if(pass)
      cout << "Initial values passed" << endl;
    else
      cout << "Inital values FAILED" << endl;

    cout << "Testing I/O" << endl;
    pass = true;
    for(Grid<int> grid: arr)
    {
      for(int i = 1; i <= grid.getW(); i++)
      {
        for(int j = 1; j <= grid.getH(); j++)
        {
          grid.set(i,j,5);
          if(grid.get(i,j) != 5)
            pass = false;
        }
      }
    }

    if(pass)
      cout << "All I/O testing passed." << endl;
    else
      cout << "I/O testing FAILED." << endl;

    delete test4;
  }

protected:
  int width, height;
  type** gridArr;
  type initial; // Initial value

  // Initalize the 2D array
  void init()
  {
    gridArr = new type *[height];
    for(int i = 0; i < height; i++)
    {
      gridArr[i] = new type[width];
    }
  }

  // When interacting with grid as x,y, converts x input
  // to correct value for access
  int cX(int x) const
  {
    return x - 1;
  }

  // When interacting with grid as x,y, converts y input
  // to correct value for access
  int cY(int y) const
  {
    return abs(y - height);
  }
};

// Grid for battleship, generates grid with letters down a side and numbers across
class BBGrid: public Grid<char>
{
public:
  using Grid<char>::Grid;

  friend ostream& operator<<(ostream& out, BBGrid& grid);

  // Default Constructor
  BBGrid(): Grid(11,11,' ')
  {
    generate();
  }

  // Copy constructor
  BBGrid(const BBGrid& rhs): Grid<char>::Grid(rhs)
  {

  }

  // Adds the letters and numbers to battleship board
  virtual void generate()
  {
    for(int i = 2; i <= 11; i++)
    {
      this->set(1,i,('J' - i + 2));
    }

    for(int i = 2; i <= 11; i++)
    {
      this->set(i,1,i - 2 + '0');
    }
  }
};

// Outputs grid with reasonable spacing
// Specification C4 - Overload «
ostream& operator<<(ostream& out, BBGrid& grid)
{
  for(int i = 0; i < grid.height; i++)
  {
    for(int j = 0; j < grid.width; j++)
    {
      out << grid.gridArr[i][j] << " ";
    }
    out << endl;
  }

  return out;
}

// Grid that holds ships and places them on board
class ShipGrid: public BBGrid
{
public:
  using BBGrid::BBGrid;

  // Default Constructor
  ShipGrid(): BBGrid()
  {
    generate();
  }

  // Copy constructor
  ShipGrid(const ShipGrid& rhs): BBGrid(rhs)
  {
    for(int i = 0; i < 5; i++)
    {
      delete ships[i];
      ships[i] = new Ship(*rhs.ships[i]);
    }
  }

  // Deconstructor
  ~ShipGrid()
  {
    for(int i = 0; i < 5; i++)
    {
      delete ships[i];
    }
  }

  // Randomly places ships on board, and fills the ships with
  // their location data
  // Specification B3 - Random Start
  void generate()
  {
    for(int i = 0; i < 5; i++)
    {
      int size = ships[i]->getSize();
      // Initialize array to hold positions
      int** pos = new int*[size];
      for(int i = 0; i < size; i++)
      {
        pos[i] = new int[2];
      }

      // Place a single ship
      bool done;
      int counter = 0; // Breakout counter incase loop happens forever
      do{
        counter++;
        done = true;
        // Choose random spot within grid
        int x = rand() % 10 + 2;
        int y = rand() % 10 + 2;

        if(isOpen(x,y)) // If available, try ship placement
        {
          // Randomly choose direction
          bool xDir = rand() % 2; // X or Y
          int direction = (rand() % 2) ? -1 : 1; // Add or subtract

          // Try direction
          for(int j = 0; j < size; j++){
            pos[j][0] = x;
            pos[j][1] = y;
            // If on last spot, don't do another check
            if(j == (size - 1))
              continue;
            if(xDir)
            {
              x += direction;
            } else
            {
              y += direction;
            }
            // If spot doesn't work, end loop
            if(!isOpen(x,y)){
              done = false;
              break;
            }
          }
        } else {
          done = false;
        }

        if(done)
          ships[i]->fillPos(pos);
      } while(!done && counter < 10000);

      if(counter >= 10000)
      {
        cout << "ERROR ShipGrid: Single ship placement loop" << endl;
        throw;
      }

      // Place ship on grid
      for(int k = 0; k < size; k++)
      {
        this->set(pos[k][0],pos[k][1], ships[i]->getKey());
      }

      // Delete array
      for(int i = 0; i < size; i++)
      {
        delete [] pos[i];
      }
      delete [] pos;
      pos = nullptr;
    }
  }

  // Returns a pointer to a ship based on location on grid, else nullptr
  Ship* getShip(int x, int y)
  {
    char result;

    result = this->get(x,y);
    for(int k = 0; k < 5; k++)
    {
      if(result == ships[k]->getKey())
        return ships[k];
    }

    return nullptr;
  }

  // Returns number of ships destroyed
  int getDestroyed()
  {
    int count = 0;
    for(Ship* ship: ships)
    {
      if(ship->checkDestroy()) // Add 1 per destroyed ship
       count++;
    }

    return count;
  }

private:
  Ship* ships[5] = {new Destroyer(), new Submarine(), new Carrier(), new Battleship(), new Cruiser()};

  // Checks if spot is open
  bool isOpen(int x, int y)
  {
      if(x < 12 && x > 1 && y < 12 && y > 1)
        return this->get(x,y) == ' ';
      else
        return false;
  }
};

// Manages the actions of a player
class PlayerManager
{
public:
  // Default constructor
  PlayerManager()
  {

  }

  // If the shot can be fired, executes the shot and returns result
  // If can't be fired, returns -1
  int fireShot(int x, int y, PlayerManager& other)
  {
    if(x > 11 || x < 2 || y < 2 || y > 11){
      cout << "Trying to fire outside map. fireShot() PlayerManager" << endl;
      return -1;
    }

    char spot = pShots.get(x,y);
    int result;
    if(spot != 'O' && spot != 'X') // If location hasn't been shot at
    {
      result = other.receiveShot(x,y);
      if(result == 0)
        pShots.set(x,y,'O');
      else
        pShots.set(x,y,'X');
    } else {
      return -1; // Location has already been shot at, cannot shoot there
    }

    return result;
  }

  // Executes the recieving a shot, and returns an int based on result
  // 0 miss, 1 hit, 2 sink
  int receiveShot(int x, int y)
  {
    if(x > 11 || x < 2 || y < 2 || y > 11){
      cout << "Receiving shot outside of map. receiveShot() PlayerManager" << endl;
      return 0;
    }

    int result = 0;

    Ship* hitShip = pShips.getShip(x,y);

    // If hit ship, damage it then check if destroyed
    if(hitShip != nullptr)
    {
      int tempArr[2] = {x,y};
      hitShip->checkHit(tempArr);
      result = 1;

      if(hitShip->checkDestroy())
        result = 2;

      pShips.set(x,y,'X'); // Set area as hit
    } else {

      pShips.set(x,y,'O'); // Set area as miss
      result = 0;
    }

    return result;
  }

  void showGrids()
  {
    createHeader("Ships", '=');
    cout << pShips;
    cout << endl;
    createHeader("Shots", '=');
    cout << pShots;
  }

  // Returns number of ships destroyed
  int getDestroyed()
  {
    return pShips.getDestroyed();
  }

  // Returns this players ship grid
  ShipGrid getPShips()
  {
    return pShips;
  }

private:
  ShipGrid pShips;
  BBGrid pShots;

  // Accepts a string and prints said string to the console along
  // with a line of inputted characters below it, defaulted to '='
  void createHeader(string header, char character)
  {
    cout << header << endl;
    cout << string(header.length(), character) << endl;
  }
};

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

		dArray<string> test10;

		string* array = new string[10];
		delete array;

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

		// If array is empty, ignore copying to temp array
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

// Records both player moves, then outputs them
class Recorder
{
public:
  // Default Constructor
  Recorder()
  {

  }

  // Records the current turn
  void record(ShipGrid g, int s, char l, char n, int r)
  {
    Turn temp;
    temp.grid = g;
    temp.shooter = s;
    temp.letter = l;
    temp.number = n;
    temp.result = r;
    turns.add(temp);
  }

  // Replays the game back at given speed, turns per second
  void replay(double tps)
  {
    cout << endl;
    cout << "Starting replay" << endl;
    cout << endl;
    for(int i = 0; i < turns.len(); i++)
    {
      chrono::nanoseconds time = chrono::nanoseconds((int)((1.0 / tps) * 1000000000));

      this_thread::sleep_for(time);

      Turn temp = turns[i];
      // Output
      if(temp.shooter == 1)
      {
        if(temp.letter == 'S')
        {
          cout << "You decided to be sneaky and look at the computer..." << endl;
          cout << endl;
          continue;
        } else if(temp.letter == 'X')
        {
          cout << "You decided to quit." << endl;
          cout << endl;
          continue;
        }

        cout << "You shot the computer at " << temp.letter << " " << temp.number << " ";
      } else {
        cout << "The computer shot you at " << temp.letter << " " << temp.number << " ";
      }

      if(temp.result == 0)
      {
        cout << "and missed." << endl;
      } else if(temp.result == 1){
        cout << "and hit." << endl;
      } else {
        cout << "and destroyed a ship." << endl;
      }

      cout << endl;

      if(temp.shooter == 1)
      {
        cout << "Computer Grid" << endl;
        cout << temp.grid;
      } else {
        cout << "Player Grid" << endl;
        cout << temp.grid;
      }

      cout << endl;

      // Last turn so there must be a winner
      if(i == turns.len() - 1)
      {
        if(temp.shooter == 1)
        {
          cout << "And you won!" << endl;
        } else {
          cout << "And the computer won." << endl;
        }
      }
    }
  }

private:
  // Holds all necessary data for the turn
  struct Turn
  {
    BBGrid grid; // Shot players grid, after the shot
    int shooter; // Person who shot, 1 = player, 2 = computer
    // Letter Num of shot
    char letter;
    char number;
    int result; // Result of shot, 0 miss, 1 hit, 2 destroy
  };

  dArray<Turn> turns; // Dynamic array of turns
};

// Function prototypes
void UnitTest();
void ProgramGreeting();
void parseInput(char input[2]);

// Accepts player coords entry
//Specification B4 - Overload »
istream& operator>>(istream& in, char input[])
{
  string temp;
  bool good;
  do{
    good = true;
    getline(in, temp);

    if(temp.length() != 3)
    {
      good = false;
      cout << "Bad Syntax: Input length must be 3. Ex:A 1" << endl;
      continue;
    }

    if(temp[1] == 'X')
    {
      cout << endl;
      cout << "Exit key accepted" << endl;
      input[0] = 'X';
      input[1] = 'X';
      return in;
    }

    if(temp[1] == 'S')
    {
      cout << endl;
      cout << "Secret key accepted" << endl;
      input[0] = 'S';
      input[1] = 'S';
      return in;
    }

    if(temp[1] != ' ')
    {
      good = false;
      cout << "Bad Syntax: Must be a space between inputs. Ex:A 1" << endl;
      continue;
    }

    // Specification C3 - Validate Input
    if(temp[0] < 65 || temp[0] > 74)
    {
      good = false;
      cout << "Bad Syntax: First input must be a capital letter A-J" << endl;
      continue;
    }

    if(temp[2] < 48 || temp[2] > 57)
    {
      good = false;
      cout << "Bad Syntax: Second input must be number 0-9" << endl;
      continue;
    }
  }while(!good); // While bad input, loop

  input[0] = temp[0];
  input[1] = temp[2];

  return in;
}

int main()
{
  UnitTest();
  cout << endl;
  ProgramGreeting();

  srand(123456); // Random init

  int shots;
  bool multishot;
  PlayerManager player;
  PlayerManager computer;
  Recorder gameRecord;
  double playBackTPS = 1;

  cout << endl;
  cout << "Would you like to play a multishot game?" << endl;
  cout << "Shots equal to your number of ships alive instead " << endl;
  cout << "of 1. Enter Y for yes." << endl;
  string choice;
  getline(cin, choice);

  if(choice == "Y")
  {
    cout << "Multishot shot game chosen" << endl;
    multishot = true;
  } else
  {
    cout << "Single shot game chosen" << endl;
    multishot = false;
  }

  cout << endl;
  cout << "Here is your game board" << endl;
  player.showGrids();
  cout << endl;
  cout << "Key:" << endl;
  cout << "X - Hit" << endl;
  cout << "O - Miss" << endl;
  cout << "D - Destroyer" << endl;
  cout << "B - Battleship" << endl;
  cout << "R - Cruiser" << endl;
  cout << "C - Carrier" << endl;
  cout << "S - Submarine" << endl;
  cout << endl;
  cout << "When it is your turn, choose a spot to shoot " << endl;
  cout << "by entering first the letter, then the number. Ex:A 1" << endl;
  cout << "Press enter when you are ready to begin. Good luck and have fun!" << endl;
  cout << "Quit: Replace space with X Ex:AX1" << endl;
  cout << "Debugging Key: Replace the space with S to see enemy board. Ex:AS1" << endl;
  string temp;
  getline(cin, temp);


  int winner = 0; // 1 is player, 2 is computer
  while(!winner) // While winner hasn't been set
  {
    // Player Turn
    cout << endl;
    cout << "Your Turn!" << endl;
    cout << endl;
    cout << "Your Game Board" << endl;
    player.showGrids();
    cout << endl;
    if(multishot)
    {
      // Specification A2 - MultiFire Game
      shots = 5 - player.getDestroyed();
      cout << "You get " << shots << " shot(s)." << endl;
    }
    else
    {
      shots = 1;
      cout << "You get " << shots << " shot." << endl;
    }

    bool good;
    char input[2] = {'S','S'};
    // Number of shots
    for(int i = 0; i < shots; i++)
    {
      // Get and attempt player shot
      do{
        good = true;
        cout << endl;
        cout << shots - i << " shot(s) left." << endl;
        cout << "Choose a point to shoot:" << endl;
        cin >> input;

        // Specification A1 - Resign Game
        if(input [0] == 'X')
        {
          gameRecord.record(computer.getPShips(), 1, input[0], input[1], 0);
          cout << endl;
          cout << "Quitting game" << endl;
          break;
        }

        // Specification B1 - Secret Option
        if(input[0] == 'S')
        {
          gameRecord.record(computer.getPShips(), 1, input[0], input[1], 0);
          cout << endl;
          cout << "Showing enemy game board" << endl;
          computer.showGrids();
          continue;
        }

        // Coords of player shot
        int y = abs(input[0] - 74) + 2; // Convert letter to num
        int x = input[1] - 48 + 2;  // Convert char num to int num

        int result = player.fireShot(x,y, computer);

        cout << endl;
        if(result == -1)
        {
          good = false;
          // Specification B2 - Adv Input Validation
          cout << "You've already fired there. Please try again." << endl;
        }
        else
        {
          gameRecord.record(computer.getPShips(), 1, input[0], input[1], result);
          if(result == 0)
          {
            cout << "Miss" << endl;
          } else if(result > 0){
            cout << "Hit!" << endl;
          }
        }
      }while(!good); // Bad input

      if(input [0] == 'X')
      {
        break;
      }

      // Check for winner after player shot
      if(computer.getDestroyed() == 5) // If 5 destroyed, all ships gone
      {
        winner = 1;
        break; // Skip other shots
      }
    }

    if(input [0] == 'X')
    {
      break;
    }

    cout << endl;
    cout << "Your turn is over." << endl;
    // End player Turn

    // Check for winner after player turn
    if(computer.getDestroyed() == 5) // If 5 destroyed, all ships gone
    {
      winner = 1;
      continue; // Skip any other actions
    }

    // Computer Turn
    if(multishot)
    {
      shots = 5 - computer.getDestroyed();
    }
    else
    {
      shots = 1;
    }

    // Number of shots
    for(int i = 0; i < shots; i++)
    {
      // Get and attempt shot
      do{
        good = true;

        // Randomly get computer shot
        int x = rand() % 10 + 2;
        int y = rand() % 10 + 2;

        int result = computer.fireShot(x,y, player);
        // Specification C2 - Prohibit AI wasted shots
        if(result == -1)
        {
          good = false;
        }
        else
        {
          gameRecord.record(player.getPShips(), 2, static_cast<char>('J' - y + 2), static_cast<char>('0' + x - 2), result);
          cout << endl;
          cout << "Computer shot at " << static_cast<char>('J' - y + 2) << " " << static_cast<char>('0' + x - 2);
          if(result == 0)
          {
            cout << " and missed." << endl;
          } else if(result == 1){
            cout << " and hit your ship!" << endl;
          } else
          {
            cout << " and destroyed one of your ships!" << endl;
          }
        }
      }while(!good); // Bad input

      // Check for winner after computer shot
      if(player.getDestroyed() == 5) // If 5 destroyed, all ships gone
      {
        winner = 2;
        break; // Skip other shots
      }
    }
    // Computer turn end

    // Check for winner after computer turn
    if(player.getDestroyed() == 5) // If 5 destroyed, all ships gone
    {
      winner = 2;
      continue; // Skip any other actions
    }

  }

  cout << endl;
  if(winner == 0)
  {
    cout << "Game ended" << endl;
  }
  else if(winner == 1)
  {
    cout << "Congrats! You won!" << endl;
  } else {
    cout << "You lost. Better luck next time." << endl;
  }

  cout << endl;
  cout << "Press enter to view replay" << endl;
  getline(cin, temp);

  // Specification A4 - Replay Finished Game
  gameRecord.replay(playBackTPS);

  cout << "Replay finished. Have a nice day." << endl;
  cout << "Good Bye!" << endl;
  return 0;
}

void UnitTest()
{
  Grid<double>::ComponentTest();
  cout << endl;
  Ship::ComponentTest();
}

void ProgramGreeting() {
	cout << "Welcome to " << "extra cred Battleship" << "." << endl;
	cout << "Play a game of battleship vs the computer." << endl;
}

// Gets player input and stores it in an array passed to function
void parseInput(char input[2])
{
  string temp;
  bool good;
  do{
    good = true;
    getline(cin, temp);

    if(temp.length() != 3)
    {
      good = false;
      cout << "Bad Syntax: Input length must be 3. Ex:A 1" << endl;
      continue;
    }

    if(temp[1] == 'X')
    {
      cout << "Exit key accepted" << endl;
      input[0] = 'X';
      input[1] = 'X';
      return;
    }

    if(temp[1] == 'S')
    {
      cout << "Secret key accepted" << endl;
      input[0] = 'S';
      input[1] = 'S';
      return;
    }

    if(temp[1] != ' ')
    {
      good = false;
      cout << "Bad Syntax: Must be a space between inputs. Ex:A 1" << endl;
      continue;
    }

    // Specification C3 - Validate Input
    if(temp[0] < 65 || temp[0] > 74)
    {
      good = false;
      cout << "Bad Syntax: First input must be a capital letter A-J" << endl;
      continue;
    }

    if(temp[2] < 48 || temp[2] > 57)
    {
      good = false;
      cout << "Bad Syntax: Second input must be number 0-9" << endl;
      continue;
    }
  }while(!good); // While bad input, loop

  input[0] = temp[0];
  input[1] = temp[2];
}
