
#ifndef Player_h
#define Player_h

#include "Actor.h"
#include "Weapon.h"
#include <vector>
using namespace std;


class GameObject;
class Scroll;
class Dungeon;

class Player : public Actor
{
public:
    Player(int r, int c);
    virtual ~Player();
    Player(const Player& other);
    Player& operator=(const Player& other);
    
    int maxHP();
    GameObject* playerWeapon();
    
    void setMaxHP(int max);
    virtual void setHP(int hp);
    
    string addToInventory(GameObject* c);
    void displayInventory();
    string wieldWeapon(char choice);
    string readScroll(char choice, Dungeon* dun);
    bool isScroll(GameObject* g);
    void randomIncreaseHP();
    
    void swap(Player& other);
    
    virtual string classification();
    virtual char reachable(Dungeon* dun, int sr, int sc);
    virtual void dropItem(Dungeon* dun, int sr, int sc);
    
private:
    vector<GameObject*> m_inventory;
    int m_maxHP;
};

#endif /* Player_h */
