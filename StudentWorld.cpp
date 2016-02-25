#include "GameController.h"
#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{}

StudentWorld::~StudentWorld()
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		vector<Actor*>::iterator temp = p;
		delete *p;
		p = m_actors.erase(temp);
	}

	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			if (m_dirt[x][y] != nullptr)
			{
				delete m_dirt[x][y];
				m_dirt[x][y] = nullptr;
			}
		}
	}

	delete fracker;
}

int StudentWorld::init()
{
	maxNumProtesters = min(15, (int)(2 + getLevel() * 1.5));
	numBarrels = min(2 + getLevel(), 20);
	numProtesters = 0;
	protesterTick = 0;
	hardCoreMaxVal = 16 + getLevel() * 2;
	fracker = nullptr;

	// initialize the dirt
	for (int r = 0; r < 64; r++)
	{
		for (int c = 0; c < 64; c++)
		{
			if ((r < 30 || r > 33 || c < 4) && c < 60)
			{
				m_dirt[r][c] = new Dirt(r, c);
			}
			else
			{
				m_dirt[r][c] = nullptr;
			}
		}
	}

	// initialize the FrackMan
	fracker = new FrackMan(this);

	// initialize the Boulders
	addBoulders();

	// initialize the Gold Nuggets
	addNuggets();

	// initialize the Barrels
	addBarrels();

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	protesterTick--;

	solveGrid(60, 60, grid, INT16_MAX);
	solveGrid(fracker->getX(), fracker->getY(), frackGrid, hardCoreMaxVal);

	// update status text
	setDisplayText();
	
	// if allowed to add protester, add a protester, increment number of protesters, and reset the protesterTick
	if (protesterTick <= 0 && numProtesters < maxNumProtesters) 
	{
		addProtester();
		protesterTick = max(25, 200 - getLevel());
		numProtesters++;
	}

	// add Sonar or Water
	addSonarOrWater();
	
	// ask frackman to do something
	fracker->doSomething();

	// ask every object to do something
	for (size_t i = 0; i < m_actors.size(); i++)
	{
		if (m_actors.at(i)->isStillAlive())
		{
			m_actors.at(i)->doSomething();

			if (!fracker->isStillAlive())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}

			if (numBarrels == 0)
			{
				GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}

	removeDeadObjects();

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		vector<Actor*>::iterator temp = p;
		delete *p;
		p = m_actors.erase(temp);
	}

	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			if (m_dirt[x][y] != nullptr)
			{
				delete m_dirt[x][y];
				m_dirt[x][y] = nullptr;
			}
		}
	}

	delete fracker;
}

bool StudentWorld::isCloseTo(int x, int y, Actor* obj)
{
	return isWithinRadius(x, y, obj->getX(), obj->getY(), 6.0);
}

bool StudentWorld::isCloseToAnyActor(int x, int y)
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		if (isCloseTo(x, y, *p))
		{
			return true;
		}
		p++;
	}
	return false;
}

bool StudentWorld::isCollidingWith(int x, int y, Actor* act)
{
	//cout << x << " " << y << " " << act->getX() << " " << act->getY() << endl;
	return isWithinRadius(x, y, act->getX(), act->getY(), 3.0);
}

bool StudentWorld::canMoveHere(int x, int y)
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		if ((*p)->doesThisBlock() == true && isCollidingWith(x, y, *p))
		{
			return false;
		}
		p++;
	}
	return true;
}

bool StudentWorld::canStepHere(int x, int y, GraphObject::Direction dir)
{
	if (dir == Actor::right)
	{
		if (!clearOfDirtBoulder(x + 1,y))
			return false;
	}
	else if (dir == Actor::left)
	{
		if (!clearOfDirtBoulder(x - 1, y))
			return false;
	}
	else if (dir == Actor::up)
	{
		if (!clearOfDirtBoulder(x, y + 1))
			return false;
	}
	else if (dir == Actor::down)
	{
		if (!clearOfDirtBoulder(x, y - 1))
			return false;
	}
	return true;
}

bool StudentWorld::isThereDirtHere(int x, int y)
{
	if (x < 0 || y < 0 || x > 63 || y > 63)
		return false;

	if (m_dirt[x][y] != nullptr)
		return true;
	return false;
}

