
#ifndef Goblin_h
#define Goblin_h

//#include "Monster.h"
#include "Actor.h"
#include <queue>
#include <vector>
using namespace std;

class Coord;

class Goblin : public Actor
{
public:
    Goblin(int r, int c, int dist);
    virtual ~Goblin();
    
    virtual string classification();
    virtual void dropItem(Dungeon *dun, int r, int c);
    virtual char reachable(Dungeon *dun, int sr, int sc);
    
    int shortestPath(Dungeon* dun, int &dr, int &dc, int sr, int sc, int er, int ec, int ref, int visited[18][70]);
    int path(Dungeon* dun, queue<Coord>& coords, int er, int ec, int distance[18][70], int visited[18][70], char prev[18][70]);
    
    char travelPath(char matrix[18][70], int sr, int sc, int er, int ec);
    
    
private:
    int m_row;
    int m_col;
    int m_distance;
    
};

#endif /* Goblin_h */
