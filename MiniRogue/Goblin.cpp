
#include "Goblin.h"
#include "utilities.h"
#include "Weapon.h"
#include "Dungeon.h"
#include "Player.h"
#include <iostream>
using namespace std;

// coord class used for queue recursive path function
class Coord
{
  public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
  private:
    int m_row;
    int m_col;
};

Goblin::Goblin(int r, int c, int dist) : Actor(r, c, randInt(15, 20), 1, 3, 1, 0)
{
    m_distance = dist;
    m_row = r;
    m_col = c;
    Weapon* w = new Weapon(r, c, "shortsword");
    setWeapon(w);
}


Goblin::~Goblin()
{
    
}

string Goblin::classification()
{
    return "Goblin";
}

void Goblin::dropItem(Dungeon *dun, int r, int c)
{
    if (!(dun->hasObject(r, c)) && dun->charAtPoint(r, c) != '>')
    {
        // calculates first whether a goblin will drop an object at all
        bool drop = trueWithProbability(1.0/3.0);
        if(drop)
        {
            // then determines which type of weapon to drop
            int type = randInt(1,2);
            
            if(type == 1)
            {
                 Weapon* w = new Weapon(r, c, "axe");
                 dun->addObject(w);
                 dun->insert(r, c, ')');
            }
            else if(type == 2)
            {
                 Weapon* w = new Weapon(r, c, "fang");
                 dun->addObject(w);
                 dun->insert(r, c, ')');
            }
        }
    }
    

}

char Goblin::reachable(Dungeon *dun, int sr, int sc)
{
    
    Player* p = dun->player();
    int er = p->row();
    int ec = p->col();
    
    // if goblin is right next to the player return the direction it should travel to attack the player
    if(sr-1 == er && sc == ec)
        return ARROW_UP;
    else if(sr == er && sc+1 == ec)
        return ARROW_RIGHT;
    else if(sr+1 == er && sc == ec)
        return ARROW_DOWN;
    else if(sr == er && sc-1 == ec)
        return ARROW_LEFT;
    
    
    // initialize a matrix representing the distance from the player to the goblin at that position in the dungeon
    int dist[18][70];
    for(int i = 0; i < 18; i++)
    {
        for(int j = 0; j < 70; j++)
        {
            dist[i][j] = m_distance + 1;
        }
    }
    dist[sr][sc] = 0;
    
    
    // initialize a matrix representing whether the goblin has visited that coordinate in its search for the player already
    int visitedArray[18][70];
    for(int i = 0; i < 18; i++)
    {
        for(int j = 0; j < 70; j++)
        {
            visitedArray[i][j] = 0;
        }
    }
    
    
    // initializes matrix representing the direction that the goblin would have to travel if it were to retrace its steps from the path of player to goblin
    char playerToGob[18][70];
    for (int i = 0; i < 18; i++)
    {
        for(int j = 0; j < 70; j++)
        {
            playerToGob[i][j] = ' ';
        }
    }
    
    // initializes a queue of Coords to represent the coordinates the goblin visits in its search for the player
    // pushes starting coordinate of goblin onto queue
    Coord start(sr, sc);
    queue<Coord> pathToPlayer;
    pathToPlayer.push(start);
    
    // Calls recursive goblin path search algorithm
    // Returns the shortest path length the goblin could take to get to (er, ec)
    int pathLength = path(dun, pathToPlayer, er, ec, dist, visitedArray, playerToGob);

    
    // if the shortest path is less than the goblin's smell distance
    if(pathLength <= m_distance)
    {
        // interpret the path from the player to goblin to return the direction the goblin should travel to accomplish its shortest path by calling another recursive function:
        char trav = travelPath(playerToGob, er, ec, sr, sc);
        if(trav == 'n')
            return ARROW_UP;
        else if(trav == 'e')
            return ARROW_RIGHT;
        else if(trav == 's')
            return ARROW_DOWN;
        else if(trav == 'w')
            return ARROW_LEFT;
        return trav;
    }
    
    // otherwise the goblin is too far from the player to smell it
    else
    {
        return ' ';
    }
    
    return ' ';
}

