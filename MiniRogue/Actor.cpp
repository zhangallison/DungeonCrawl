// Actor: base class for Players and monsters

#include "Actor.h"
#include "utilities.h"
#include "Dungeon.h"
#include "GameObject.h"
#include "Weapon.h"
#include "Player.h"
#include <cstdlib>
using namespace std;

Actor::Actor(int r, int c, int hp, int ap, int sp, int dp, int st)
{
    m_row = r;
    m_col = c;

    m_hitPoints = hp;
    m_armorPoints = ap;
    m_strengthPoints = sp;
    m_dexPoints = dp;
    m_sleepTime = st;
    m_weapon = nullptr;
}

Actor::~Actor()
{
    delete m_weapon;
}

int Actor::row() const
{
    return m_row;
}

int Actor::col() const
{
    return m_col;
}

int Actor::hitPoints() const
{
    return m_hitPoints;
}

int Actor::armorPoints() const
{
    return m_armorPoints;
}

int Actor::strengthPoints() const
{
    return m_strengthPoints;
}

int Actor::dexPoints() const
{
    return m_dexPoints;
}

int Actor::sleepTime() const
{
    return m_sleepTime;
}

Weapon* Actor::weapon() const
{
    return m_weapon;
}

// add rest of setters ...

void Actor::setRow(int r)
{
    m_row = r;
}

void Actor::setCol(int c)
{
    m_col = c;
}

void Actor::setHP(int hp)
{
    if(hp < 0)
        m_hitPoints = 0;
    else if(hp > 99)
        m_hitPoints = 99;
    else if(hp > 0 && hp <= 99)
        m_hitPoints = hp;
        
}

void Actor::setArmor(int ap)
{
    if(ap < 0)
        m_armorPoints = 0;
    else if(ap > 99)
        m_armorPoints = 99;
    else if(ap <= 99 && ap >= 0)
        m_armorPoints = ap;
}

void Actor::setStrength(int sp)
{
    if(sp < 0)
        m_strengthPoints = 0;
    else if(sp > 99)
        m_strengthPoints = 99;
    else if(sp <= 99 && sp >= 0)
        m_strengthPoints = sp;
}

void Actor::setDex(int dp)
{
    if(dp < 0)
        m_dexPoints = 0;
    else if(m_dexPoints > 99)
        m_dexPoints = 99;
    else if(dp <= 99 && dp >= 0)
        m_dexPoints = dp;
}

void Actor::setSleep(int sp)
{
    if(sp > 9)
        m_sleepTime = 9;
    else if(sp < 0)
        m_sleepTime = 0;
    else if(sp <= 9 && sp >= 0)
        m_sleepTime = sp;
}

void Actor::setWeapon(Weapon* w)
{
    m_weapon = w;
}

