// petersonGA.cpp
// 12/17/20
// Joshua Peterson

#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>
// For sleeping
#include <chrono>
#include <thread>

using namespace std;

// Enums
enum mapObj {EMPTY = 0, WALL, BATTERY, ROBOT};
enum dir {NORTH = 0, SOUTH, EAST, WEST,RANDOM};

// 2D Grid class that allows interaction through
// x,y input, in the range of 1 to size, inclusive
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
  Grid operator=(const Grid& rhs)
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

private:
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

class Map : public Grid<mapObj>
{
  friend ostream& operator<<(ostream& out, Map& map);
public:
  // Default and Overloaded Constructor
  Map(float pBat = 40, float pWall = 30, float var = 0, int size = 12):
  Grid(size),
  percentBattery(pBat),
  percentWall(pWall),
  variation(var),
  size(size)
  {
    this->generate();
  }

    void generate()
    {
      // % chance that a wall placement tries to place another wall next to it
      const int CHANCE_OF_WALL_CONTINUE = 0;

      // Make edges walls and clear inner
      for(int i = 1; i <= width; i++)
      {
        for(int j = 1; j <= height; j++)
        {
          if(i == 1 || j == 1 || i == size || j == size)
            this->set(i,j,WALL);
          else
            this->set(i,j,EMPTY);
        }
      }

      // Percent of inner wall space
      int walls = (size - 2)*(size - 2) * percentWall / 100.0;
      int batteries = (size - 2)*(size - 2) * percentBattery / 100.0;

      // If count is greater than error count, stop program
      // because infinite loop likely occured
      int errorCount = 100000;
      int counter = 0;
      // Place all walls
      while(walls > 0)
      {
        int randX = (rand() % (size - 2)) + 2;
        int randY = (rand() % (size - 2)) + 2;

        if(randX < 2 || randY < 2 || randX > size - 1 || randY > size - 1)
        {
          cout << "Out of bounds error in map wall generate" << endl;
          throw;
        }

        // If empty space, place
        if(this->get(randX,randY) == EMPTY)
        {
          this->set(randX,randY,WALL);
          walls--;

          // Try to place walls around current wall
          while((rand() % 100) + 1 <= CHANCE_OF_WALL_CONTINUE)
          {
            // Randomly determine if we add or subtract from each direction
            int dirX = rand() % 3 - 1; // Result in -1,0, or 1
            int dirY = rand() % 3 - 1;

            // If open spot, place wall and set current coords
            if(this->get(randX + dirX, randY + dirY) == EMPTY)
            {
              randX += dirX;
              randY += dirY;
              this->set(randX,randY,WALL);
              walls--;

              // Stop if no more walls to place
              if(walls <= 0)
                break;
            }
          }
        }

        counter++;
        if(counter > errorCount)
        {
          cout << "Over " << errorCount << " loops occured";
          cout << " in wall generation. Stopping program." << endl;
          cout << "This suggests an infinite loop occured.";
          cout << " Please change input values." << endl;
          throw;
        }
      }

      counter = 0;

      // Place all batteries
      while(batteries > 0)
      {
        int randX = rand() % (size - 2) + 2;
        int randY = rand() % (size - 2) + 2;

        if(randX < 2 || randY < 2 || randX > size - 1 || randY > size - 1)
        {
          cout << "Out of bounds error in map battery generate" << endl;
          throw;
        }

        // If empty space, place
        if(this->get(randX,randY) == EMPTY)
        {
          this->set(randX,randY,BATTERY);
          batteries--;
        }

        counter++;
        if(counter > errorCount)
        {
          cout << "Over " << errorCount << " loops occured";
          cout << " in battery generation. Stopping program." << endl;
          cout << "This suggests an infinite loop occured.";
          cout << " Please change input values." << endl;
          throw;
        }
      }

    }

private:
  float percentBattery; //  Percent of inner map that is battery
  float percentWall; // Percent of inner map that is wall
  float variation; // + or - % variation in wall and batteries
  int size;

};

// Outputs map with reasonable spacing and chars instead of ints
ostream& operator<<(ostream& out, Map& map)
{
  char intToLet[] = {' ','W','B','R'}; // Converts int to letter

  for(int i = 0; i < map.height; i++)
  {
    for(int j = 0; j < map.width; j++)
    {
      out << intToLet[map[i][j]] << " ";
    }
    out << endl;
  }

  return out;
}

