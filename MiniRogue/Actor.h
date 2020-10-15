

#ifndef Actor_h
#define Actor_h

#include <string>
using namespace std;

class Dungeon;
class Weapon;
class GameObject;

class Actor
{
public:
    Actor(int r, int c, int hp, int ap, int sp, int dp, int st);
//    Actor(int r, int c, int hp, int ap, int sp, int dp, int st);
    virtual ~Actor();
    
    // Accessors:
    int row() const;
    int col() const;
    int hitPoints() const;
    int armorPoints() const;
    int strengthPoints() const;
    int dexPoints() const;
    int sleepTime() const;
    Weapon* weapon() const;
    
    // Setter methods:
    void setRow(int r);
    void setCol(int c);
    virtual void setHP(int hp);
    void setArmor(int ap);
    void setStrength(int sp);
    void setDex(int dp);
    void setSleep(int sp);
    void setWeapon(Weapon* w);
    
    string move(Dungeon* dun, char dir, int& r, int& c);
    string attack(Actor* defender);
    char dirCloser(Dungeon* dun, int sr, int sc, int er, int ec); // returns a direction that moves closer to sr to er or sc to ec
    
    // maybe move these into monster class
    virtual char reachable(Dungeon* dun, int sr, int sc) = 0; // implemented differently for monsters
    virtual void dropItem(Dungeon* dun, int r, int c) = 0;
    
    virtual string classification(); // pure virtual
//    bool attemptMove(const Dungeon& dun, char com);
private:
    int m_row;
    int m_col;
    int m_hitPoints;
    int m_armorPoints;
    int m_strengthPoints;
    int m_dexPoints;
    int m_sleepTime;
    Weapon* m_weapon;
};

#endif /* Actor_h */
