#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATION // 

Actor::Actor(int id, int x, int y, Direction dir, double size, unsigned int depth)
	: GraphObject(id, x, y, dir, size, depth), m_alive(true)
{
	// setVisible(true);
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

bool Actor::doesThisBlock()
{
	return false;
}

bool Actor::isProtester()
{
	return false;
}

// only does something in FrackMan, Protester, and HardCore Protesters. Need to declare in Actor class so that StudentWorld can access this virtual method
void Actor::getAnnoyed(bool isCompletelyAnnoyed)
{}

// DIRT IMPLEMENTATION //

Dirt::Dirt(int x, int y)
	: Actor(IID_DIRT, x, y, right, 0.25, 3)
{
	setVisible(true);
}

Dirt::~Dirt()
{}

void Dirt::doSomething()
{}

// LIVING ACTOR IMPLEMENTATION //

LivingActor::LivingActor(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world, int hp)
	: Actor(id, x, y, dir, size, depth), m_World(world), m_hp(hp)
{}

LivingActor::~LivingActor()
{}

StudentWorld* LivingActor::getWorld() const
{
	return m_World;
}

int LivingActor::getHP() const
{
	return m_hp;
}

void LivingActor::reduceHP(int num)
{
	m_hp = m_hp - num;
}
// STATIONARY OBJECTS IMPLEMENTATION //

StationaryObject::StationaryObject(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world, FrackMan* fracker)
	: Actor(id, x, y, dir, size, depth), m_World(world), m_Fracker(fracker)
{}

StationaryObject::~StationaryObject()
{}

StudentWorld* StationaryObject::getWorld() const
{
	return m_World;
}

FrackMan* StationaryObject::getFracker() const
{
	return m_Fracker;
}

// FRACKMAN IMPLEMENTATION //

FrackMan::FrackMan(StudentWorld* world)
	: LivingActor(IID_PLAYER, 30, 60, right, 1.0, 0, world, 10), m_squirts(5), m_sCharges(1), m_gold(0)
{
	setVisible(true);
}

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
				GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
				getWorld()->insertSquirt(getX(), getY(), getDirection());
				//m_squirts--;
			}
			break;
		case 'z':
		case 'Z':
			if (m_sCharges > 0)
			{
				getWorld()->revealCloseObjects(getX(), getY());
				GameController::getInstance().playSound(SOUND_SONAR);
				m_sCharges--;
			}
			break;
		case KEY_PRESS_TAB:
			if (m_gold > 0)
			{
				getWorld()->dropNugget(getX(), getY());
				m_gold--;
			}
			break;
		case KEY_PRESS_LEFT:
			if (getDirection() != left)
				setDirection(left);
			else if (getWorld()->canMoveHere(getX() - 1, getY()) == false)
				break;
			else if (getX() - 1 >= 0)
				moveTo(getX() - 1, getY());
			else
				moveTo(getX(), getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
				setDirection(right);
			else if (getWorld()->canMoveHere(getX() + 1, getY()) == false)
				break;
			else if (getX() + 1 <= 60)
				moveTo(getX() + 1, getY());
			else
				moveTo(getX(), getY());
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
				setDirection(up);
			else if (getWorld()->canMoveHere(getX(), getY() + 1) == false)
				break;
			else if (getY() + 1 <= 60)
				moveTo(getX(), getY() + 1);
			else
				moveTo(getX(), getY());
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
				setDirection(down);
			else if (getWorld()->canMoveHere(getX(), getY() - 1) == false)
				break;
			else if (getY() - 1 >= 0)
				moveTo(getX(), getY() - 1);
			else
				moveTo(getX(), getY());
			break;
		}
	}
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

void FrackMan::addSquirts() 
{
	m_squirts += 5;
}

void FrackMan::addCharges() 
{
	m_sCharges++;
}

void FrackMan::addGold() 
{
	m_gold++;
}