class DNA
{
friend ostream& operator<<(ostream& out,const DNA& dna);

public:
  // Default Constructor
  DNA(): mutationChance(0), size(16)
  {
    genes = new Gene[16];
  }

  // Constructor
  DNA(int mutChance, int size = 16): mutationChance(mutChance),
  size(size)
  {
    genes = new Gene[size];
  }

  // Copy constructor
  DNA(const DNA& rhs): mutationChance(rhs.mutationChance),
  size(rhs.size)
  {
    genes = new Gene[size];

    for(int i = 0; i < size; i++)
    {
      genes[i] = rhs.genes[i];
    }
  }

  // Deconstructor
  ~DNA()
  {
    delete [] genes;
  }

  // Assignment operator
  DNA operator=(const DNA& rhs)
  {

    delete [] genes;

    size = rhs.getSize();
    mutationChance = rhs.getMutChance();

    genes = new Gene[size];

    for(int i = 0; i < size; i++)
    {
      genes[i] = rhs.genes[i];
    }

    return *this;
  }

  // Combines DNA. Size of both must be the same
  // half from lhs, half from rhs
  DNA operator+(const DNA& rhs)
  {
    // Chance that genes shift after copying
    const int SHIFT_CHANCE = 50;

    if(size != rhs.size)
    {
      cout << "Error: Size values different in DNA +" << endl;
      throw;
    }

    DNA temp;

    temp.size = size;
    temp.mutationChance = mutationChance;
    temp.genes = new Gene[size];

    for(int i = 0; i < size; i++)
    {
      // half from rhs
      if(i >= size / 2)
        temp.genes[i] = rhs.genes[i];
      // half from lhs
      else
        temp.genes[i] = genes[i];
    }

    if(rand() % 100 + 1 <= mutationChance)
    {
      // Shift all DNA down 1. First gene becomes 16th
      if(rand() % 100 + 1 <= SHIFT_CHANCE)
      {
        Gene tempG = temp.genes[size - 1];

        for(int i = size - 1; i > 0; i--)
        {
          temp.genes[i] = temp.genes[i - 1];
        }

        temp.genes[0] = tempG;
      }
      // Randomly select a gene and randomly change one of its values
      else
      {
        int geneChoice = rand() % size;
        int choice = rand() % 5;
        switch(choice)
        {
          case 0:
            temp.genes[geneChoice].n = static_cast<mapObj>(rand() % OBJ_P);
            break;
          case 1:
            temp.genes[geneChoice].s = static_cast<mapObj>(rand() % OBJ_P);
            break;
          case 2:
            temp.genes[geneChoice].e = static_cast<mapObj>(rand() % OBJ_P);
            break;
          case 3:
            temp.genes[geneChoice].w = static_cast<mapObj>(rand() % OBJ_P);
            break;
          case 4:
            temp.genes[geneChoice].direction = static_cast<dir>(rand() % MOV_P);
            break;
          default:
            cout << "Error in DNA + mutation switch" << endl;
            throw;
        }
      }
    }
    return temp;
  }

  // Returns direction based on mapObj passed
  dir getDir(mapObj n, mapObj s, mapObj e, mapObj w)
  {
    for(int i = 0; i < size; i++)
    {
      if(genes[i].match(n,s,e,w))
        return genes[i].direction;
    }

    // If no gene matches, use last
    return genes[size - 1].direction;
  }

  // Various gets
  int getMutChance() const
  {
    return mutationChance;
  }

  int getSize() const
  {
    return size;
  }

private:
  // Robot mapObj is equivalent to don't care value
  struct Gene
  {
    mapObj n;
    mapObj s;
    mapObj e;
    mapObj w;
    dir direction;

    // Constructor
    Gene()
    {
      n = static_cast<mapObj>(rand() % OBJ_P);
      s = static_cast<mapObj>(rand() % OBJ_P);
      e = static_cast<mapObj>(rand() % OBJ_P);
      w = static_cast<mapObj>(rand() % OBJ_P);
      direction = static_cast<dir>(rand() % MOV_P);
    }

    // Returns true if matching
    bool match(mapObj nr, mapObj sr, mapObj er, mapObj wr)
    {
      bool matching = true;
      if(!(nr == n) && !(n == ROBOT))
        matching = false;
      if(!(sr == s) && !(s == ROBOT))
        matching = false;
      if(!(er == e) && !(e == ROBOT))
        matching = false;
      if(!(wr == w) && !(w == ROBOT))
        matching = false;

      return matching;
    }
  };

