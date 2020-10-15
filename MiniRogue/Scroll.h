
#ifndef Scroll_hpp
#define Scroll_hpp

#include "GameObject.h"

class Player;
class Actor;
class Dungeon;

class Scroll : public GameObject
{
public:
    Scroll(int r, int c, string type);
    virtual ~Scroll();
    
    virtual string description();
    virtual string classification();
    string actionString();
    
    void addEffect(Player* p, Dungeon* dun);
    //void putToSleep(Actor* a);
};


#endif /* Scroll_hpp */
