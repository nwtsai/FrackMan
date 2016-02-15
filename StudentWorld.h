#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld::StudentWorld();
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

private:
	int m_counter;
	vector<Dirt*> m_dirt;
	vector<Actor*> m_actors;
	FrackMan* fracker;
};

#endif // STUDENTWORLD_H_
