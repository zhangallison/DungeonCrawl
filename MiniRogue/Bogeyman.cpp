
#include "Bogeyman.h"
#include "utilities.h"
#include "Dungeon.h"
#include "Weapon.h"
#include "Player.h"
#include <stack>
using namespace std;

Bogeyman::Bogeyman(int r, int c) : Actor(r, c, randInt(5,10), 2, randInt(2,3), randInt(2,3), 0)
{
    m_row = r;
    m_col = c;
    Weapon* w = new Weapon(r, c, "shortsword");
    setWeapon(w);
    
}

Bogeyman::~Bogeyman()
{
    
}

string Bogeyman::classification()
{
    return "Bogeyman";
}

void Bogeyman::dropItem(Dungeon *dun, int r, int c)
{
    if(!(dun->hasObject(r, c)) && dun->charAtPoint(r, c) != '>')
    {
        bool drop = trueWithProbability(0.1);
        if(drop)
        {
            Weapon* w = new Weapon(r, c, "axe");
            dun->addObject(w);
            dun->insert(r, c, ')');
        }
    }

}

char Bogeyman::reachable(Dungeon *dun, int sr, int sc) // do i need starting and ending coordinate or just call row and col
{
    Player* p = dun->player();
    int er = p->row();
    int ec = p->col();
    
    // calculates the total horizontal and vertical distance between the player and the monster
    int horiz = abs(ec - sc);
    int vert = abs(er - sr);
    
    // if it is less than or equal to 5 means Bogeyman is 5 steps away from player disregarding walls
    if((horiz + vert) <= 5)
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
