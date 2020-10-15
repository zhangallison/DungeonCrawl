
#include "Dragon.h"
#include "utilities.h"
#include "Weapon.h"
#include "Scroll.h"
#include "Dungeon.h"
#include "Player.h"

Dragon::Dragon(int r, int c) : Actor(r, c, randInt(20, 25), 4, 4, 4, 0)
{
    m_row = r;
    m_col = c;
    Weapon* w = new Weapon(r, c, "longsword");
    setWeapon(w);
}


Dragon::~Dragon()
{
    
}

string Dragon::classification()
{
    return "Dragon";
    
}

void Dragon::dropItem(Dungeon *dun, int r, int c)
{
    int type = randInt(1, 5);
    // choose which type of scroll to drop
    if (!dun->hasObject(r, c) && dun->charAtPoint(r, c) != '>')
    {
        if(type == 1)
        {
             Scroll* s = new Scroll(r, c, "tele");
             dun->addObject(s);
             dun->insert(r, c, '?');
        }
        else if(type == 2)
        {
             Scroll* s = new Scroll(r, c, "armor");
             dun->addObject(s);
             dun->insert(r, c, '?');
        }
        else if(type == 3)
        {
             Scroll* s = new Scroll(r, c, "strength");
             dun->addObject(s);
             dun->insert(r, c, '?');
        }
        else if(type == 4)
        {
             Scroll* s = new Scroll(r, c, "health");
             dun->addObject(s);
             dun->insert(r, c, '?');
        }
        else if(type == 5)
        {
             Scroll* s = new Scroll(r, c, "dex");
             dun->addObject(s);
             dun->insert(r, c, '?');
        }
    }
    

}

char Dragon::reachable(Dungeon *dun, int sr, int sc)
{
    // dragon's reachable function only returns a meaningful character if it is right next to the player, and if it is return the direction the player is located
    int er = dun->player()->row();
    int ec = dun->player()->col();
    
    if(sr+1 == er && sc == ec)
        return ARROW_DOWN;
    else if(sr-1 == er && sc == ec)
        return ARROW_UP;
    else if(sr == er && sc+1 == ec)
        return ARROW_RIGHT;
    else if(sr == er && sc-1 == ec)
        return ARROW_LEFT;

    return ' ';
}

void Dragon::randomIncreaseHP()
{
    bool increase = trueWithProbability(0.1);
    if(increase)
    {
        if(hitPoints() < 99)
            setHP(hitPoints()+1);
    }
}
