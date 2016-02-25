#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

class Actor;

class StudentWorld : public GameWorld
{
public:
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	StudentWorld(std::string assetDir);
	~StudentWorld();

	virtual bool isCloseTo(int x, int y, Actor* obj);
	virtual bool isCollidingWith(int x, int y, Actor* obj);

	bool isWithinRadius(int x1, int y1, int x2, int y2, double r);
	bool isCloseToAnyActor(int x, int y);
	bool canMoveHere(int x, int y);
	bool canStepHere(int x, int y, GraphObject::Direction dir);
	bool isThereDirtHere(int x, int y);
	bool isThereDirtInThisBox(int x, int y);
	bool isThereBoulderInThisBox(int x, int y);
	bool clearOfDirtBoulder(int x, int y);
	bool isThereBoulderUnderMe(int x, int y);
	bool isCollidingWithBoulder(int x, int y);
	bool isWithinShoutingDistance(int x, int y);
	bool isFacingFrackMan(int x, int y, GraphObject::Direction dir);
	bool isInLineOfSight(int x, int y);

	int annoyAProtester(int x, int y, char cause);
	int min(int a, int b);
	int max(int a, int b);
	int randInt(int l, int h);

	void dropNugget(int x, int y);
	void reduceBarrels();
	void insertSquirt(int x, int y, GraphObject::Direction dir);
	void decrementProtesterCount();
	void annoyFrackMan(char cause);
	void destroyDirt(int x, int y);
	void revealCloseObjects(int x, int y);
	
	GraphObject::Direction faceTheFrack(int x, int y);
	GraphObject::Direction getRandDir();
	GraphObject::Direction canTurn(int x, int y, GraphObject::Direction dir); // returns none if can't turn
	GraphObject::Direction protesterGiveUp(int &x, int &y);
	GraphObject::Direction getCloseToFrack(int &x, int &y);

	FrackMan* getFrack();
	string formatDisplayText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft);

private:
	void addBoulders();
	void addNuggets();
	void addBarrels();
	void addSonarKit();
	void addWaterPool();
	void addSonarOrWater();
	void addProtester();
	void removeDeadObjects();
	void setDisplayText();
	void solveGrid(int x, int y, GraphObject::Direction m[64][64], int r);

	Dirt* m_dirt[64][64];
	vector<Actor*> m_actors;
	FrackMan* fracker;
	GraphObject::Direction grid[64][64];
	GraphObject::Direction frackGrid[64][64];

	int numBarrels;
	int protesterTick;
	int numProtesters;
	int maxNumProtesters;
	int hardCoreMaxVal;
	
	struct Coord
	{
	public:
		Coord(int row, int col, int datAzz) : m_r(row), m_c(col), m_Azz(datAzz) {}
		int r() const { return m_r; }
		int c() const { return m_c; }
		int z() const { return m_Azz; }
	private:
		int m_r;
		int m_c;
		int m_Azz;
	};
};

#endif // STUDENTWORLD_H_
