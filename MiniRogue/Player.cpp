
#include "Player.h"
#include "GameObject.h"
#include "utilities.h"
#include "Scroll.h"
#include "Dungeon.h"
#include <iostream>
#include <string>
using namespace std;

Player::Player(int r, int c) : Actor(r, c, 20, 2, 2, 2, 0)
{
    // initialize a player with the same base statistics and weapon
    m_maxHP = 20;
    Weapon* w = new Weapon(r, c, "shortsword");
    setWeapon(w);
    m_inventory.push_back(w);
}


Player::~Player()
{
    for(int i = 0; i < m_inventory.size(); i++)
    {
        // only delete if it is not the weapon because the weapon will be deleted by actor's destructor
        if(m_inventory[i] != playerWeapon())
            delete m_inventory[i];
    }
    m_inventory.clear();
}

Player::Player(const Player& other) : Actor(other.row(), other.col(), other.hitPoints(), other.armorPoints(), other.strengthPoints(), other.dexPoints(), other.sleepTime())
{
    setHP(other.hitPoints());
    setArmor(other.armorPoints());
    setStrength(other.strengthPoints());
    setDex(other.dexPoints());
    setSleep(other.sleepTime());
    m_maxHP = other.m_maxHP;
    for(int i = 0; i < m_inventory.size(); i++)
    {
        GameObject* g = new GameObject(*other.m_inventory[i]);
        m_inventory[i] = g;
    }
    Weapon* w = new Weapon(*weapon());
    setWeapon(w);
}

Player& Player::operator=(const Player &other)
{
    if(this != &other)
    {
        Player temp(other);
        swap(temp);
    }
    
    return *this;
}

int Player::maxHP()
{
    return m_maxHP;
}

GameObject* Player::playerWeapon()
{
    return weapon();
}

void Player::setMaxHP(int hp)
{
    m_maxHP = hp;
}

void Player::setHP(int hp)
{
    if(hp > m_maxHP)
        Actor::setHP(m_maxHP);
    else
        Actor::setHP(hp);
}

string Player::addToInventory(GameObject* c)
{
    // check if inventory is full
    if(m_inventory.size() < 26)
    {
        // push GameObject into vector if inventory is not full
        m_inventory.push_back(c);
        // determine what string commentary should be returned based on what type of object it is
        if(!isScroll(c))
            return "You pick up " + c->description() + ".";
        else
            return "You pick up a scroll called " + c->description() + ".";
    }
    else
    {
        return "Your knapsack is full; you can't pick that up.";
    }
}

void Player::displayInventory()
{
    // keep a character counter that will properly number the objects
    char counter = 'a';
    string desc = "";
    clearScreen();
    cout << "Inventory: " << endl;
    
    for(int i = 0; i < m_inventory.size(); i++)
    {
        cout << "   " << counter << ". ";
        if(m_inventory[i]->classification() == "scroll")
            cout << "A scroll called ";
        cout << m_inventory[i]->description() << endl;
        counter++;
    }
}

string Player::wieldWeapon(char choice)
{
    char c = 'a';
    int pos = -1;
    GameObject* go = nullptr;;
    for(int i = 0; i < m_inventory.size(); i++)
    {
        // determine which object is chosen based off of the character counter
        if(c == choice)
        {
            go = m_inventory[i];
            pos = i;
            break;
        }
        c++;
    }
    if(pos != -1)
    {
        // try to dynamic cast it to weapon
        Weapon* w = dynamic_cast<Weapon*>(go);
        if(w != nullptr) // means the GameObject is indeed a weapon
        {
            setWeapon(w);
            return "You are wielding " + w->description() + ".";
        }
        else // game object was not a weapon
        {
            return "You can't wield a scroll.";
        }

    }
    return "";

}

string Player::readScroll(char choice, Dungeon* dun)
{
    string result = "";
    char c = 'a';
    int pos = -1;
    GameObject* go = nullptr;
    for(int i = 0; i < m_inventory.size(); i++)
    {
        // determine which object is chosen based off of the character counter
        if(c == choice)
        {
            go = m_inventory[i];
            pos = i;
            break;
        }
        c++;
    }
    
    if(pos != -1)
    {
        // try to dynamic cast into scroll
        Scroll* s = dynamic_cast<Scroll*>(go);
        if(s != nullptr) // means go was indeed a scroll
        {
            result = "You read the scroll called " + s->description() + ".\n" + s->actionString();
            s->addEffect(this, dun);
            
            // since we want to get rid of it from our inventory, need to delete it from inventory and remove its pointer from the vector
            delete(*(m_inventory.begin() + pos));
            m_inventory.erase(m_inventory.begin() + pos);
        }
        else
        {
            result = "You can't read a weapon.";
        }
    }

    return result;
}


bool Player::isScroll(GameObject *g)
{
    Scroll* s = dynamic_cast<Scroll*>(g);
    if(s != nullptr)
    {
        return true;
    }
    else
        return false;
}

string Player::classification()
{
    return "Player";
}


void Player::randomIncreaseHP()
{
    bool increase = trueWithProbability(0.1);
    if(increase)
    {
        setHP(hitPoints()+1);
    }
}

void Player::swap(Player &other)
{
    int temp = m_maxHP;
    m_maxHP = other.m_maxHP;
    other.m_maxHP = temp;
    

    m_inventory.swap(other.m_inventory);
}

char Player::reachable(Dungeon* dun, int sr, int sc)
{
    dun->updateGrid();
    dun->isTravelable(sr, sc);
    return ' ';
}

void Player::dropItem(Dungeon *dun, int sr, int sc)
{
    dun->updateGrid();
    dun->isTravelable(sr, sc);
}