void FrackMan::getAnnoyed(bool isCompletelyAnnoyed)
{
	if (isCompletelyAnnoyed)
	{
		setDead();
		return;
	}

	reduceHP(2);
	if (getHP() <= 0)
	{
		setDead();
		GameController::getInstance().playSound(SOUND_PLAYER_GIVE_UP);
	}
}

// BOULDER IMPLEMENTATION //

Boulder::Boulder(int x, int y, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_BOULDER, x, y, down, 1.0, 1, world, fracker), m_state(0), m_tickLife(30), m_x(getX()), m_y(getY())
{
	setVisible(true);
	getWorld()->destroyDirt(x, y);
}

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
		if (m_tickLife <= 0)
		{
			m_state = -1;
			GameController::getInstance().playSound(SOUND_FALLING_ROCK);
		}
		else
			m_tickLife--;
	}
	// if Boulder is in a falling state
	else 
	{
		if (getY() > 0 && !isAnyDirtUnderBoulder() && !getWorld()->isThereBoulderUnderMe(getX(), getY()))
		{
			moveTo(getX(), getY() - 1);

			if (getWorld()->isCollidingWithProtester(getX(), getY()))
			{
				getWorld()->annoyProtester(getX(), getY(), true);
				getWorld()->increaseScore(500);
			}

			if (getWorld()->isCollidingWith(getX(), getY(), getWorld()->getFrack()))
			{
				getWorld()->annoyFrackMan(true);
				setDead();
			}
		}
		else
			setDead();
	}
}

int Boulder::getBoulderX()
{
	return m_x;
}

int Boulder::getBoulderY()
{
	return m_y;
}

bool Boulder::doesThisBlock()
{
	return true;
}

// SQUIRT IMPLEMENTATION //

Squirt::Squirt(int x, int y, Direction dir, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_WATER_SPURT, x, y, dir, 1.0, 1, world, fracker), m_distanceTrav(0), isFirstTick(true)
{
	setVisible(true);
}

Squirt::~Squirt()
{}