bool StudentWorld::isThereDirtInThisBox(int x, int y)
{
	if (x < 0 || y < 0 || x > 60 || y > 60)
		return false;

	for (int i = x; i < x + 4; i++)
	{
		for (int j = y; j < y + 4; j++)
		{
			if (m_dirt[i][j] != nullptr)
				return true;
		}
	}
	return false;
}

bool StudentWorld::isThereBoulderInThisBox(int x, int y)
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		// if the actor is a Boulder
		if ((*p)->doesThisBlock())
		{
			for (int i = x - 3; i < x + 4; i++)
			{
				for (int j = y - 3; j < y + 4; j++)
				{
					if (i == (*p)->getX() && j == (*p)->getY())
						return true;
				}
			}
		}
		p++;
	}
	return false;
}

bool StudentWorld::clearOfDirtBoulder(int x, int y)
{
	if (x > 60 || x < 0)
		return false;

	if (y > 60 || y < 0)
		return false;

	// checks for dirt
	if (isThereDirtInThisBox(x, y))
		return false;

	//checks for boulder
	if (isCollidingWithBoulder(x, y))
		return false;

	return true;
}

bool StudentWorld::isThereBoulderUnderMe(int x, int y)
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		// if the actor is a Boulder
		if ((*p)->doesThisBlock())
		{
			for (int i = x - 3; i < x + 4; i++)
			{
				if (i == (*p)->getX() && y - 4 == (*p)->getY())
					return true;
			}
		}
		p++;
	}
	return false;
}

bool StudentWorld::isCollidingWithBoulder(int x, int y)
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		// if the actor is a Boulder
		if ((*p)->doesThisBlock())
		{
			if (isCollidingWith(x, y, *p))
				return true;
		}
		p++;
	}
	return false;
}

int StudentWorld::annoyAProtester(int x, int y, char cause)
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		// if the actor is a Regular Protester
		if ((*p)->isProtester() == 1)
		{
			if (isCollidingWith(x, y, *p))
			{
				(*p)->getAnnoyed(cause);
				return 1;
			}
		}
		// if the actor is a Hardcore Protester
		else if ((*p)->isProtester() == 2)
		{
			if (isCollidingWith(x, y, *p))
			{
				(*p)->getAnnoyed(cause);
				return 2;
			}
		}
		p++;
	}
	// if the actor is not any type of protester
	return 0;
}

bool StudentWorld::isWithinShoutingDistance(int x, int y)
{
	if (isWithinRadius(x, y, fracker->getX(), fracker->getY(), 4.0))
		return true;
	return false;
}

bool StudentWorld::isFacingFrackMan(int x, int y, GraphObject::Direction dir)
{
	int fx = fracker->getX();
	int fy = fracker->getY();

	if (x < fx && dir == Actor::right) 
		return true;
	else if (x > fx && dir == Actor::left) 
		return true;
	else if (y < fy && dir == Actor::up)
		return true;
	else if (y > fy && dir == Actor::down) 
		return true;

	return false;
}

GraphObject::Direction StudentWorld::faceTheFrack(int x, int y)
{
	int fx = fracker->getX();
	int fy = fracker->getY();

	if (x < fx)
		return Actor::right; 
	else if (x > fx) 
		return Actor::left;
	else if (y < fy)
		return Actor::up;
	else if (y > fy)
		return Actor::down;
	else
		return Actor::up; // if the coordinates x and y are directly on top of the frackman, return up as default
}

GraphObject::Direction StudentWorld::getRandDir()
{
	int num = randInt(1, 4);
	if (num == 1)
		return Actor::up;
	else if (num == 2)
		return Actor::down;
	else if (num == 3)
		return Actor::left;
	else // num == 4
		return Actor::right;
}

GraphObject::Direction StudentWorld::canTurn(int x, int y, GraphObject::Direction dir) // returns none if can't turn
{
	if (dir == Actor::up || dir == Actor::down)
	{
		// choose a random direction
		if (canStepHere(x, y, Actor::left) && canStepHere(x, y, Actor::right))
		{
			int num = randInt(1, 2);
			if (num == 1)
				return Actor::left;
			else
				return Actor::right;
		}
		else if (canStepHere(x, y, Actor::left))
			return Actor::left;
		else if (canStepHere(x, y, Actor::right))
			return Actor::right;
		else
			return Actor::none;
	}
	else // if (dir == Actor::left || dir == Actor::right)
	{
		// choose a random direction
		if (canStepHere(x, y, Actor::up) && canStepHere(x, y, Actor::down))
		{
			int num = randInt(1, 2);
			if (num == 1)
				return Actor::up;
			else
				return Actor::down;
		}
		else if (canStepHere(x, y, Actor::up))
			return Actor::up;
		else if (canStepHere(x, y, Actor::down))
			return Actor::down;
		else
			return Actor::none;
	}

}

