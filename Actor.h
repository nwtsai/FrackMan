#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

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

//////////////////////////////////////////////////////////////////

class Dirt : public Actor
{
public:
	Dirt(int x, int y);
	virtual ~Dirt();
	virtual void doSomething();
};

class FrackMan : public Actor
{
public:
	FrackMan(StudentWorld* world);
	~FrackMan();
	virtual void doSomething();
	StudentWorld* getWorld() const;
private:
	int m_hp;
	int m_water;
	bool m_sCharge;
	int m_gold;
	StudentWorld* m_World;
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