void Squirt::doSomething()
{
	if (isFirstTick)
	{
		isFirstTick = false;
		return;
	}

	if (!isStillAlive())
	{
		return;
	}
	else if (getWorld()->isCollidingWithProtester(getX(), getY()))
	{
		getWorld()->annoyProtester(getX(), getY(), false);
		setDead();
		return;
	}
	else if (m_distanceTrav < 4)
	{
		if (getDirection() == right && getX() + 1 <= 60 && !getWorld()->isThereDirtInThisBox(getX() + 1, getY()) && !getWorld()->isThereBoulderInThisBox(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			m_distanceTrav++;
		}
		else if (getDirection() == left && getX() - 1 >= 0 && !getWorld()->isThereDirtInThisBox(getX() - 1, getY()) && !getWorld()->isThereBoulderInThisBox(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			m_distanceTrav++;
		}
		else if (getDirection() == up &&  getY() + 1 <= 60 && !getWorld()->isThereDirtInThisBox(getX(), getY() + 1) && !getWorld()->isThereBoulderInThisBox(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			m_distanceTrav++;
		}
		else if (getDirection() == down && getY() - 1 >= 0 && !getWorld()->isThereDirtInThisBox(getX(), getY() - 1) && !getWorld()->isThereBoulderInThisBox(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			m_distanceTrav++;
		}
		else
		{
			setDead();
		}
	}
	else if (m_distanceTrav >= 4)
	{
		setDead();
	}
}

// BARREL IMPLEMENTATION //

Barrel::Barrel(int x, int y, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_BARREL, x, y, right, 1.0, 2, world, fracker) 
{
	setVisible(true); // for testing only, delete after
}

Barrel::~Barrel()
{}

void Barrel::doSomething()
{
	if (!isStillAlive())
		return;

	if (isVisible() == false && getWorld()->isWithinRadius(getX(), getY(), getFracker()->getX(), getFracker()->getY(), 4))
	{
		setVisible(true);
		return;
	}
	else if (getWorld()->isCollidingWith(getX(), getY(), getFracker()))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->reduceBarrels();
	}
}

// GOLD NUGGET IMPLEMENTATION //

GoldNugget::GoldNugget(int x, int y, bool isPerm, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_GOLD, x, y, right, 1.0, 2, world, fracker), isPermanentState(isPerm), m_tickLife(300)
{
	if (isPermanentState)
	{
		setVisible(true); // for testing only, change to false later
		canFrackManGet = true;
	}
	else
	{
		setVisible(true);
		canFrackManGet = false;
	}
}

GoldNugget::~GoldNugget()
{}

void GoldNugget::doSomething()
{
	if (!isStillAlive())
		return;

	if (!isVisible() && getWorld()->isWithinRadius(getX(), getY(), getFracker()->getX(), getFracker()->getY(), 4.0))
	{
		setVisible(true);
		return;
	}
	else if (canFrackManGet == true && getWorld()->isCollidingWith(getX(), getY(), getFracker()))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getFracker()->addGold();
	}
	else if (canFrackManGet == false && getWorld()->isCollidingWithProtester(getX(), getY())) 
	{
		setDead();
		GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
		getWorld()->annoyProtester(getX(), getY(), true); // how to only annoy one protester?
		getWorld()->increaseScore(25);
	}

	if (!isPermanentState)
	{
		if (m_tickLife == 0)
		{
			setDead();
			return;
		}
		m_tickLife--;
	}
}

// SONAR KIT IMPLEMENTATION //

SonarKit::SonarKit(int x, int y, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_SONAR, x, y, right, 1.0, 2, world, fracker)
{
	m_tickLife = getWorld()->max(100, 300 - 10 * getWorld()->getLevel());
	setVisible(true);
}

SonarKit::~SonarKit()
{}

void SonarKit::doSomething()
{
	if (!isStillAlive())
		return;

	if (getWorld()->isCollidingWith(getX(), getY(), getFracker()))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getFracker()->addCharges();
		getWorld()->increaseScore(75);
	}

	if (m_tickLife == 0)
	{
		setDead();
		return;
	}
	m_tickLife--;
}

// WATER POOL IMPLEMENTATION //

WaterPool::WaterPool(int x, int y, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_WATER_POOL, x, y, right, 1.0, 2, world, fracker)
{
	m_tickLife = getWorld()->max(100, 300 - 10 * getWorld()->getLevel());
	setVisible(true);
}

WaterPool::~WaterPool()
{}

void WaterPool::doSomething()
{
	if (!isStillAlive())
		return;

	if (getWorld()->isWithinRadius(getX(), getY(), getFracker()->getX(), getFracker()->getY(), 3.0))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getFracker()->addSquirts();
		getWorld()->increaseScore(100); 
	}

	if (m_tickLife == 0)
	{
		setDead();
		return;
	}
	m_tickLife--;
}

// PROTESTER IMPLEMENTATION //

Protester::Protester(int ID, int hp, StudentWorld* world)
	: LivingActor(ID, 60, 60, left, 1.0, 0, world, hp), leaveField(false), hasShouted(false), canTurn(true), turnCounter(0)
{
	numSquaresToMove = getWorld()->randInt(8, 60);
	tickCounter = getWorld()->max(0, 3 - getWorld()->getLevel() / 4);
	setVisible(true);
}

Protester::~Protester()
{}

void Protester::takeStep(Direction dir)
{
	if (dir == right)
		moveTo(getX() + 1, getY());
	else if (dir == left)
		moveTo(getX() - 1, getY());
	else if (dir == up)
		moveTo(getX(), getY() + 1);
	else // dir == down
		moveTo(getX(), getY() - 1);
}

void Protester::getAnnoyed(bool isCompletelyAnnoyed)
{
	// if Protester is Completely Annoyed, immediately set leavefield to true
	if (isCompletelyAnnoyed)
		leaveField = true;

	// if Protester is already in leave the field state, can't be further annoyed
	if (leaveField == true)
	{
		return;
	}

	// Protesters can only be annoyed by boulders, gold, and squirts. Since boulders and gold both cause function to return right away
	// all code below this comment is a result of being squirted
	
	reduceHP(2);

	if (getHP() > 0)
	{
		GameController::getInstance().playSound(SOUND_PROTESTER_ANNOYED);
		tickCounter = getWorld()->max(50, 100 - getWorld()->getLevel() * 10);
	}
	else 
	{
		leaveField = true;
		GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
		tickCounter = 0;
		getWorld()->increaseScore(100);
	}
}

