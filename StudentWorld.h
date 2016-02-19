#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <vector>
#include "Actor.h"
#include <iomanip>
#include <sstream>

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
	virtual bool isCloseTo(int x, int y, Actor* obj);
	virtual bool isCloseToAnyActor(int x, int y);
	virtual bool isCollidingWith(int x, int y, Actor* obj);
	virtual bool canMoveHere(int x, int y);
	bool isThereDirtHere(int x, int y);
	void destroyDirt(int x, int y);
	void removeDeadObjects();

	void addBoulders();
	void addNuggets();
	void dropNugget(int x, int y);
	void addBarrels();
	void addSonarKits();
	void addWaterPools();
	void insertSquirt(int x, int y, GraphObject::Direction dir);
	
	int min(int a, int b);
	int max(int a, int b);
	int randInt(int l, int h);
	void setDisplayText();
	string formatDisplayText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft);
	bool isWithinRadius(int x1, int y1, int x2, int y2, double r);
	void reduceBarrels();
	FrackMan* getFrack();
	void revealCloseObjects(int x, int y);
private:
	// vector <Dirt*> m_dirt;
	Dirt* m_dirt[64][64];
	vector<Actor*> m_actors;
	FrackMan* fracker;
	int m_barrelsLeft;
};

#endif // STUDENTWORLD_H_
