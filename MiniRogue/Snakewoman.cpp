
#include "Snakewoman.h"
#include "utilities.h"
#include "Weapon.h"
#include "Dungeon.h"
#include "Player.h"
using namespace std;

Snakewoman::Snakewoman(int r, int c) : Actor(r, c, randInt(3,6), 3, 2, 3, 0)
{
    m_row = r;
    m_col = c;
    Weapon* w = new Weapon(r, c, "fang");
    setWeapon(w);
    // set weapon
}

Snakewoman::~Snakewoman()
{
    
}

string Snakewoman::classification()
{
    return "Snakewoman";
}

void Snakewoman::dropItem(Dungeon *dun, int r, int c)
{
    if(!(dun->hasObject(r,c)) && dun->charAtPoint(r, c) != '>')
    {
        // determine whether an item should be dropped at all
        bool drop = trueWithProbability(1.0/3.0);
        if(drop)
        {
             Weapon* w = new Weapon(r, c, "fang");
             dun->addObject(w);
             dun->insert(r, c, ')');
        }
    }

}

char Snakewoman::reachable(Dungeon *dun, int sr, int sc)
{
    Player* p = dun->player();
    int er = p->row();
    int ec = p->col();
    
    // calculate the horiztonal and vertical difference between the snakewoman and player
    int horiz = abs(ec - sc);
    int vert = abs(er - sr);
    
    // if combined distance is less than three, that means the player is 3 steps away disregarding walls or other objects
    if((horiz + vert) <= 3)
    {
        // return the direction returned by this function that calculates the direction that will move
        // the Bogeyman closer to the player's row and column
        return dirCloser(dun, sr, sc, er, ec);
    }
    
    else
    {
        return ' ';
    }
}
