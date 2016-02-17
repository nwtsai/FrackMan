#include "StudentWorld.h"
#include <string>
#include <vector>
#include <cstdlib>
#include "Actor.h"
#include "GameController.h"
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			m_dirt[i][j] = nullptr;
		}
	}
	m_barrelsLeft = min(2 + getLevel(), 20);
	fracker = nullptr;
}

StudentWorld::~StudentWorld()
{
	for (int i = 0; i < m_actors.size(); i++)
		delete m_actors.at(i);
	m_actors.clear();

	/*for (int k = 0; k < m_dirt.size(); k++) 
		delete m_dirt.at(k);
	m_dirt.clear();*/

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
	// initialize the dirt
	for (int r = 0; r < 64; r++)
	{
		for (int c = 0; c < 60; c++)
		{
			if (r < 30 || r > 33 || c < 4)
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

	SonarKit* sk = new SonarKit(0, 60, this, fracker);
	m_actors.push_back(sk);

	WaterPool* water = new WaterPool(5, 40, this, fracker);
	m_actors.push_back(water);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// update status text
	setDisplayText();

	// ask every object to do something
	fracker->doSomething();
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors.at(i)->isStillAlive())
		{
			m_actors.at(i)->doSomething();

			if (!fracker->isStillAlive())
			{
				return GWSTATUS_PLAYER_DIED;
			}

			if (m_barrelsLeft == 0)
			{
				GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}

	// delete actors that need to be deleted and remove them from the vector
	removeDeadObjects();

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i < m_actors.size(); i++)
		delete m_actors.at(i);
	m_actors.clear();

	/*for (int k = 0; k < m_dirt.size(); k++)
		delete m_dirt.at(k);
	m_dirt.clear();*/

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

bool StudentWorld::isCollidingWith(int x, int y, Actor* obj)
{
	return isWithinRadius(x, y, obj->getX(), obj->getY(), 6.0);
}

bool StudentWorld::isThereDirtHere(int x, int y)
{
	/*vector<Dirt*>::iterator p = m_dirt.begin();
	while (p != m_dirt.end())
	{
		if ((*p)->getX() == x && (*p)->getY() == y)
		{
			return true;
		}
		else
		{
			p++;
		}
	}
	return false;*/

	if (m_dirt[x][y] != nullptr)
		return true;
	return false;
}

void StudentWorld::destroyDirt(int x, int y)
{
	/*vector<Dirt*>::iterator p = m_dirt.begin();
	while (p != m_dirt.end())
	{
		if ((*p)->getX() - x >= 0 && (*p)->getX() - x < 4 && (*p)->getY() - y >= 0 && (*p)->getY() - y < 4)
		{
			vector<Dirt*>::iterator temp = p;
			delete *p;
			p = m_dirt.erase(temp);
			GameController::getInstance().playSound(SOUND_DIG);
		}
		else
		{
			p++;
		}
	}*/

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

void StudentWorld::insertSquirt(int x, int y, int dir)
{
	// Squirt* sqr = new Squirt();
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

void StudentWorld::addBoulders()
{
	int numBoulders = max(getLevel() / 2 + 2, 6);
	for (int boul = 0; boul < numBoulders; boul++)
	{
		int randX = randInt(0, 60);
		int randY = randInt(20, 56);
		
		/*for (int a = 0; a < m_actors.size(); a++)
		{
			while (isCollidingWith(randX, randY, m_actors.at(a)) || (randX >= 26 && randX <= 33))
			{
				randX = randInt(0, 60);
				randY = randInt(20, 56);
			}
		}*/

		vector<Actor*>::iterator p = m_actors.begin();
		while (p != m_actors.end())
		{
			if (isCollidingWith(randX, randY, *p) || (randX >= 26 && randX <= 33))
			{
				randX = randInt(0, 60);
				randY = randInt(20, 56);
			}
			else
			{
				p++;
			}
		}

		m_actors.push_back(new Boulder(randX, randY, this));
	}
}

void StudentWorld::addNuggets()
{
	int numNugs = min(5 - getLevel() / 2, 2);
	for (int nugs = 0; nugs < numNugs; nugs++)
	{
		int randX = randInt(0, 60);
		int randY = randInt(20, 56);

		vector<Actor*>::iterator p = m_actors.begin();
		while (p != m_actors.end())
		{
			if (isCollidingWith(randX, randY, *p) || (randX >= 26 && randX <= 33))
			{
				randX = randInt(0, 60);
				randY = randInt(20, 56);
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
		int randY = randInt(20, 56);

		vector<Actor*>::iterator p = m_actors.begin();
		while (p != m_actors.end())
		{
			if (isCollidingWith(randX, randY, *p) || (randX >= 26 && randX <= 33))
			{
				randX = randInt(0, 60);
				randY = randInt(20, 56);
			}
			else
			{
				p++;
			}
		}

		m_actors.push_back(new Barrel(randX, randY, this, fracker));
	}
}

void StudentWorld::addSonarKits()
{
	
}

void StudentWorld::addWaterPools()
{
	
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
	int health = fracker->getHP();
	int squirts = fracker->getSquirts();
	int gold = fracker->getGold();
	int sonar = fracker->getsCharges();
	int barrelsLeft = m_barrelsLeft;

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
	string wtr = "Water: ";
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
	m_barrelsLeft--;
}