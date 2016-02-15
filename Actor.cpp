#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATION // 

Actor::Actor(int id, int x, int y, Direction dir, double size, unsigned int depth)
	: GraphObject(id, x, y, dir, size, depth), m_alive(true)
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
	setVisible(false);
}

// DIRT IMPLEMENTATION //

Dirt::Dirt(int x, int y)
	: Actor(IID_DIRT, x, y, right, 0.25, 3)
{}

Dirt::~Dirt()
{}

void Dirt::doSomething()
{}

// MOVEABLE OBJECTS IMPLEMENTATION

MoveableObjects::MoveableObjects(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world)
	: Actor(id, x, y, dir, size, depth), m_World(world)
{}

MoveableObjects::~MoveableObjects()
{}

StudentWorld* MoveableObjects::getWorld() const
{
	return m_World;
}

// STATIONARY OBJECTS IMPLEMENTATION //

StationaryObjects::StationaryObjects(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world)
	: Actor(id, x, y, dir, size, depth), m_World(world)
{}

StationaryObjects::~StationaryObjects()
{}

StudentWorld* StationaryObjects::getWorld() const
{
	return m_World;
}

// FRACKMAN IMPLEMENTATION //

FrackMan::FrackMan(StudentWorld* world)
	: MoveableObjects(IID_PLAYER, 30, 60, right, 1, 0, world), m_hp(10), m_squirts(5), m_sCharges(1), m_gold(0)
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
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
		case KEY_PRESS_SPACE:
			if (m_squirts > 0)
			{
				// getWorld()->insertSquirt(getX(), getY(), dir);
				m_squirts--;
			}
			break;
		case 'z':
		case 'Z':
			if (m_sCharges > 0)
			{
				// getWorld()->insertsCharge(getX(), getY());
				m_sCharges--;
			}
			break;
		case KEY_PRESS_TAB:
			if (m_gold > 0)
			{
				// getWorld()->insertGold(getX(), getY());
				m_gold--;
			}
			break;
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

void FrackMan::setHP(int hp) 
{
	m_hp = hp;
}

void FrackMan::setSquirts(int squirts) 
{
	m_squirts = squirts;
}

void FrackMan::setsCharges(int charges) 
{
	m_sCharges = charges;
}

void FrackMan::setGold(int gold) 
{
	m_gold = gold;
}

// BOULDER IMPLEMENTATION //

Boulder::Boulder(int x, int y, StudentWorld* world)
	: MoveableObjects(IID_BOULDER, x, y, down, 1.0, 1, world), m_state(0), m_counter(30)
{}

Boulder::~Boulder()
{}

bool Boulder::isAnyDirtUnderBoulder()
{
	if (getWorld()->isThereDirtHere(getX(), getY() - 1) || getWorld()->isThereDirtHere(getX() + 1, getY() - 1) ||
		getWorld()->isThereDirtHere(getX() + 2, getY() - 1) || getWorld()->isThereDirtHere(getX() + 3, getY() - 1))
	{
		return true;
	}
	return false;
}

void Boulder::doSomething()
{
	if (!isStillAlive())
		return;

	// if Boulder is in a stable state
	if (m_state == 0)
	{
		if (!isAnyDirtUnderBoulder())
		{
			m_state = 1;
		}
	}
	// if Boulder is in a waiting state
	else if (m_state == 1)
	{
		if (m_counter == 0)
		{
			m_state = -1;
			// GameController::getInstance().playSound(SOUND_FALLING_ROCK);
		}
		m_counter--;
	}
	// if Boulder is in a falling state
	else 
	{
		if (getY() > 0 && !isAnyDirtUnderBoulder())
			moveTo(getX(), getY() - 1);
		else
			setDead();
	}

	return;
}

// SQUIRT IMPLEMENTATION //

Squirt::Squirt(int x, int y, Direction dir, StudentWorld* world)
	: StationaryObjects(IID_WATER_SPURT, x, y, dir, 1.0, 1, world), m_distanceTrav(0)
{}

Squirt::~Squirt()
{}

void Squirt::doSomething()
{
	/*
	If a Squirt is within a radius of 3.0 of one or more Protesters (up to and including
	a distance of 3.0 squares away), it will cause 2 points of annoyance to these
	Protester(s), and then immediately set its state to dead, so it can be removed from
	the oil field at the end of the tick
	*/

	if (isStillAlive() && m_distanceTrav <= 4)
	{
		m_distanceTrav++;
	}
	else
	{
		setDead();
	}
}

// BARRELS IMPLEMENTATION //

Barrels::Barrels(int x, int y, StudentWorld* world)
	: StationaryObjects(IID_BARREL, x, y, right, 1.0, 2, world) 
{}

Barrels::~Barrels()
{}

void Barrels::doSomething()
{
	if (!isStillAlive())
		return;


}