// My first attempt at a recursive goblin search function, if you are curious. Was too slow past 13 smell distance
int Goblin::shortestPath(Dungeon* dun, int& dr, int& dc, int sr, int sc, int er, int ec, int ref, int visited[18][70])
{
    // mark sr, sc as visited if function becomes really slow
    // int pathNorth = reachable(dun, dirx, diry, sr-1, sc, visited) // dirx and diry is a direction vector on how to move
    // ...
    // if all of them > 15 return smelldistance +1 // also unvisit
    // find which path is the shortest
    // return 1 + pathNorth;

    if(ref > m_distance)
    {
        return m_distance + 1;
    }
    
    
    if(sr == er && sc == ec)
        return 0;
    
    if(sr > 17 || sc > 69)
    {
        return m_distance+1;
    }
    
    
    int pathNorth = m_distance + 1;
    int pathEast = m_distance + 1;
    int pathSouth = m_distance + 1;
    int pathWest = m_distance + 1;
    if(dun->isOccupiable(sr-1, sc))
    {
        pathNorth = shortestPath(dun, dr, dc, sr-1, sc, er, ec, ref+1, visited);
    }
    if(dun->isOccupiable(sr, sc+1))
    {
        pathEast = shortestPath(dun, dr, dc, sr, sc+1, er, ec, ref+1, visited);
    }
    if(dun->isOccupiable(sr+1, sc))
    {
        pathSouth = shortestPath(dun, dr, dc, sr+1, sc, er, ec, ref+1, visited);
    }
    if(dun->isOccupiable(sr, sc-1))
    {
        pathWest = shortestPath(dun, dr, dc, sr, sc-1, er, ec, ref+1, visited);
    }

    
    int dir[4];
    dir[0] = pathNorth;
    dir[1] = pathEast;
    dir[2] = pathSouth;
    dir[3] = pathWest;
    
    
    
    int min = 0;
    for(int i = 0; i < 4; i++)
    {
        if(dir[i] < dir[min])
            min = i;
    }
    
    if(dir[min] > m_distance)
    {
        return m_distance+1;
    }
    
    if(min == 0) // north
    {
        dr = -1;
        dc = 0;
        return 1 + pathNorth;
    }
    else if(min == 1) // east
    {
        dr = 0;
        dc = 1;
        return 1 + pathEast;
    }
    else if(min == 2) // south
    {
        dr = 1;
        dc = 0;
        return 1 + pathSouth;
    }
    else if(min == 3) // west
    {
        dr = 0;
        dc = -1;
        return 1 + pathWest;
    }
    
    return m_distance+1;
}


// Idea behind recursive function:
// Visit each coordinate in the queue
// If any of the coordinates N, E, S, W of the coordinate is travelable and not visited, add to the queue and
//      mark in the visited matrix that the spot was visit and mark in the 'prev' matrix what direction you
//      would have to travel to get back to the starting spot, also increment the distance in the position of
//      starting coordinate in distance matrix
// if the coordinate is equal to (er, ec), return the distance at that spot in the distance matrix
// if the queue is empty, means all possible paths do not lead to the player so return a number larger than the smell distance
// return the function call with same parameters
int Goblin::path(Dungeon* dun, queue<Coord>& coords, int er, int ec, int distance[18][70], int visited[18][70], char prev[18][70])
{
    
    if(!coords.empty())
    {
        Coord front = coords.front();
        coords.pop();
        int sr = front.r();
        int sc = front.c();
        if(front.r() == er && front.c() == ec)
            return distance[sr][sc];
        else
        {
            if(dun->isTravelable(sr-1, sc) && visited[sr-1][sc] != 1) // north
            {
                Coord north(sr-1, sc);
                visited[sr-1][sc] = 1;
                coords.push(north);
                prev[sr-1][sc] = 's';
                distance[sr-1][sc] = distance[sr][sc] + 1;
            }
            if(dun->isTravelable(sr, sc+1) && visited[sr][sc+1] != 1) // east
            {
                Coord east(sr, sc+1);
                visited[sr][sc+1] = 1;
                coords.push(east);
                prev[sr][sc+1] = 'w';
                distance[sr][sc+1] = distance[sr][sc] + 1;
            }
            if(dun->isTravelable(sr+1, sc) && visited[sr+1][sc] != 1) // south
            {
                Coord south(sr+1, sc);
                visited[sr+1][sc] = 1;
                coords.push(south);
                prev[sr+1][sc] = 'n';
                distance[sr+1][sc] = distance[sr][sc] + 1;
            }
            if(dun->isTravelable(sr, sc-1) && visited[sr][sc-1] != 1)
            {
                Coord west(sr, sc-1);
                visited[sr][sc-1] = 1;
                coords.push(west);
                prev[sr][sc-1] = 'e';
                distance[sr][sc-1] = distance[sr][sc] +1;
            }
        }
    }
    else
    {
        return m_distance + 1;
    }
    
    return path(dun, coords, er, ec, distance, visited, prev);
}


// Moves throughout the passed matrix starting at (sr, sc) and interpreting the directions on the grid and moving in that direction until the coordinate just before (er, ec) is reached
char Goblin::travelPath(char matrix[18][70], int sr, int sc, int er, int ec)
{
    int newR = sr;
    int newC = sc;
    int newDir = ' ';
    char curDir = matrix[sr][sc];
    if(curDir == 'n')
    {
        newR--;
        newDir = 's';
    }
    else if(curDir == 'e')
    {
        newC++;
        newDir = 'w';
    }
    else if(curDir == 's')
    {
        newR++;
        newDir = 'n';
    }
    else if(curDir == 'w')
    {
        newC--;
        newDir = 'e';
    }
    else if(curDir == ' ')
        return ' ';
    
    if(newR == er && newC == ec)
        return newDir;
    
    else
    {
        return travelPath(matrix, newR, newC, er, ec);
    }
}