string Actor::move(Dungeon* dun, char dir, int& r, int& c)
{
    string result = "";
    string actorType = "";
    // store classification into variable - helpful in determining if actor should be attacking a monster or a player
    if(classification() == "Bogeyman" || classification() == "Dragon" || classification() == "Goblin" || classification() == "Snakewoman")
    {
        actorType = "Monster";
    }
    else
        actorType = "Player";
    int rnew = r;
    int cnew = c;
    

    if(dir == ARROW_LEFT) // left
    {
        
        if((dun->isWall(r, c-1)))
            ;
        // if the coordinate to the left is NOT a wall:
        else
        {
            if(actorType == "Player")
            {
                Actor* m = dun->findMonster(r, c-1); 
                if(m == nullptr) // monster not found, proceed as normal
                    cnew--;
                else
                {
                    result = attack(m);
                }
            }
            else
            {
                if(r == dun->player()->row() && (c-1) == dun->player()->col())
                {
                    result = attack(dun->player());
                }
                else
                {
                    cnew--;
                }
            }
        }
            
    }
    else if(dir == ARROW_RIGHT) // right
    {
        if((dun->isWall(r, c+1)))
            ;
        else
        {
            if(actorType == "Player")
            {
                Actor* m = dun->findMonster(r, c+1);
                if(m == nullptr) // monster not found, proceed as normal
                    cnew++;
                else
                {
                    result = attack(m);
                }
            }
            else
            {
                if(r == dun->player()->row() && (c+1) == dun->player()->col())
                {
                    result = attack(dun->player());
                }
                else
                {
                    cnew++;
                }
            }
        }
    }
    else if(dir == ARROW_UP) // up
    {
        if((dun->isWall(r-1, c)))
            ;
        else
        {
            if(actorType == "Player")
            {
                Actor* m = dun->findMonster(r-1, c);
                if(m == nullptr) // monster not found, proceed as normal
                    rnew--;
                else
                {
                    result = attack(m);
                }
            }
            else
            {
                if((r-1) == dun->player()->row() && c == dun->player()->col())
                {
                    result = attack(dun->player());
                }
                else
                {
                    rnew--;
                }
            }
        }
    }
    else if(dir == ARROW_DOWN) // down
    {
        if((dun->isWall(r+1, c)))
            ;
        else
        {
            if(actorType == "Player")
            {
                Actor* m = dun->findMonster(r+1, c);
                if(m == nullptr) // monster not found, proceed as normal
                    rnew++;
                else
                {
                    result = attack(m);
                }
            }
            else
            {
                if((r+1) == dun->player()->row() && c == dun->player()->col())
                {
                    result = attack(dun->player());
                }
                else
                {
                    rnew++;
                }
            }
        }
    }
    m_row = rnew;
    m_col = cnew;
    
    dun->updateGrid();
    // ??? how does this work with monsters
    dun->insert(r, c, ' '); // need?
//    dun->insert(rnew, cnew, '@');
    
    return result;
    
}


// attack function:
// return a string saying what happened
string Actor::attack(Actor* defender)
{
    string result = "";
    bool attackerHit = false;
    bool isAsleep = false;
    int attackerPoints = dexPoints() + weapon()->dexBonus();
    int defenderPoints = defender->dexPoints() + defender->armorPoints();
    if(randInt(1, attackerPoints) >= randInt(1, defenderPoints))
       attackerHit = true;
    if(attackerHit)
    {
        int damagePoints = randInt(0, strengthPoints() + weapon()->dmgAmount() - 1);
        defender->setHP(defender->hitPoints() - damagePoints);
        // if weapon()->type() == "fang", call put to sleep
        if(weapon()->type() == "fang")
        {
            isAsleep = weapon()->putToSleep(defender);
        }
    }
    
    if(classification() != "Player")
        result = "the ";
    result +=  classification() + weapon()->actionString() + defender->classification();
    if(attackerHit)
    {
        if(defender->hitPoints() <= 0)
            result += " dealing a final blow.";
        else
        {
            result += " and hits";
            if(isAsleep)
                result += ", putting " + defender->classification() + " to sleep.";
            else
                result += ".";
        }
    }
    else
    {
         result += " and misses.";
    }
    
    // send a status message
    return result;
}

char Actor::dirCloser(Dungeon* dun, int sr, int sc, int er, int ec)
{
    // the current horizontal and vertical distance between players
    int curHoriz = abs(ec-sc);
    int curVert = abs(er-sr);
    
    // calculate the possible new horizontal and vertical distance between players
    int south = abs(er-(sr+1));
    int west = abs(ec-(sc-1));
    int north = abs(er-(sr-1));
    int east = abs(ec-(sc+1));
    
    // if moving in any of the above directions decreases direction between players return that direction
    
    //  may want to call move here
    if((south < curVert) && dun->isTravelable(sr+1, sc))
        return ARROW_DOWN;
    else if((west < curHoriz) && dun->isTravelable(sr, sc-1))
        return ARROW_LEFT;
    else if((north < curVert) && dun->isTravelable(sr-1, sc))
        return ARROW_UP;
    else if((east < curHoriz) && dun->isTravelable(sr, sc+1))
        return ARROW_RIGHT;
    
    return ' ';
}
/*
char Actor::reachable(Dungeon *dun, int sr, int sc)
{
    
    return ' ';
}

void Actor::dropItem(Dungeon* dun, int r, int c)
{
    
}*/

string Actor::classification()
{
    return "";
}


