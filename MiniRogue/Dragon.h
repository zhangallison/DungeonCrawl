
#ifndef Dragon_hpp
#define Dragon_hpp

//#include "Monster.h"
#include "Actor.h"

class Dragon : public Actor
{
public:
    Dragon(int r, int c);
    virtual ~Dragon();
    
    virtual string classification();
    virtual void dropItem(Dungeon *dun, int r, int c);
    virtual char reachable(Dungeon *dun, int sr, int sc);
    
    
    void randomIncreaseHP();
    
private:
    int m_row;
    int m_col;
};

#endif /* Dragon_hpp */