bool StudentWorld::isInLineOfSight(int x, int y)
{
	int fx = fracker->getX();
	int fy = fracker->getY();

	int lessX;
	int biggerX;
	int lessY;
	int biggerY;

	if (x == fx)
	{
		if (y < fy)
		{
			lessY = y;
			biggerY = fy - 1;
		}
		else
		{
			lessY = fy + 4;
			biggerY = y;
		}

		for (int j = lessY; j < biggerY; j++)
		{
			if (isThereDirtInThisBox(x, j) || isThereBoulderInThisBox(x, j))
				return false;
		}
	}
	else if (y == fy)
	{
		if (x < fx)
		{
			lessX = x;
			biggerX = fx - 1;
		}
		else
		{
			lessX = fx + 4;
			biggerX = x;
		}
			
		for (int i = lessX; i < biggerX; i++)
		{
			if (isThereDirtInThisBox(i, y) || isThereBoulderInThisBox(i, y))
				return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

void StudentWorld::annoyFrackMan(char cause)
{
	fracker->getAnnoyed(cause);
}

void StudentWorld::destroyDirt(int x, int y)
{
	for (int i = x; i <= x + 3; i++)
	{
		for (int j = y; j <= y + 3; j++)
		{
			if (m_dirt[i][j] != nullptr && j < 60 && isThereDirtHere(i, j))
			{
				m_dirt[i][j]->setVisible(false);
				playSound(SOUND_DIG);
				delete m_dirt[i][j];
				m_dirt[i][j] = nullptr;
			}
		}
	}
}

GraphObject::Direction StudentWorld::protesterGiveUp(int &x, int &y)
{
	if (grid[x][y] == Actor::up)
	{
		y++;
		return Actor::up; // up
	}
	else if (grid[x][y] == Actor::right) 
	{
		x++;
		return Actor::right; // right
	}
	else if (grid[x][y] == Actor::down) 
	{
		y--;
		return Actor::down; // down
	}
	else if (grid[x][y] == Actor::left) 
	{
		x--;
		return Actor::left; // left
	}
	else
		return Actor::none;
}

GraphObject::Direction StudentWorld::getIntimateWithFrack(int &x, int &y) 
{
	if (frackGrid[x][y] == Actor::up) 
	{
		y++;
		return Actor::up; 
	}
	else if (frackGrid[x][y] == Actor::right) 
	{
		x++;
		return Actor::right; 
	}
	else if (frackGrid[x][y] == Actor::down) 
	{
		y--;
		return Actor::down; 
	}
	else if (frackGrid[x][y] == Actor::left) 
	{
		x--;
		return Actor::left; 
	}
	else
		return Actor::none;
}

void StudentWorld::addBoulders()
{
	int numBoulders = min(getLevel() / 2 + 2, 6);
	for (int boul = 0; boul < numBoulders; boul++)
	{
		int randX = randInt(0, 60);
		int randY = randInt(20, 56);

		// If it is the first Actor, check if it is in mine shaft
		while (randX >= 26 && randX <= 33)
		{
			randX = randInt(0, 60);
		}

		vector<Actor*>::iterator p = m_actors.begin();
		while (p != m_actors.end())
		{
			if (isCloseToAnyActor(randX, randY) || (randX >= 26 && randX <= 33))
			{
				randX = randInt(0, 60);
				randY = randInt(20, 56);
			}
			else
			{
				p++;
			}
		}

		m_actors.push_back(new Boulder(randX, randY, this, fracker));
	}
}

void StudentWorld::addNuggets()
{
	int numNugs = max(5 - getLevel() / 2, 2);
	for (int nugs = 0; nugs < numNugs; nugs++)
	{
		int randX = randInt(0, 60);
		int randY = randInt(0, 56);

		// If it is the first Actor, check if it is in mine shaft
		while (randX >= 26 && randX <= 33)
		{
			randX = randInt(0, 60);
		}

		vector<Actor*>::iterator p = m_actors.begin();
		while (p != m_actors.end())
		{
			if (isCloseToAnyActor(randX, randY) || (randX >= 26 && randX <= 33))
			{
				randX = randInt(0, 60);
				randY = randInt(0, 56);
			}
			else
			{
				p++;
			}
		}

		m_actors.push_back(new GoldNugget(randX, randY, true, this, fracker));
	}
}

void StudentWorld::dropNugget(int x, int y)
{
	m_actors.push_back(new GoldNugget(x, y, false, this, fracker));
}

void StudentWorld::addBarrels()
{
	int numBarrels = min(2 + getLevel(), 20);
	for (int barrels = 0; barrels < numBarrels; barrels++)
	{
		int randX = randInt(0, 60);
		int randY = randInt(0, 56);

		// If it is the first Actor, check if it is in mine shaft
		while (randX >= 26 && randX <= 33)
		{
			randX = randInt(0, 60);
		}

		vector<Actor*>::iterator p = m_actors.begin();
		while (p != m_actors.end())
		{
			if (isCloseToAnyActor(randX, randY) || (randX >= 26 && randX <= 33))
			{
				randX = randInt(0, 60);
				randY = randInt(0, 56);
			}
			else
			{
				p++;
			}
		}

		m_actors.push_back(new Barrel(randX, randY, this, fracker));
	}
}

void StudentWorld::addSonarKit()
{
	SonarKit* sk = new SonarKit(0, 60, this, fracker);
	m_actors.push_back(sk);
}

void StudentWorld::addWaterPool()
{
	int randX = randInt(0, 60);
	int randY = randInt(0, 60);

	while (isThereDirtInThisBox(randX, randY))
	{
		randX = randInt(0, 60);
		randY = randInt(0, 60);
	}

	WaterPool* water = new WaterPool(randX, randY, this, fracker);
	m_actors.push_back(water);
}

void StudentWorld::addSonarOrWater()
{
	int G = getLevel() * 25 + 300;
	if (randInt(1, G) == 1)
	{
		if (randInt(1, 5) == 1)
		{
			addSonarKit();
		}
		else
		{
			addWaterPool();
		}
	}
}

void StudentWorld::insertSquirt(int x, int y, GraphObject::Direction dir)
{
	if (dir == GraphObject::left && !isThereDirtInThisBox(x - 4, y) && !isThereBoulderInThisBox(x - 4, y) && x - 4 >= 0)
		m_actors.push_back(new Squirt(x - 4, y, dir, this, fracker));
	else if (dir == GraphObject::right && !isThereDirtInThisBox(x + 4, y) && !isThereBoulderInThisBox(x + 4, y) && x + 4 <= 60)
		m_actors.push_back(new Squirt(x + 4, y, dir, this, fracker));
	else if (dir == GraphObject::up && !isThereDirtInThisBox(x, y + 4) && !isThereBoulderInThisBox(x, y + 4) && y + 4 <= 60)
		m_actors.push_back(new Squirt(x, y + 4, dir, this, fracker));
	else if (dir == GraphObject::down && !isThereDirtInThisBox(x, y - 4) && !isThereBoulderInThisBox(x, y - 4) && y - 4 >= 0)
		m_actors.push_back(new Squirt(x, y - 4, dir, this, fracker));
}

void StudentWorld::decrementProtesterCount()
{
	numProtesters--;
}

void StudentWorld::addProtester()
{
	int hardcoreP = min(90, getLevel() * 10 + 30);
	if (randInt(1, 100) <= hardcoreP)
	{
		HardcoreProtester* Hpro = new HardcoreProtester(IID_HARD_CORE_PROTESTER, 20, this);
		m_actors.push_back(Hpro);
	}
	else
	{
		Protester* pro = new Protester(IID_PROTESTER, 5, this);
		m_actors.push_back(pro);
	}
}

void StudentWorld::removeDeadObjects()
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		if ((*p)->isStillAlive() == false)
		{
			vector<Actor*>::iterator temp = p;
			delete *p;
			p = m_actors.erase(temp);
		}
		else
		{
			p++;
		}
	}
}

int StudentWorld::min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

int StudentWorld::max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

int StudentWorld::randInt(int l, int h)
{
	if (h < l)
		swap(h, l); 
	return l + (rand() % (h - l + 1)); 
}

void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = fracker->getHP() * 10;
	int squirts = fracker->getSquirts();
	int gold = fracker->getGold();
	int sonar = fracker->getsCharges();
	int barrelsLeft = numBarrels;

	string s = formatDisplayText(score, level, lives, health, squirts, gold, sonar, barrelsLeft);
	setGameStatText(s);
}

