#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameController.h"

class StudentWorld;
class FrackMan;

// ACTOR DECLARATION //

class Actor : public GraphObject
{
public:
	Actor(int id, int x, int y, Direction dir, double size, unsigned int depth);
	virtual ~Actor();
	virtual void doSomething() = 0;
	virtual bool isStillAlive();
	virtual void setDead();
	virtual bool doesThisBlock();
	virtual int isProtester();
	virtual void getAnnoyed(char cause);
private:
	bool m_alive;
};

// DIRT DECLARATION //

class Dirt : public Actor
{
public:
	Dirt(int x, int y);
	virtual ~Dirt();
	virtual void doSomething();
};

// LIVING ACTOR DECLARATION //

class LivingActor : public Actor
{
public:
	LivingActor(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world, int hp);
	virtual ~LivingActor();
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	int getHP() const;
	void reduceHP(int num);
private:
	StudentWorld* m_World;
	int m_hp;
};

// STATIONARY OBJECT DECLARATION //

class StationaryObject : public Actor
{
public:
	StationaryObject(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world, FrackMan* fracker);
	virtual ~StationaryObject();
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	FrackMan* getFracker() const;
private:
	StudentWorld* m_World;
	FrackMan* m_Fracker;
};

// FRACKMAN DECLARATION // 

class FrackMan : public LivingActor
{
public:
	FrackMan(StudentWorld* world);
	~FrackMan();
	virtual void doSomething();
	int getSquirts() const;
	int getsCharges() const;
	int getGold() const;
	virtual void getAnnoyed(char cause);
	
	void addSquirts();
	void addCharges();
	void addGold();
private:
	int m_squirts;
	int m_sCharges;
	int m_gold;
};

// BOULDER DECLARATION //

class Boulder : public StationaryObject
{
public:
	Boulder(int x, int y, StudentWorld* world, FrackMan* fracker);
	~Boulder();
	virtual void doSomething();
	bool isAnyDirtUnderBoulder();
	int getBoulderX();
	int getBoulderY();
	virtual bool doesThisBlock();
private:
	int m_state; // 0 stable | 1 waiting | -1 falling
	int m_tickLife;
	int m_x;
	int m_y;
};

// SQUIRT DECLARATION //

class Squirt : public StationaryObject
{
public:
	Squirt(int x, int y, Direction dir, StudentWorld* world, FrackMan* fracker);
	~Squirt();
	virtual void doSomething();
private:
	int m_distanceTrav;
	bool isFirstTick;
};

// BARREL DECLARATION //

class Barrel : public StationaryObject
{
public:
	Barrel(int x, int y, StudentWorld* world, FrackMan* fracker);
	~Barrel();
	virtual void doSomething();
private:

};

// GOLD NUGGET DECLARATION //

class GoldNugget : public StationaryObject
{
public:
	GoldNugget(int x, int y, bool isPerm, StudentWorld* world, FrackMan* fracker);
	~GoldNugget();
	virtual void doSomething();
private:
	bool canFrackManGet;
	bool isPermanentState;
	int m_tickLife;
};

// SONAR KIT DECLARATION //

class SonarKit : public StationaryObject
{
public:
	SonarKit(int x, int y, StudentWorld* world, FrackMan* fracker);
	~SonarKit();
	virtual void doSomething();
private:
	int m_tickLife;
};

// WATER POOL DECLARATION // 

class WaterPool : public StationaryObject
{
public:
	WaterPool(int x, int y, StudentWorld* world, FrackMan* fracker);
	~WaterPool();
	virtual void doSomething();
private:
	int m_tickLife;
};

// PROTESTER DECLARATION //

class Protester : public LivingActor
{
public:
	Protester(int ID, int HP, StudentWorld* world);
	~Protester();
	void takeStep(Direction dir);
	virtual void getAnnoyed(char cause);
	virtual void doSomething();
	virtual int isProtester();

	void setLeaveField(bool shouldILeave);
	bool getLeaveField() const;
	void setNumSquaresToMove();
	void setNumSquaresToMove(int N);
	int getNumSquaresToMove() const;
	void setTickCounter(int N); // overload, if it needs to be set it will take in an int
	void setTickCounter();	   // otherwise, it will just set it to the default counter
	int getTickCounter() const;
	void setHasShouted(bool shoutedyet);
	bool getHasShouted() const;
	void setCanTurn(bool canITurn);
	bool getCanTurn() const;
	void setTurnCounter(int counter);
	int getTurnCounter() const;

	void updateTurnCounter();
	void shout();
	Direction getViableDirection();
private:
	bool leaveField;
	int numSquaresToMove;
	int tickCounter;
	bool hasShouted;
	bool canTurn;
	int turnCounter;
};

// HARDCORE PROTESTER DECLARATION // 

class HardcoreProtester : public Protester
{
public:
	HardcoreProtester(int ID, int HP, StudentWorld* world);
	~HardcoreProtester();
	virtual int isProtester();
	virtual void getAnnoyed(char cause);
	virtual void doSomething();
private:
	int tickCounter;
};
#endif // ACTOR_H_