void Protester::doSomething()
{
	// if it is dead, return
	if (!isStillAlive())
		return;

	// if Protester isn't in rest state, update turn counter (what about when it is immobilized? ask professor)
	if (tickCounter <= 0)
	{
		if (turnCounter > 0)
		{
			turnCounter--;
		}
		else if (canTurn == false)
		{
			canTurn = true;
		}
	}

	// hasShouted is only true after the Protester gets immobilized. After 15 non resting ticks have passed, reset the shout counter 
	if (hasShouted == true)
		hasShouted = false;

	// if Protesters is in rest state
	if (tickCounter > 0)
	{
		tickCounter--;
		return;
	}
	else if (leaveField == true)
	{
		if (getX() == 60 && getY() == 60)
			setDead();
		// call a function that finds the next best step for it to take towards the exit
		// call moveTo function
		tickCounter = gettickCounter();
		return;
	}
	else if (getWorld()->isWithinShoutingDistance(getX(), getY()) && getWorld()->isFacingFrackMan(getX(), getY(), getDirection()) && hasShouted == false)
	{
		GameController::getInstance().playSound(SOUND_PROTESTER_YELL);
		getWorld()->annoyFrackMan(false);
		hasShouted = true;
		tickCounter = 15 * gettickCounter(); // immobilize protester for 15 non resting ticks
		return;
	}
	else if (getWorld()->isInLineOfSight(getX(), getY()))
	{
		if (!getWorld()->isWithinShoutingDistance(getX(), getY()))
		{
			setDirection(getWorld()->faceTheFrack(getX(), getY()));
			takeStep(getDirection());
		}
		else
		{
			setDirection(getWorld()->faceTheFrack(getX(), getY()));
		}
		numSquaresToMove = 0;
		tickCounter = gettickCounter();
		return;
	}
	else
	{
		numSquaresToMove--;
		if (numSquaresToMove <= 0)
		{
			Direction dir;
			dir = getWorld()->getRandDir();

			while (!getWorld()->canStepHere(getX(), getY(), dir))
			{
				dir = getWorld()->getRandDir();
			}
			setDirection(dir);
			numSquaresToMove = getWorld()->randInt(8, 60);
		}
		else
		{
			Direction dir = getWorld()->canTurn(getX(), getY(), getDirection());
			if (dir != none)
			{
				if (canTurn == true)
				{
					setDirection(dir);
					numSquaresToMove = getWorld()->randInt(8, 60);
					turnCounter = 200;
					canTurn = false;
				}
			}
		}

		// continue with step 8
		if (getWorld()->canStepHere(getX(), getY(), getDirection()))
			takeStep(getDirection());
		else
			numSquaresToMove = 0;
		
		tickCounter = gettickCounter();
		return;
	}	
}

bool Protester::isProtester()
{
	return true;
}

int Protester::gettickCounter() const
{
	return getWorld()->max(0, 3 - getWorld()->getLevel() / 4);
}

// HARDCORE PROTESTER IMPLEMENTATION //

HardcoreProtester::HardcoreProtester(int ID, int HP, StudentWorld* world)
	: Protester(IID_HARD_CORE_PROTESTER, 20, world)
{
	tickCounter = gettickCounter();
}

HardcoreProtester::~HardcoreProtester()
{}

void HardcoreProtester::getAnnoyed(bool isCompletelyAnnoyed)
{

}

void HardcoreProtester::doSomething()
{
	// make methods out of protesters doSomething, and call them individually here, and add the one new change within this body of code
	if (!isStillAlive())
		return;

	if (tickCounter > 0)
	{
		tickCounter--;
		return;
	}
}