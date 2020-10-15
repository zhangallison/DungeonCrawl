

#include "Scroll.h"
#include "Player.h"
#include "utilities.h"
#include "Actor.h"
#include "Dungeon.h"
using namespace std;

Scroll::Scroll(int r, int c, string type) : GameObject(r, c, type)
{
    
}

Scroll::~Scroll()
{
    
}

string Scroll::description()
{
    if(type() == "tele")
        return "scroll of teleportation";
    else if(type() == "armor")
        return "scroll of enhance armor";
    else if(type() == "strength")
        return "scroll of strength";
    else if(type() == "health")
        return "scroll of enhance health";
    else if(type() == "dex")
        return "scroll of enhance dexterity";
    return "";
}

void Scroll::addEffect(Player* p, Dungeon* dun)
{
    string t = type();
    if(t == "tele")
    {
        int randRow = randInt(0, 17);
        int randCol = randInt(0, 69);
        dun->generateRandomCoord(randRow, randCol);
        p->setRow(randRow);
        p->setCol(randCol);
    }
    else if(t == "armor")
    {
        p->setArmor(p->armorPoints() + randInt(1,3));
    }
    else if(t == "strength")
    {
        p->setStrength(p->strengthPoints() + randInt(1,3));
    }
    else if(t == "health")
    {
        p->setMaxHP(p->hitPoints() + randInt(3, 8));
    }
    else if(t == "dex")
    {
        p->setDex(p->dexPoints()+1);
    }
}

string Scroll::classification()
{
    return "scroll";
}

string Scroll::actionString()
{
    string t = type();
    if(t == "tele")
        return "You feel your body wrenched in space and time.";
    else if(t == "armor")
        return "Your armor glows blue.";
    else if(t == "strength")
        return "Your muscles bulge.";
    else if(t == "health")
        return "You feel your heart beating stronger.";
    else if(t == "dex")
        return "You feel like less of a klutz.";
    
    return "";
    
}
