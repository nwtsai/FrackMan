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
	virtual void getAnnoyed(char cause);
	virtual bool doesThisBlock();
	virtual int isProtester();
	bool isStillAlive();
	void setDead();

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
	void reduceHP(int num);
	int getHP() const;
	StudentWorld* getWorld() const;
	
private:
	StudentWorld* m_World;
	int m_hp;
};

// OBJECT DECLARATION //

class Object : public Actor
{
public:
	Object(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world, FrackMan* fracker);
	virtual ~Object();
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
	virtual void getAnnoyed(char cause);
	int getSquirts() const;
	int getsCharges() const;
	int getGold() const;
	void addSquirts();
	void addCharges();
	void addGold();

private:
	int m_squirts;
	int m_sCharges;
	int m_gold;
};

// BOULDER DECLARATION //

class Boulder : public Object
{
public:
	Boulder(int x, int y, StudentWorld* world, FrackMan* fracker);
	~Boulder();
	virtual void doSomething();
	virtual bool doesThisBlock();

private:
	bool isAnyDirtUnderBoulder();

	int m_state; // 0 stable | 1 waiting | -1 falling
	int m_tickLife;
};

// SQUIRT DECLARATION //

class Squirt : public Object
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

class Barrel : public Object
{
public:
	Barrel(int x, int y, StudentWorld* world, FrackMan* fracker);
	~Barrel();
	virtual void doSomething();
};

// GOLD NUGGET DECLARATION //

class GoldNugget : public Object
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

class SonarKit : public Object
{
public:
	SonarKit(int x, int y, StudentWorld* world, FrackMan* fracker);
	~SonarKit();
	virtual void doSomething();

private:
	int m_tickLife;
};

// WATER POOL DECLARATION // 

class WaterPool : public Object
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
	virtual void getAnnoyed(char cause);
	virtual void doSomething();
	virtual int isProtester();
	void setLeaveField(bool shouldILeave);
	bool getLeaveField() const;
	void setTickCounter(int N); // overload, if it needs to be set it will take in an int
	void setTickCounter();	   // otherwise, it will just set it to the default counter
	bool normalMove1();
	void normalMove2();

private:
	void updateTurnCounter();
	void shout();
	void takeStep(Direction dir);
	void setNumSquaresToMove();
	void setNumSquaresToMove(int N);
	void setTurnCounter(int counter);
	void setHasShouted(bool shoutedyet);
	void setCanTurn(bool canITurn);
	bool getCanTurn() const;
	bool getHasShouted() const;
	int getTurnCounter() const;
	int getNumSquaresToMove() const;
	int getTickCounter() const;
	Direction getViableDirection();

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
	virtual void doSomething();
	virtual void getAnnoyed(char cause);
	virtual int isProtester();

private:
	int tickCounter;
};

#endif // ACTOR_H_
