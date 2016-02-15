#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world);
	virtual ~Actor();
	virtual void doSomething() = 0;
	virtual bool isStillAlive();
	virtual void setDead();
	StudentWorld* getWorld() const;
private:
	bool m_alive;
	StudentWorld* m_World;
};

//////////////////////////////////////////////////////////////////

class Dirt : public Actor
{
public:
	Dirt(int x, int y, StudentWorld* world);
	virtual ~Dirt();
	virtual void doSomething();
};

//////////////////////////////////////////////////////////////////

class FrackMan : public Actor
{
public:
	FrackMan(StudentWorld* world);
	~FrackMan();
	virtual void doSomething();
	int getHP() const;
	int getSquirts() const;
	int getsCharges() const;
	int getGold() const;
private:
	int m_hp;
	int m_squirts;
	int m_sCharges;
	int m_gold;
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
