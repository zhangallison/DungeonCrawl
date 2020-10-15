
#include "Weapon.h"
#include "Actor.h"
#include "utilities.h"
using namespace std;

Weapon::Weapon(int r, int c, string type) : GameObject(r, c, type)
{
    
}

Weapon::~Weapon()
{
    
}


// determine which type of weapon and accordingly give it its dexterity bonus
int Weapon::dexBonus()
{
    string t = type();
    if(t == "mace")
        return 0;
    else if(t == "shortsword")
        return 0;
    else if(t == "longsword")
        return 2;
    else if(t == "axe")
        return 5;
    else if(t == "fang")
        return 3;
    else
        return 0;
}

// determine which type of weapon it is and accordingly give it its damage amount
int Weapon::dmgAmount()
{
    string t = type();
    if(t == "mace")
        return 2;
    else if(t == "shortsword")
        return 2;
    else if(t == "longsword")
        return 4;
    else if(t == "axe")
        return 5;
    else if(t == "fang")
        return 2;
    return 0;
}

string Weapon::description()
{
    string t = type();
    if(t == "mace")
        return t;
    else if(t == "shortsword")
        return "short sword";
    else if(t == "longsword")
        return "long sword";
    else if(t == "axe")
        return "magic axe";
    else if(t == "fang")
        return "magic fangs of sleep";
    
    return "";
}

string Weapon::actionString()
{
    string t = type();
    if(t == "mace")
        return " swings mace at ";
    else if(t == "shortsword")
        return " slashes short sword at ";
    else if(t == "longsword")
        return " swings long sword at ";
    else if(t == "axe")
        return " chops magic axe at ";
    else if(t == "fang")
        return " strikes magic fangs at ";
    return "";
}


// only applicable to fangs
bool Weapon::putToSleep(Actor *a)
{
    if(type() != "fang")
        return false;
    
    // calculate the probably that the actor does indeed fall sleep
    bool sleep = trueWithProbability(0.2);
    if(sleep)
    {
        int newSleepTime = randInt(2, 6);
        int oldSleepTime = a->sleepTime();
        if(oldSleepTime != 0) // defender is already asleep
        {
            // only set time to the max sleep time out of new sleep time and old sleep time
            if(newSleepTime > oldSleepTime)
                a->setSleep(newSleepTime);
        }
        else
        {
            a->setSleep(newSleepTime);
        }
        return true;
        
    }
    return false;
}

string Weapon::classification()
{
    return "weapon";
}