string StudentWorld::formatDisplayText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft)
{
	stringstream sa;
	string scr = "Scr: ";
	sa << setw(6) << setfill('0') << score;
	string a = sa.str();

	stringstream sb;
	string lvl = "Lvl: ";
	sb << setw(2) << setfill(' ') << level;
	string b = sb.str();

	stringstream sc;
	string lvs = "Lives: ";
	sc << setw(1) << setfill(' ') << lives;
	string c = sc.str();

	stringstream sd;
	string HP = "Hlth: ";
	sd << setw(3) << setfill(' ') << health;
	string d = sd.str();

	stringstream se;
	string wtr = "Wtr: ";
	se << setw(2) << setfill(' ') << squirts;
	string e = se.str();

	stringstream sf;
	string gld = "Gld: ";
	sf << setw(2) << setfill(' ') << gold;
	string f = sf.str();

	stringstream sg;
	string snr = "Sonar: ";
	sg << setw(2) << setfill(' ') << sonar;
	string g = sg.str();

	stringstream sh;
	string oil = "Oil Left: ";
	sh << setw(2) << setfill(' ') << barrelsLeft;
	string h = sh.str();

	return scr + a + "  " + lvl + b + "  " + lvs + c + "  " + HP + d + "%  " + wtr + e + "  " + gld + f + "  " + snr + g + "  " + oil + h;
}

