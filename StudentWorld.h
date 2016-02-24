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
	bool isCloseToAnyActor(int x, int y);
	virtual bool isCollidingWith(int x, int y, Actor* obj);
	bool canMoveHere(int x, int y);
	bool canStepHere(int x, int y, GraphObject::Direction dir);
	bool isThereDirtHere(int x, int y);
	bool isThereDirtInThisBox(int x, int y);
	bool isThereBoulderInThisBox(int x, int y);
	bool isThereBoulderUnderMe(int x, int y);
	bool isCollidingWithBoulder(int x, int y);
	int isCollidingWithAProtester(int x, int y);
	void annoyProtester(int objX, int objY, char cause);
	void annoyHardcoreProtester(int objX, int objY, char cause);
	bool isWithinShoutingDistance(int x, int y);
	bool isFacingFrackMan(int x, int y, GraphObject::Direction dir);
	GraphObject::Direction faceTheFrack(int x, int y);
	GraphObject::Direction getRandDir();
	GraphObject::Direction canTurn(int x, int y, GraphObject::Direction dir); // returns none if can't turn
	bool isInLineOfSight(int x, int y);
	void annoyFrackMan(char cause);
	void destroyDirt(int x, int y);
	void removeDeadObjects();
	bool isBlocked(int x, int y);
	void findBestPathToTopRight(); // need to change
	void findBestPathToHere(int x, int y);
	void findPath(int x, int y, GraphObject::Direction dir, int step); // need to change
	GraphObject::Direction getDirMap(int x, int y);
	int getStepMap(int x, int y);
	int getFrackerX();
	int getFrackerY();

	void addBoulders();
	void addNuggets();
	void dropNugget(int x, int y);
	void addBarrels();
	void addSonarKit();
	void addWaterPool();
	void insertSquirt(int x, int y, GraphObject::Direction dir);
	void addProtester();
	void addHardcoreProtester();

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
	Dirt* m_dirt[64][64];
	vector<Actor*> m_actors;
	FrackMan* fracker;
	int m_barrelsLeft;
	Actor::Direction dirMap[61][61];
	int stepMap[61][61];
};

#endif // STUDENTWORLD_H_
