#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

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

// MOVEABLE OBJECTS DECLARATION //

class MoveableObjects : public Actor
{
public:
	MoveableObjects(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world);
	virtual ~MoveableObjects();
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
private:
	StudentWorld* m_World;
};

// STATIONARY OBJECTS DECLARATION //

class StationaryObjects : public Actor
{
public:
	StationaryObjects(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world);
	virtual ~StationaryObjects();
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
private:
	StudentWorld* m_World;
};

// FRACKMAN DECLARATION // 

class FrackMan : public MoveableObjects
{
public:
	FrackMan(StudentWorld* world);
	~FrackMan();
	virtual void doSomething();
	int getHP() const;
	int getSquirts() const;
	int getsCharges() const;
	int getGold() const;
	void setHP(int hp);
	void setSquirts(int squirts);
	void setsCharges(int charges);
	void setGold(int gold);
private:
	int m_hp;
	int m_squirts;
	int m_sCharges;
	int m_gold;
};

// BOULDER DECLARATION //

class Boulder : public MoveableObjects
{
public:
	Boulder(int x, int y, StudentWorld* world);
	~Boulder();
	virtual void doSomething();
	bool isAnyDirtUnderBoulder();
private:
	int m_state; // 0 stable | 1 waiting | -1 falling
	int m_counter;
};

// SQUIRT DECLARATION //

class Squirt : public StationaryObjects
{
public:
	Squirt(int x, int y, Direction dir, StudentWorld* world);
	~Squirt();
	virtual void doSomething();
private:
	int m_distanceTrav;
};

// BARRELS DECLARATION //

class Barrels : public StationaryObjects
{
public:
	Barrels(int x, int y, StudentWorld* world);
	~Barrels();
	virtual void doSomething();
private:

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