  int mutationChance; // % Chance that a mutation occurs during combination
  int size;
  Gene* genes;
  // Total possibilities
  static const int OBJ_P = 4;
  static const int MOV_P = 5;
};

// Prints out genes configuration
ostream& operator<<(ostream& out,const DNA& dna)
{
  // Arrays to convert enum to string for output
  string obj[] = {"Empty", "Wall", "Battery", "Don't Care"};
  string dir[] = {"North", "South", "East", "West","Random"};
  for(int i = 0; i < dna.size; i++)
  {
    out << "[" << setw(10) << obj[dna.genes[i].n] << ", ";
    out << setw(10) << obj[dna.genes[i].s] << ", ";
    out << setw(10) << obj[dna.genes[i].e] << ", ";
    out << setw(10) << obj[dna.genes[i].w] << ", ";
    out << setw(10) << dir[dna.genes[i].direction] << "]";
    out << endl;
  }

  return out;
}

class Robot
{
friend ostream& operator<<(ostream& out, const Robot& rhs);

public:
  // Default constructor
  Robot()
  {

  }

  // Constructor
  Robot(int mutChance, int dnaSize, Map* map): robotDNA(mutChance, dnaSize),
  gameMap(map),
  fitness(0),
  energy(5),
  generation(0)
  {}

  // Constructor specifically for adding two robots
  Robot(Map* map, DNA dna):
  robotDNA(dna),
  gameMap(map),
  fitness(0),
  energy(5),
  generation(0),
  robX(0),
  robY(0)
  {}

  // Copy constructor
  Robot(const Robot& rhs): robotDNA(rhs.robotDNA),
  gameMap(rhs.gameMap),
  fitness(rhs.fitness),
  energy(rhs.energy),
  generation(rhs.generation),
  robX(rhs.robX),
  robY(rhs.robY)
  {}

  // Deconstructor
  ~Robot()
  {
    gameMap = nullptr;
  }

  // Operator = overload
  Robot operator=(const Robot& rhs)
  {
    robotDNA = rhs.robotDNA;
    gameMap = rhs.gameMap;
    fitness = rhs.fitness;
    energy = rhs.energy;
    generation = rhs.generation;
    robX = rhs.robX;
    robY = rhs.robY;

    return *this;
  }

  // Operator + overload
  Robot operator+(const Robot& rhs)
  {
    Robot tempRobot(rhs.getMap(), robotDNA + rhs.getDNA());

    return tempRobot;
  }

  // Operator < overload
  bool operator<(const Robot& rhs)
  {
    return fitness < rhs.getFitness();
  }

  // Create random spawn for robot
  void createSpawn()
  {
    int x,y;
    bool badSpot = true;

    // If counter greater than errorCount, infinite loop may have occured
    int errorCount = 100000;
    int counter = 0;
    // While no good spot found
    while(badSpot)
    {
      // Get random spot on map
      x = (rand() % (gameMap->getW() - 2)) + 2;
      y = (rand() % (gameMap->getH() - 2)) + 2;

      // If spot empty, place robot
      if(gameMap->get(x,y) == EMPTY)
      {
        robX = x;
        robY = y;

        badSpot = false;

        gameMap->set(robX,robY,ROBOT);
      }

      counter++;
      if(counter > errorCount)
      {
        cout << "Over " << errorCount << " loops occured";
        cout << " in robot placement. Stopping program." << endl;
        cout << "This suggests an infinite loop occured.";
        cout << " Please change input values." << endl;
        throw;
      }
    }
  }

  // Simulates the entire robots life, resetting initial values
  // Returns fitness
  // If show is true, shows map after each move
  // At rate fps, frames per second
  int simulate(bool show = false, float fps = 1)
  {
    createSpawn();

    energy = 5;
    fitness = 0;

    if(show)
    {
      cout << *gameMap << endl;

      // Wait some time after each map output
      chrono::nanoseconds time = chrono::nanoseconds((int)((1.0 / fps) * 1000000000));

      this_thread::sleep_for(time);
    }

    while(energy > 0)
    {
      move();

      if(show)
      {
        cout << *gameMap << endl;

        // Wait some time after each map output
        chrono::nanoseconds time = chrono::nanoseconds((int)((1.0 / fps) * 1000000000));

        this_thread::sleep_for(time);
      }
    }

    return fitness;
  }

  // Returns number generations survived
  int getGen()
  {
    return generation;
  }

  // Gets for various values
  DNA getDNA() const
  {
    return robotDNA;
  }

