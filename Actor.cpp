#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATION // 

Actor::Actor(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world)
	: GraphObject(id, x, y, dir, size, depth), m_alive(true), m_World(world)
{
	setVisible(true);
}

Actor::~Actor()
{
	setVisible(false);
}

bool Actor::isStillAlive()
{
	return m_alive;
}

void Actor::setDead()
{
	m_alive = false;
}

StudentWorld* Actor::getWorld() const
{
	return m_World;
}

// DIRT IMPLEMENTATION //

Dirt::Dirt(int x, int y, StudentWorld* world)
	: Actor(IID_DIRT, x, y, right, 0.25, 3, world)
{}

Dirt::~Dirt()
{}

void Dirt::doSomething()
{}

// FRACKMAN IMPLEMENTATION //

FrackMan::FrackMan(StudentWorld* world)
	: Actor(IID_PLAYER, 30, 60, right, 1, 0, world), m_hp(10), m_squirts(5), m_sCharges(1), m_gold(0)
{}

FrackMan::~FrackMan()
{}

void FrackMan::doSomething()
{
	if (!isStillAlive())
		return;

	getWorld()->destroyDirt(getX(), getY());

	int dir;
	if (getWorld()->getKey(dir) == true)
	{
		switch (dir)
		{
		case KEY_PRESS_LEFT:
			if (getDirection() != left)
				setDirection(left);
			else if (getX() - 1 >= 0)
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
				setDirection(right);
			else if (getX() + 1 <= 60)
			moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
				setDirection(up);
			else if (getY() + 1 <= 60)
			moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
				setDirection(down);
			else if (getY() - 1 >= 0)
			moveTo(getX(), getY() - 1);
			break;
		}
	}
}

int FrackMan::getHP() const
{
	return m_hp;
}

int FrackMan::getSquirts() const
{
	return m_squirts;
}

int FrackMan::getsCharges() const
{
	return m_sCharges;
}

int FrackMan::getGold() const
{
	return m_gold;
}

