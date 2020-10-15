
#ifndef Weapon_hpp
#define Weapon_hpp

#include "GameObject.h"

class Actor;

class Weapon : public GameObject
{
public:
    Weapon(int r, int c, string type);
    virtual ~Weapon();
    
    int dexBonus();
    int dmgAmount();
    virtual string description();
    virtual string classification();
    string actionString();
    bool putToSleep(Actor* a);
};

#endif /* Weapon_hpp */
