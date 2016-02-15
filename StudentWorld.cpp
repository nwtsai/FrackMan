#include "StudentWorld.h"
#include <string>
#include <vector>
#include <cstdlib>
#include "Actor.h"

using namespace std;

/*GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}*/

StudentWorld::StudentWorld()
	: m_counter(0)
{}

StudentWorld::~StudentWorld()
{
	for (int i = 0; i < m_actors.size(); i++)
		delete m_actors.at(i);
	m_actors.clear();
}

int StudentWorld::init()
{
	FrackMan* fracker = new FrackMan(this); //create the player and put it into the vector
	m_actors.push_back(fracker);


	// return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	decLives();
	return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
}