bool StudentWorld::isWithinRadius(int x1, int y1, int x2, int y2, double r)
{
	double xDiff = x1 - x2;
	double yDiff = y1 - y2;
	double dist = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
	if (dist <= r)
		return true;
	return false;
}

void StudentWorld::reduceBarrels()
{
	numBarrels--;
}

FrackMan* StudentWorld::getFrack()
{
	return fracker;
}

void StudentWorld::revealCloseObjects(int x, int y)
{
	vector<Actor*>::iterator p = m_actors.begin();
	while (p != m_actors.end())
	{
		if (isWithinRadius(x, y, (*p)->getX(), (*p)->getY(), 12.0))
		{
			(*p)->setVisible(true);
		}
		p++;
	}
}

void StudentWorld::solveGrid(int x, int y, Actor::Direction g[64][64], int Manhattanism)
{
	for (int i = 0; i < 64; i++) 
	{
		for (int j = 0; j < 64; j++) 
		{
			g[i][j] = Actor::none;
		}
	}

	queue<Coord> gridQueue;
	gridQueue.push(Coord(x, y, Manhattanism));
	while (!gridQueue.empty()) 
	{
		Coord gridCoord = gridQueue.front();
		gridQueue.pop();
		if (g[gridCoord.r() - 1][gridCoord.c()] == 0 && clearOfDirtBoulder(gridCoord.r() - 1, gridCoord.c()) && gridCoord.z() > 0)
		{
			gridQueue.push(Coord(gridCoord.r() - 1, gridCoord.c(), gridCoord.z() - 1));
			g[gridCoord.r() - 1][gridCoord.c()] = Actor::right;
		}
		if (g[gridCoord.r()][gridCoord.c() - 1] == 0 && clearOfDirtBoulder(gridCoord.r(), gridCoord.c() - 1) && gridCoord.z() > 0)
		{
			gridQueue.push(Coord(gridCoord.r(), gridCoord.c() - 1, gridCoord.z() - 1));
			g[gridCoord.r()][gridCoord.c() - 1] = Actor::up;
		}
		if (g[gridCoord.r() + 1][gridCoord.c()] == 0 && clearOfDirtBoulder(gridCoord.r() + 1, gridCoord.c()) && gridCoord.z() > 0)
		{
			gridQueue.push(Coord(gridCoord.r() + 1, gridCoord.c(), gridCoord.z() - 1));
			g[gridCoord.r() + 1][gridCoord.c()] = Actor::left;
		}
		if (g[gridCoord.r()][gridCoord.c() + 1] == 0 && clearOfDirtBoulder(gridCoord.r(), gridCoord.c() + 1) && gridCoord.z() > 0)
		{
			gridQueue.push(Coord(gridCoord.r(), gridCoord.c() + 1, gridCoord.z() - 1));
			g[gridCoord.r()][gridCoord.c() + 1] = Actor::down;
		}
	}
}