#include "GraphObject.h"
#include "GameController.h"
#include "StudentWorld.h"
#include "Actor.h"

// ACTOR IMPLEMENTATION // 

Actor::Actor(int id, int x, int y, Direction dir, double size, unsigned int depth)
	: GraphObject(id, x, y, dir, size, depth), m_alive(true)
{}

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

int Actor::isProtester() // returns 0 if not protester, 1 if regular protester, 2 if hardcore protester
{
	return 0;
}

// only does something in FrackMan, Protester, and HardCore Protesters. Need to declare in Actor class so that StudentWorld can access this virtual method
void Actor::getAnnoyed(char cause)
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
// OBJECT IMPLEMENTATION //

Object::Object(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world, FrackMan* fracker)
	: Actor(id, x, y, dir, size, depth), m_World(world), m_Fracker(fracker)
{}

Object::~Object()
{}

StudentWorld* Object::getWorld() const
{
	return m_World;
}

FrackMan* Object::getFracker() const
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
				m_squirts--;
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

void FrackMan::getAnnoyed(char cause)
{
	if (cause == 'B')
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
	: Object(IID_BOULDER, x, y, down, 1.0, 1, world, fracker), m_state(0), m_tickLife(30)
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

			if (getWorld()->annoyAProtester(getX(), getY(), 'B') == 1)
			{
				getWorld()->increaseScore(500);
			}

			if (getWorld()->annoyAProtester(getX(), getY(), 'B') == 2)
			{
				getWorld()->increaseScore(500);
			}

			if (getWorld()->isCollidingWith(getX(), getY(), getWorld()->getFrack()))
			{
				getWorld()->annoyFrackMan('B');
				setDead();
			}
		}
		else
			setDead();
	}
}

bool Boulder::doesThisBlock()
{
	return true;
}

// SQUIRT IMPLEMENTATION //

Squirt::Squirt(int x, int y, Direction dir, StudentWorld* world, FrackMan* fracker)
	: Object(IID_WATER_SPURT, x, y, dir, 1.0, 1, world, fracker), m_distanceTrav(0), isFirstTick(true)
{
	setVisible(true);
}

Squirt::~Squirt()
{}

