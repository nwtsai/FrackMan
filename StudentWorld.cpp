#include "StudentWorld.h"
#include <string>
#include <vector>
#include <cstdlib>
#include "Actor.h"

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
	for (int i = 0; i < m_actors.size(); i++)
		delete m_actors.at(i);
	m_actors.clear();

	for (int k = 0; k < m_dirt.size(); k++)
		delete m_dirt.at(k);
	m_dirt.clear();

	delete fracker;
}

int StudentWorld::init()
{
	fracker = new FrackMan(this); 
	// m_actors.push_back(fracker);

	for (int r = 0; r < 64; r++)
	{
		for (int c = 0; c < 60; c++)
		{
			if (r < 30 || r > 33)
			{
				Dirt* dirt = new Dirt(r, c, this);
				m_dirt.push_back(dirt);
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	//setGameStatText();
	fracker->doSomething();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i < m_actors.size(); i++)
		delete m_actors.at(i);
	m_actors.clear();

	for (int k = 0; k < m_dirt.size(); k++)
		delete m_dirt.at(k);
	m_dirt.clear();

	delete fracker;
}

void StudentWorld::destroyDirt(int x, int y)
{
	vector<Dirt*>::iterator p = m_dirt.begin();
	while (p != m_dirt.end())
	{
		if ((*p)->getX() - x >= 0 && (*p)->getX() - x < 4 && (*p)->getY() - y >= 0 && (*p)->getY() - y < 4)
		{
			vector<Dirt*>::iterator temp = p;
			delete *p;
			p = m_dirt.erase(temp);
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
	
}