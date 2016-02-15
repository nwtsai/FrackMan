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
	int min(int a, int b);
	int max(int a, int b);
	int randInt(int l, int h);
	void setDisplayText();
private:
	vector <Dirt*> m_dirt;
	vector<Actor*> m_actors;
	FrackMan* fracker;
	int m_score;
	int m_level;
	int m_barrelsLeft;
};

#endif // STUDENTWORLD_H_
