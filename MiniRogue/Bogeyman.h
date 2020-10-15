

#ifndef Bogeyman_h
#define Bogeyman_h

//#include "Monster.h"
#include "Actor.h"

class Bogeyman : public Actor
{
public:
    Bogeyman(int r, int c);
    virtual ~Bogeyman();
    
    virtual string classification();
    virtual void dropItem(Dungeon *dun, int r, int c);
    virtual char reachable(Dungeon *dun, int sr, int sc);
    
    
private:
    int m_row;
    int m_col;
};

#endif /* Bogeyman_h */