  Map* getMap() const
  {
    return gameMap;
  }

  int getFitness() const
  {
    return fitness;
  }

  void addGen()
  {
    generation++;
  }

private:
  DNA robotDNA;
  Map* gameMap;
  int fitness; // Batteries found
  int energy;
  int generation; // Generations survived
  int robX;
  int robY;


  // Simulates a single move for the robot
  void move()
  {
    const int ENERGY_PER_BATTERY = 5;

    // Energy always goes down by one whenever a move is attempted
    energy--;

    // Get surrounding map values
    mapObj e = gameMap->get(robX + 1,robY);
    mapObj w = gameMap->get(robX - 1,robY);
    mapObj n = gameMap->get(robX,robY + 1);
    mapObj s = gameMap->get(robX,robY - 1);

    // Get direction
    dir direction = robotDNA.getDir(n, s, e, w);

    int newX = robX;
    int newY = robY;

    int randVal;
    int randDir;


    // Decide which direction we go by changing new x,y
    switch(direction)
    {
      case NORTH:
        newY++;
        break;
      case SOUTH:
        newY--;
        break;
      case EAST:
        newX++;
        break;
      case WEST:
        newX--;
        break;
      case RANDOM:
        randVal = rand() % 2;
        randDir = rand() % 2;

        // Randomly decide which direction we go
        if(randDir)
        {
          if(randVal)
          {
            newY++;
          } else {
            newY--;
          }
        } else {
          if(randVal)
          {
            newX++;
          } else {
            newX--;
          }
        }
        break;
      default:
        cout << "Error in robot move() switch direction" << endl;
        throw;
    }

    // Get object at new direction
    mapObj newSpace = gameMap->get(newX,newY);


    // Decide movement based on object in our direction
    switch(newSpace)
    {
      case BATTERY:
        // Add energy and fitness because we found battery
        energy += ENERGY_PER_BATTERY;
        fitness++;
        // Drop down to move to new space
      case EMPTY:
        // Move robot to new space
        gameMap->set(robX,robY,EMPTY); // Set previous location to empty
        robX = newX;
        robY = newY;
        gameMap->set(robX,robY,ROBOT); // Set new location to robot
        break;
      case WALL:
        // If wall, do nothing, drop down to do nothing robot
      case ROBOT:
        //energy = 0;
        // If robot, do nothing
        break;
      default:
        cout << "Error in robot move() switch movement" << endl;
    }
  }
};

ostream& operator<<(ostream& out, const Robot& rhs)
{
  out << "Generation: " << rhs.generation << endl;
  out << "Fitness: " << rhs.fitness << endl;
  out << "Energy: " << rhs.energy << endl;
  out << "Genes: " << endl << rhs.robotDNA << endl;

  return out;
}

// Function Prototypes
void ProgramGreeting();
void Unittest();
void graph(int w, int h, vector<float>& arr);
int getInt(int min, int max);
void clrScreen();
void createHeader(string header, char character = '=');

