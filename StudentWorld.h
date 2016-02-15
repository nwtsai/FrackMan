#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include "Actor.h"

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void destroyDirt(int x, int y);
private:
	// int m_counter;
	vector <Dirt*> m_dirt;
	vector<Actor*> m_actors;
	FrackMan* fracker;
};

#endif // STUDENTWORLD_H_
