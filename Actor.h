#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameController.h"

class StudentWorld;

// ACTOR DECLARATION //

class Actor : public GraphObject
{
public:
	Actor(int id, int x, int y, Direction dir, double size, unsigned int depth);
	virtual ~Actor();
	virtual void doSomething() = 0;
	virtual bool isStillAlive();
	virtual void setDead();
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

// MOVEABLE OBJECT DECLARATION //

class MoveableObject : public Actor
{
public:
	MoveableObject(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world);
	virtual ~MoveableObject();
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
private:
	StudentWorld* m_World;
};

// STATIONARY OBJECT DECLARATION //

class FrackMan;

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

class FrackMan : public MoveableObject
{
public:
	FrackMan(StudentWorld* world);
	~FrackMan();
	virtual void doSomething();
	int getHP() const;
	int getSquirts() const;
	int getsCharges() const;
	int getGold() const;
	void addHP();
	void addSquirts();
	void addCharges();
	void addGold();
private:
	int m_hp;
	int m_squirts;
	int m_sCharges;
	int m_gold;
};

// BOULDER DECLARATION //

class Boulder : public MoveableObject
{
public:
	Boulder(int x, int y, StudentWorld* world);
	~Boulder();
	virtual void doSomething();
	bool isAnyDirtUnderBoulder();
private:
	int m_state; // 0 stable | 1 waiting | -1 falling
	int m_tickLife;
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
};

// Barrel DECLARATION //

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
	GoldNugget(int x, int y, StudentWorld* world, FrackMan* fracker);
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

//////////////////////////////////////////////////////////////////

/*class Protester : public GameObject
{
public:
	Protester(StudentWorld* world, int x, int y);
	~Protester();
	virtual void doSomething();
	int getHealth();
	void lowerHealth();
private:
	int m_health;
};

//////////////////////////////////////////////////////////////////

class HardcoreProtester : public Protester
{
public:
	HardcoreProtester(StudentWorld* world, int x, int y);
	~HardcoreProtester();
	virtual void doSomething();
	int getHealth();
	void lowerHealth();
private:
	int m_health;
};*/
#endif // ACTOR_H_