int main() {
	Unittest();
	cout << endl;
	ProgramGreeting();

  // Random number init
  srand(time(NULL));

  // Constants (not really const, just because we allow users to edit them)
  // Map constants
  float PERCENT_BATTERY = 40;
  float PERCENT_WALL = 20;
  int PERCENT_VARIATION = 0; // Not used
  int MAP_SIZE = 12;
  // Robot constants
  int MUTATION_CHANCE = 5;
  int DNA_SIZE = 16;
  int NUM_ROBOTS = 200; // Must be EVEN


  string input;
  do{
    bool settings = true;

    cout << endl;

    do{
      // Menu
      createHeader("Map Settings");
      cout << "1. Change Percent Battery" << " Currently: " << PERCENT_BATTERY << "%" << endl;
      cout << "2. Change Percent Wall" << " Currently: " << PERCENT_WALL << "%" << endl;
      cout << "3. Change Map Size" << " Currently: " << MAP_SIZE << endl;
      cout << endl;
      createHeader("Robot Settings");
      cout << "4. Change Mutation Chance" << " Currently: " << MUTATION_CHANCE << "%" << endl;
      cout << "5. Change DNA Size" << " Currently: " << DNA_SIZE << endl;
      cout << "6. Change Population Size" << " Currently: " << NUM_ROBOTS << endl;

      cout << endl;
      cout << "7. Next" << endl;
      cout << endl;
      cout << "Choice: ";
      int choice = getInt(1,7);
      clrScreen();
      switch(choice)
      {
        case 1:
          cout << "Changing percent of map that is batteries." << endl;
          cout << "Note: Percent Battery + Percent Wall must be less than 100%" << endl;
          cout << endl;
          cout << "Input: ";
          PERCENT_BATTERY = getInt(0,99);
          break;
        case 2:
          cout << "Changing percent of map that is walls." << endl;
          cout << "Note: Percent Battery + Percent Wall must be less than 100%" << endl;
          cout << endl;
          cout << "Input: ";
          PERCENT_WALL = getInt(0,99);
          break;
        case 3:
          cout << "Changing map size." << endl;
          cout << "Since walls surround edges, playabale area for the robot ";
          cout << "will be map size - 2." << endl;
          cout << endl;
          cout << "Input: ";
          MAP_SIZE = getInt(5,100);
          break;
        case 4:
          cout << "Changing mutation chance." << endl;
          cout << "The chance of mutation per child robot." << endl;
          cout << endl;
          cout << "Input: ";
          MUTATION_CHANCE = getInt(0,100);
          break;
        case 5:
          cout << "Changing DNA size." << endl;
          cout << "Number of genes per robot." << endl;
          cout << endl;
          cout << "Input: ";
          DNA_SIZE = getInt(4,100);
          break;
        case 6:
          cout << "Changing number of robots." << endl;
          cout << "Population size that stays constant throughout simulation." << endl;
          cout << "Note: Must be divisible by 4." << endl;
          cout << endl;
          cout << "Input: ";
          NUM_ROBOTS = getInt(20,1000);
          break;
        case 7:
          settings = false;
          break;
        default:
          cout << "Error in menu switch" << endl;
          throw;
      }

      clrScreen();
    }while(settings);

    createHeader("Simulation");
    cout << "Would you like to simulate..." << endl;
    cout << endl;
    cout << "1. till average fitness levels off." << endl;
    cout << "2. for an inputted number of loops." << endl;
    cout << endl;
    int choice = getInt(1,2);

    int loops = 6000;

    bool levelOff = false;
    clrScreen();
    if(choice == 1)
    {
      cout << "Simulation will stop after fitness levels off." << endl;
      cout << "Expected stop between 1000 and 5000 generations." << endl;
      cout << "Forced stop at 6000." << endl;
      levelOff = true;
    } else {
      cout << "Choosing number of loops. Suggested between 1000-3000." << endl;
      cout << endl;
      loops = getInt(0,10000);
    }
    cout << endl;
    string temp;
    cout << "Every thing is set!" << endl;
    cout << "Press enter to continue and run simulation." << endl;
    getline(cin, temp);
    clrScreen();

    // Single map for aggregation in robots
    Map* gameMap = new Map(PERCENT_BATTERY,PERCENT_WALL,PERCENT_VARIATION,MAP_SIZE);

    // Vector to hold all our fitness scores per generation
    vector<float> genFitness;

    // All of our robots
    Robot myRobots[NUM_ROBOTS];
    // Robot init loop
    for(int i = 0; i < NUM_ROBOTS; i++)
    {
      myRobots[i] = Robot(MUTATION_CHANCE, DNA_SIZE,gameMap);
    }

    // Data for figuring out when to end based on fitness gain
    float rateOfChange = 0;
    float currentAvg = 0;
    float pastAvg = 0;
    int counter = 0;
    float expectedRateOfChange = 0.2;

    for(int i = 0; i < loops; i++)
    {
      float avgFitness = 0; // This generations avg fitness

      // Simulate all robots
      for(int j = 0; j < NUM_ROBOTS; j++)
      {
        // Generate a new map for this robot
        gameMap->generate();
        // Get their fitness from simulate
        avgFitness += myRobots[j].simulate(false);
      }

      // Get and record avg fitness
      avgFitness /= NUM_ROBOTS;
      currentAvg += avgFitness / 100;
      genFitness.push_back(avgFitness);

      // Sort robots
      sort(myRobots, myRobots + NUM_ROBOTS);

      if(i % 100 == 0)
      {
        clrScreen();
        cout << "Loop: " << i << " Avg. Fitness: " << avgFitness << endl;

        // Rate of change calculations
        if(levelOff)
        {
          rateOfChange = (currentAvg - pastAvg);
          // If our rate of change is less than what we expect,
          // then we have likely leveled off
          if(rateOfChange < expectedRateOfChange)
          {
            counter++;
            // If we havn't increased a number of times, end simulation
            if(counter == 5)
              i = loops - 1;
          } else {
            counter = 0;
          }
        }

        pastAvg = currentAvg;
        currentAvg = 0;
      }



      // If last loop, don't reproduce
      if(i == loops - 1)
        continue;
      // Reproduce
      // Only loop for greater half of robots
      // and jump by 2 because we breed two at a time
      int half = NUM_ROBOTS / 2;
      for(int j = half; j < NUM_ROBOTS; j+=2)
      {
        // Our parents survived a generation
        myRobots[j].addGen();
        myRobots[j + 1].addGen();
        // Child 1
        myRobots[j - half] = myRobots[j] + myRobots[j + 1];
        // Child 2
        myRobots[j - half + 1] = myRobots[j + 1] + myRobots[j];
      }
    }

    clrScreen();
    createHeader("Summary Statistics");
    cout << endl;
    cout << "First Fitness: " << genFitness[0] << endl;
    cout << "Last Fitness: " << genFitness[genFitness.size() - 1] << endl;
    cout << endl;

    bool summary = true;

    do{
      // Statistics Menu
      cout << "1. See Best Robot" << endl;
      cout << "2. See Worst Robot" << endl;
      cout << "3. See Average Fitness Graph" << endl;
      cout << "4. Watch Best Robot Simulation" << endl;
      cout << "5. Continue" << endl;

      cout << endl;
      choice = getInt(1,5);
      clrScreen();
      switch(choice)
      {
        case 1:
          createHeader("Best Robot");
          cout << myRobots[NUM_ROBOTS - 1] << endl;
          break;
        case 2:
          createHeader("Worst Robot");
          cout << myRobots[0] << endl;
          break;
        case 3:
          createHeader("Graph");
          graph(60,20,genFitness);
          break;
        case 4:
          gameMap->generate();
          cout << "Fitness: ";
          cout << myRobots[NUM_ROBOTS - 1].simulate(true, 5) << endl;
          break;
        case 5:
          summary = false;
          break;
        default:
          cout << "Error in summary switch" << endl;
          throw;
      }
      cout << endl;
    }while(summary);

    cout << "Run another simulation? Y/N" << endl;
    getline(cin, input);
    clrScreen();
    // Cleanup
    delete gameMap;
  }while(input == "Y");


  cout << "Good bye!" << endl;

  return 0;
}


