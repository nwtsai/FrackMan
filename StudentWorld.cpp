#include "StudentWorld.h"
#include <string>
#include <vector>
#include <cstdlib>
#include "Actor.h"
#include "GameController.h"

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir), m_barrelsLeft(4)
{}

StudentWorld::~StudentWorld()
{
	for (int i = 0; i < m_actors.size(); i++)
		delete m_actors.at(i);
	m_actors.clear();

	for (int k = 0; k < m_dirt.size(); k++)
		delete m_dirt.at(k);
	m_dirt.clear();

	delete fracker;
}

int StudentWorld::init()
{
	fracker = new FrackMan(this); 

	Boulder* B = new Boulder(5, 5, this);
	m_actors.push_back(B);

	for (int r = 0; r < 64; r++)
	{
		for (int c = 0; c < 60; c++)
		{
			if (r < 30 || r > 33)
			{
				Dirt* dirt = new Dirt(r, c);
				m_dirt.push_back(dirt);
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	setDisplayText();
	fracker->doSomething();
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors.at(i)->isStillAlive())
		{
			m_actors.at(i)->doSomething();

			if (!fracker->isStillAlive())
			{
				fracker->setDead();
				return GWSTATUS_PLAYER_DIED;
			}

			if (m_barrelsLeft == 0)
			{
				GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}

	// delete actors that need to be deleted and remove them from the vector
	// below is just following page 19 of the spec's pseudocode
	if (!fracker->isStillAlive())
	{
		fracker->setDead();
		return GWSTATUS_PLAYER_DIED;
	}

	if (m_barrelsLeft == 0)
	{
		GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i < m_actors.size(); i++)
		delete m_actors.at(i);
	m_actors.clear();

	for (int k = 0; k < m_dirt.size(); k++)
		delete m_dirt.at(k);
	m_dirt.clear();

	delete fracker;
}

bool StudentWorld::isThereDirtHere(int x, int y)
{
	vector<Dirt*>::iterator p = m_dirt.begin();
	while (p != m_dirt.end())
	{
		if ((*p)->getX() == x && (*p)->getY() == y)
		{
			return true;
		}
		else
		{
			p++;
		}
	}
	return false;
}

void StudentWorld::destroyDirt(int x, int y)
{
	vector<Dirt*>::iterator p = m_dirt.begin();
	while (p != m_dirt.end())
	{
		if ((*p)->getX() - x >= 0 && (*p)->getX() - x < 4 && (*p)->getY() - y >= 0 && (*p)->getY() - y < 4)
		{
			vector<Dirt*>::iterator temp = p;
			delete *p;
			p = m_dirt.erase(temp);
			GameController::getInstance().playSound(SOUND_DIG);
		}
		else
		{
			p++;
		}
	}
}

void StudentWorld::insertSquirt(int x, int y, int dir)
{
	// Squirt* sqr = new Squirt();
}

int StudentWorld::min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

int StudentWorld::max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

int StudentWorld::randInt(int l, int h)
{
	if (h < l)
		swap(h, l); 
	return l + (rand() % (h - l + 1)); 
}

void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = fracker->getHP();
	int squirts = fracker->getSquirts();
	int gold = fracker->getGold();
	int sonar = fracker->getsCharges();
	int barrelsLeft = m_barrelsLeft;

	string s = formatDisplayText(score, level, lives, health, squirts, gold, sonar, barrelsLeft);
	setGameStatText(s);
}

string StudentWorld::formatDisplayText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft)
{
	stringstream sa;
	string scr = "Scr: ";
	sa << setw(8) << setfill('0') << score;
	string a = sa.str();

	stringstream sb;
	string lvl = "Lvl: ";
	sb << setw(2) << setfill(' ') << level;
	string b = sb.str();

	stringstream sc;
	string lvs = "Lives: ";
	sc << setw(1) << setfill(' ') << lives;
	string c = sc.str();

	stringstream sd;
	string HP = "Hlth: ";
	sd << setw(3) << setfill(' ') << health;
	string d = sd.str();

	stringstream se;
	string wtr = "Water: ";
	se << setw(2) << setfill(' ') << squirts;
	string e = se.str();

	stringstream sf;
	string gld = "Gld: ";
	sf << setw(2) << setfill(' ') << gold;
	string f = sf.str();

	stringstream sg;
	string snr = "Sonar: ";
	sg << setw(2) << setfill(' ') << sonar;
	string g = sg.str();

	stringstream sh;
	string oil = "Oil Left: ";
	sh << setw(2) << setfill(' ') << barrelsLeft;
	string h = sh.str();

	// return scr.append(a.append(lvl.append(b.append(lvs.append(c.append(HP.append(d.append(wtr.append(e.append(gld.append(f.append(snr.append(g.append(oil.append(h)))))))))))))));
	return scr + a + "  " + lvl + b + "  " + lvs + c + "  " + HP + d + "  " + wtr + e + "  " + gld + f + "  " + snr + g + "  " + oil + h;
}