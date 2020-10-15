
#ifndef Snakewoman_hpp
#define Snakewoman_hpp

#include "Actor.h"

class Snakewoman : public Actor
{
public:
    Snakewoman(int r, int c);
    virtual ~Snakewoman();
    
    virtual string classification();
    virtual void dropItem(Dungeon *dun, int r, int c);
    virtual char reachable(Dungeon *dun, int sr, int sc);
    
private:
    int m_row;
    int m_col;
};

#endif /* Snakewoman_hpp */