void ProgramGreeting() {
	cout << "Welcome to " << "my final, Genetic Algorithm Robots" << "." << endl;
	cout << "Choose some options and watch them evolve." << endl;
}

void Unittest() {
  Grid<int>::ComponentTest();
}

void graph(int w, int h, vector<float>& arr)
{
  // Minus 1 to width at first because this section makes the values
  // for actual graph portion
  w--;

  // Vector size
  int size = arr.size();

  // Number of inputs that are compacted to one value
  int compact = floor(float(size) / w);
  // Left over inputs to compact into the last value
  int leftOver = size % w;

  // Store the largest avg value for the graph height calculations
  int highestVal = 0;

  // Array to hold our compacted values
  float compArr[w];

  for(int i = 0; i < w; i++)
  {
    float tempVal = 0;

    // If last value, use leftOver instead of compact
    if(i == w - 1)
    {
      for(int j = 0 + compact * i; j < leftOver + compact * i; j++)
      {
        tempVal += arr[j];
      }
      tempVal /= leftOver;
    } else
    {
      // Compact set of values into 1
      for(int j = 0 + compact * i; j < compact + compact * i; j++)
      {
        tempVal += arr[j];
      }
      tempVal /= compact;
    }

    if(highestVal < tempVal)
      highestVal = ceil(tempVal);

    compArr[i] = tempVal;
  }

  string graph = "Average Fitness vs Generation\n";
  graph += to_string(highestVal);
  graph += "\n";

  // Add 1 back to w now that we are building entire graph visual
  w++;

  // Height
  for(int i = 0; i < h; i++)
  {
    // Width
    for(int j = 0; j < w; j++)
    {
      // Left edge
      if(j == 0)
      {
        graph += "|";
      // Bottom edge
      } else  if(i == h - 1){
        graph += "=";
        if(j == w - 1)
        {
          graph += to_string(arr.size());
        }
      // Calculate if our graph point should be at this height
      } else if(ceil((compArr[j - 1] / float(highestVal)) * (h - 1)) == abs(i - (h - 1))){
        graph += "#";
      } else {
        graph += " ";
      }
    }
    graph += "\n";
  }

  cout << graph;
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