void Squirt::doSomething()
{
	if (!isStillAlive())
	{
		return;
	}

	// do nothing on first tick so that initial image shows up
	if (isFirstTick)
	{
		isFirstTick = false;
		return;
	}

	else if (getWorld()->annoyAProtester(getX(), getY(), 'S') == 1)
	{
		setDead();
		return;
	}
	else if (getWorld()->annoyAProtester(getX(), getY(), 'S') == 2)
	{
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
	: Object(IID_BARREL, x, y, right, 1.0, 2, world, fracker) 
{}

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
	: Object(IID_GOLD, x, y, right, 1.0, 2, world, fracker), isPermanentState(isPerm), m_tickLife(300)
{
	if (isPermanentState)
	{
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
	else if (canFrackManGet == false) 
	{
		// if gold is within grabbing distance of a Regular Protester
		if (getWorld()->annoyAProtester(getX(), getY(), 'G') == 1)
		{
			setDead();
			GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
			getWorld()->increaseScore(25);
		}
		// if gold is within grabbing distance of a Hardcore Protester
		else if (getWorld()->annoyAProtester(getX(), getY(), 'G') == 2)
		{
			setDead();
			GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
			getWorld()->increaseScore(50);
		}
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
	: Object(IID_SONAR, x, y, right, 1.0, 2, world, fracker)
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
	: Object(IID_WATER_POOL, x, y, right, 1.0, 2, world, fracker)
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
	setNumSquaresToMove();
	setTickCounter();
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

void Protester::getAnnoyed(char cause)
{
	// if Protester is Completely Annoyed, immediately set leavefield to true
	if (cause == 'B' || cause == 'G')
		setLeaveField(true);

	// if Protester is already in leave the field state, can't be further annoyed
	if (getLeaveField() == true)
	{
		return;
	}

	// Protesters can only be annoyed by boulders, gold, and squirts. Since boulders and gold both cause function to return right away
	// all code below this comment is a result of being squirted
	
	reduceHP(2);

	if (getHP() > 0)
	{
		GameController::getInstance().playSound(SOUND_PROTESTER_ANNOYED);
		setTickCounter(45);
	}
	else 
	{
		setLeaveField(true);
		GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
		setTickCounter(0);
		getWorld()->increaseScore(100);
	}
}

void Protester::doSomething()
{
	if (!normalMove1())
		normalMove2();
}

int Protester::isProtester()
{
	// if its leaveField member is true, then treat it as if it isn't a Protester, so it can't get annoyed further 
	if (getLeaveField() == false)
		return 1;
	return 0;
}

void Protester::setLeaveField(bool shouldILeave)
{
	leaveField = shouldILeave;
}

bool Protester::getLeaveField() const
{
	return leaveField;
}

void Protester::setNumSquaresToMove()
{
	numSquaresToMove = getWorld()->randInt(8, 60);
}

void Protester::setNumSquaresToMove(int N)
{
	numSquaresToMove = N;
}

int Protester::getNumSquaresToMove() const
{
	return numSquaresToMove;
}

// overload, if it needs to be set it will take in an int
void Protester::setTickCounter(int N)
{
	tickCounter = N;
}

// otherwise, it will just set it to the default counter
void Protester::setTickCounter()
{
	tickCounter = getWorld()->max(0, 3 - getWorld()->getLevel() / 4);
}

int Protester::getTickCounter() const
{
	return tickCounter;
}

void Protester::setHasShouted(bool shoutedyet)
{
	hasShouted = shoutedyet;
}

bool Protester::getHasShouted() const
{
	return hasShouted;
}

void Protester::setCanTurn(bool canITurn)
{
	canTurn = canITurn;
}

bool Protester::getCanTurn() const
{
	return canTurn;
}

void Protester::setTurnCounter(int counter)
{
	turnCounter = counter;
}

int Protester::getTurnCounter() const
{
	return turnCounter;
}

void Protester::updateTurnCounter()
{
	if (getTurnCounter() > 0)
	{
		setTurnCounter(getTurnCounter() - 1);
	}
	else if (getCanTurn() == false)
	{
		setCanTurn(true);
	}
}

void Protester::shout()
{
	GameController::getInstance().playSound(SOUND_PROTESTER_YELL);
	getWorld()->annoyFrackMan('P');
	setHasShouted(true);
}

bool Protester::normalMove1()
{
	// if it is dead, return
	if (!isStillAlive())
		return true;

	// if Protester isn't in rest state, update turn counter (what about when it is immobilized? ask professor)
	if (getTickCounter() <= 0)
	{
		updateTurnCounter();
	}

	// hasShouted is only true after the Protester gets immobilized. After 15 non resting ticks have passed, reset hasShouted
	if (getHasShouted() == true)
		setHasShouted(false);

	// if Protesters is in rest state
	if (getTickCounter() > 0)
	{
		setTickCounter(getTickCounter() - 1);
		return true;
	}
	else if (getLeaveField() == true)
	{
		if (getX() == 60 && getY() == 60)
		{
			setDead();
			getWorld()->decrementProtesterCount();
		}

		Direction dir = none;
		int tempX = getX();
		int tempY = getY();
		dir = getWorld()->protesterGiveUp(tempX, tempY);
		if (dir != none)
		{
			setDirection(dir);
			moveTo(tempX, tempY);
		}

		setTickCounter();
		return true;
	}
	else if (getWorld()->isWithinShoutingDistance(getX(), getY()) && getWorld()->isFacingFrackMan(getX(), getY(), getDirection()) && getHasShouted() == false)
	{
		shout();
		setTickCounter();
		setTickCounter(15 * getTickCounter());
		return true;
	}
	else
	{
		return false;
	}
}

void Protester::normalMove2()
{
	if (getWorld()->isInLineOfSight(getX(), getY()))
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
		setNumSquaresToMove(0);
		setTickCounter();
		return;
	}
	else
	{
		setNumSquaresToMove(getNumSquaresToMove() - 1);
		if (getNumSquaresToMove() <= 0)
		{
			setDirection(getViableDirection());
			setNumSquaresToMove();
		}
		else
		{
			Direction dir = getWorld()->canTurn(getX(), getY(), getDirection());
			if (dir != none)
			{
				if (getCanTurn() == true)
				{
					setDirection(dir);
					setNumSquaresToMove();
					setTurnCounter(200);
					setCanTurn(false);
				}
			}
		}

		if (getWorld()->canStepHere(getX(), getY(), getDirection()))
			takeStep(getDirection());
		else
			setNumSquaresToMove(0);

		setTickCounter();
		return;
	}
}

GraphObject::Direction Protester::getViableDirection()
{
	Direction dir;
	dir = getWorld()->getRandDir();

	while (!getWorld()->canStepHere(getX(), getY(), dir))
	{
		dir = getWorld()->getRandDir();
	}
	return dir;
}

// HARDCORE PROTESTER IMPLEMENTATION //

HardcoreProtester::HardcoreProtester(int ID, int HP, StudentWorld* world)
	: Protester(IID_HARD_CORE_PROTESTER, 20, world)
{
	setTickCounter();
}

HardcoreProtester::~HardcoreProtester()
{}

int HardcoreProtester::isProtester()
{
	// if its leaveField member is true, then treat it as if it isn't a Protester, so it can't get annoyed further 
	if (getLeaveField() == false)
		return 2;
	return 0;
}

void HardcoreProtester::getAnnoyed(char cause)
{
	// if caused by gold
	if (cause == 'G')
	{
		// just stare and return
		setTickCounter(getWorld()->max(50, 100 - getWorld()->getLevel() * 10));
		return;
	}
	// if it gets squirted and the result of squirt is death
	else if (cause == 'S' && getHP() - 2 <= 0)
	{
		reduceHP(2);
		setLeaveField(true);
		GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
		setTickCounter(0);
		getWorld()->increaseScore(250);
		return;
	}
	else
	{
		Protester::getAnnoyed(cause);
	}
}

void HardcoreProtester::doSomething()
{
	int M = 16 * getWorld()->getLevel() * 2;

	if (!normalMove1())
	{	
		Direction dir = none;
		int tempX = getX();
		int tempY = getY();
		dir = getWorld()->getCloseToFrack(tempX, tempY);

		// if he is <= M legal steps away from Frackman
		// determine which direction to face to get closer to frackman
		// setDirection(this dir)
		// takeStep(getDirection());
		if (!getWorld()->isWithinShoutingDistance(getX(), getY()) && dir != none) 
		{
			setDirection(dir);
			moveTo(tempX, tempY);
			setTickCounter();
			return;
		}
		else
		{
			normalMove2();
		}
	}
}