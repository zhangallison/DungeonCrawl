
#include "Game.h"
#include "Dungeon.h"
#include "Player.h"
#include "utilities.h"
#include "Bogeyman.h"
#include "Dragon.h"
#include "Goblin.h"
#include "Snakewoman.h"
#include "GameObject.h"
#include "Weapon.h"
#include "Scroll.h"
#include "Room.h"
#include <iostream>
#include <queue>
using namespace std;


Dungeon::Dungeon(int level, int gobDist)
{
    
    m_gobDistance = gobDist;
    createLevel(level);
    
}

Dungeon::~Dungeon()
{
    delete m_player;
    
    vector<Actor*>::iterator j = monsters.begin();
    for( ; j != monsters.end(); j++)
    {
        delete *j;
    }

    vector<GameObject*>::iterator i = objects.begin();
    for( ; i != objects.end(); i++)
    {
        delete *i; // error
    }
    
}

Dungeon::Dungeon(const Dungeon& other)
{
    m_level = other.m_level;
    for(int i = 0; i < 18; i++)
    {
        for(int j = 0; j < 70; j++)
        {
            m_grid[i][j] = other.m_grid[i][j];
            m_objGrid[i][j] = other.m_objGrid[i][j];
        }
    }
    Player* p = new Player(*other.m_player); // is this legal
    m_player = p;
    
    for(int k = 0; k < monsters.size(); k++)
    {
        // instead of actor could switch to monster to make copy construction easier
        // Actor* a = new Actor(*other.monsters[i]);
        // monsters[i] = a;
    }
    
}

Player* Dungeon::player() const
{
    return m_player;
}

int Dungeon::level() const
{
    return m_level;
}

void Dungeon::createLevel(int newLevel)
{
    m_numRooms = 0;
    m_level = newLevel;
    m_rooms.clear();
    
    for(int j = 0; j < 18; j++)
    {
        for(int k = 0; k < 70; k++)
        {
            m_grid[j][k] = '#';
            m_objGrid[j][k] = ' ';
        }
    }
    
    
    
    int startR = randInt(1, 5);
    int startC = randInt(1, 9);
    int startWid = randInt(9, 17);
    int startHeig = randInt(4, 7);
    
    while((startR+startHeig) >= 18 || (startC+startWid) >= 70)
    {
        startR = randInt(1, 5);
        startC = randInt(1, 9);
        startWid = randInt(9, 17);
        startHeig = randInt(4, 7);
    }
    int numRooms = createRoom(startR, startC, startWid, startHeig);
    while(numRooms < 4 || numRooms > 8)
    {
        m_rooms.clear();
        // reset the board
        for(int j = 0; j < 18; j++)
        {
            for(int k = 0; k < 70; k++)
            {
                m_grid[j][k] = '#';
            }
        }
        numRooms = createRoom(startR, startC, startWid, startHeig);
    }
    
    m_numRooms = numRooms;


        for(int i = 0; i < 18; i++)
        {
            m_grid[i][0] = '#';
            m_grid[i][69] = '#';
            m_objGrid[i][0] = '#';
            m_objGrid[i][69] = '#';
        }
    
        for(int m = 0; m < 70; m++)
        {
            m_grid[0][m] = '#';
            m_grid[17][m] = '#';
            m_objGrid[0][m] = '#';
            m_objGrid[17][m] = '#';
        }
    


    
    // Initializes the player's row and column
    int playerRow = 0;
    int playerCol = 0;
    generateRandomCoord(playerRow, playerCol);
    
    
    if(m_level == 0)
    {
        m_player = new Player(playerRow,playerCol);
    }
    m_player->setRow(playerRow);
    m_player->setCol(playerCol);
    m_grid[playerRow][playerCol] = '@';
    
    // add a staircase:
    int stairRow = 0;
    int stairCol = 0;
    generateRandomCoord(stairRow, stairCol);
    
    if(m_level <= 3)
    {
        m_objGrid[stairRow][stairCol] = '>';
    }
    else if(m_level == 4)
    {
        m_objGrid[stairRow][stairCol] = '&';
    }
    m_levelObjRow = stairRow;
    m_levelObjCol = stairCol;
    
    
    // need to clear monster vector
    vector<Actor*>::iterator ite = monsters.begin();
    for( ; ite != monsters.end(); ite++)
    {
        delete *ite;
    }
    monsters.clear();
    
    // add the proper number of monsters
    int num = randInt(2, 5*(m_level+1)+1);
    for(int q = 0; q <= num; q++)
    {
        addMonster();
    }
    
    
    // need to clear objects vecotr
    vector<GameObject*>::iterator it = objects.begin();
    for( ; it != objects.end(); it++)
    {
        delete *it; // error
    }
    objects.clear();
    
    
    // add the proper number of game objects
    num = randInt(2, 3);
    
    for(int p = 0; p < num; p++)
    {
        int type = randInt(0, 1);
        int objRow = 0;
        int objCol = 0;
        generateRandomCoord(objRow, objCol);
        while(hasObject(objRow, objCol))
        {
            generateRandomCoord(objRow, objCol);
        }
        string desc = "";
        if(type == 0)
        {
            // add a weapon
            int weapType = randInt(0, 2);
            if(weapType == 0)
            {
                desc = "mace";
            }
            else if(weapType == 1)
            {
                desc = "shortsword";
            }
            else if(weapType == 2)
            {
                desc = "longsword";
            }
            Weapon* w = new Weapon(objRow, objCol, desc);
            objects.push_back(w);
            m_objGrid[objRow][objCol] = ')';
            
        }
        else if(type == 1)
        {
            // add a scroll
            int scrType = randInt(0, 3);
            if(scrType == 0)
                desc = "armor";
            else if(scrType == 1)
                desc = "strength";
            else if(scrType == 2)
                desc = "health";
            else if(scrType == 3)
                desc = "dex";
            Scroll* s = new Scroll(objRow, objCol, desc);
            objects.push_back(s);
            m_objGrid[objRow][objCol] = '?';
        }
    }
    
    
    
}

void Dungeon::display()
{
    
    
    if(m_player->hitPoints()>0)
    {
        int pRow = m_player->row();
        int pCol = m_player->col();
        m_grid[pRow][pCol] = '@';
    }
    
    
    // traverse through objects
    // if objects row and col != player row and col add it to m_grid
    // need to display object if object space is not currently occupied by a player
    
    for(int j = 0; j < objects.size(); j++)
    {
        int r = objects[j]->row();
        int c = objects[j]->col();
        if(r == m_player->row() && c == m_player->col())
        {
            ;
        }
        else
        {
            m_grid[r][c] = m_objGrid[r][c];
        }
    }
    
    m_grid[m_levelObjRow][m_levelObjCol] = m_objGrid[m_levelObjRow][m_levelObjCol];
    
    // traverse through monsters
    // display each monster on the grid
    updateGrid();
    
    
    for(int i = 0; i < 18; i++)
    {
        for(int j = 0; j < 70; j++)
        {
            cout << m_grid[i][j];
        }
        cout << endl;
    }
    
    playerStats();
    
}

void Dungeon::addMonster()
{

    int monRow = 0;
    int monCol = 0;
    int randType;
        
    generateRandomCoord(monRow, monCol);
    
    // only specific monsters can appear on certain levels....
    // based on level number, determine the different possible monsters that could appear
    if(m_level < 2)
        randType = randInt(0, 1);
    else if(m_level == 2)
        randType = randInt(0, 2);
    else
        randType = randInt(0,3);
      
    // based on the random choice of monster, dynamically allocate new monster and push it onto the monsters vector
    
    if(randType == 0)
    {
        Snakewoman* s = new Snakewoman(monRow, monCol);
        monsters.push_back(s);
    }
    else if(randType == 1)
    {

        Goblin* g = new Goblin(monRow, monCol, m_gobDistance);
        monsters.push_back(g);
    }
    else if(randType == 2)
    {
        Bogeyman* b = new Bogeyman(monRow, monCol);
        monsters.push_back(b);
    }
    
    else if(randType == 3)
    {
        Dragon* d = new Dragon(monRow, monCol);
        monsters.push_back(d);
    }
    
    updateGrid();
}




bool Dungeon::isWall(int r, int c) const
{
    if(m_grid[r][c] == '#')
        return true;
    else
        return false;
}

// returns true if is wall, monster, player, or object
bool Dungeon::isOccupied(int r, int c) const // is occupied by anything
{
    if(m_grid[r][c] != ' ')
        return true;
    else if(m_objGrid[r][c] != ' ')
        return true;
    else
        return false;
}

bool Dungeon::hasObject(int r, int c) const
{
    for(int i = 0; i < objects.size(); i++)
    {
        if(objects[i]->row() == r && objects[i]->col() == c)
        {
            return true;
            break;
        }
    }
    return false;
}

char Dungeon::charAtPoint(int r, int c) const
{
    if(m_objGrid[r][c] != ' ')
        return m_objGrid[r][c];
    else
        return m_grid[r][c];
}



// returns true if is not occupied by wall, player or monster, but can be occupied by object
bool Dungeon::isOccupiable(int r, int c) const
{
    char ch = m_grid[r][c];
    if(r == m_player->row() && c == m_player->col())
        return false;
    
    for(int i = 0; i < monsters.size(); i++)
    {
        if(monsters[i]->row() == r && monsters[i]->col() == c)
            return false;
    }
    
    if(ch == '#')
        return false;
    
    return true;
}

bool Dungeon::isTravelable(int r , int c) const
{
        char ch = m_grid[r][c];
        for(int i = 0; i < monsters.size(); i++)
        {
            if(monsters[i]->row() == r && monsters[i]->col() == c)
                return false;
        }
        
        if(ch == '#')
            return false;
        
        return true;
}

bool Dungeon::checkOverlap(int sr, int sc, int width, int height)
{
    // checks leftmost column + column to the left of it to see if there is a ' ' character
    for(int i = sr; i < (sr+height); i++)
    {
        if(m_grid[i][sc-1] == ' ' || m_grid[i][sc] == ' ')
            return true;
    }
    
    // checks rightmost column + column to the right of it to see if there is a ' ' character
    for(int j = sr; j < (sr+height); j++)
    {
        if(m_grid[j][sc+width-1] == ' ' || m_grid[j][sc+width] == ' ')
            return true;
    }
    
    // checks topmost row and the row above it to see if there is a ' ' character
    for(int k = sc; k < (sc+width); k++)
    {
        if(m_grid[sr-1][k] == ' ' || m_grid[sr][k] == ' ')
            return true;
    }
    // checks bottommost row and the row below it to see if there is a ' ' character
    for(int m = sc; m < (sc+width); m++)
    {
        if(m_grid[sr+height-1][m] == ' ' || m_grid[sr+height][m] == ' ')
            return true;
    }
    
    // checks outer corners for ' '
    if(m_grid[sr+height][sc-1] == ' ' || m_grid[sr-1][sc-1] == ' ' || m_grid[sr][sc+width] == ' ' || m_grid[sr+height][sc+width] == ' ')
        return true;
    
    
    // if no blank spaces were found in these areas, rectangle is non overlapping
    return false;
}

void Dungeon::generateRandomCoord(int& r, int& c)
{
    // chooses a random room to find a point
    int roomNum = randInt(0, m_numRooms-1);
    int randRow = 0;
    int randCol = 0;
    for(int u = 0; u < m_rooms.size(); u++)
    {
        if(u == roomNum)
        {
            // within random room, picks a random coordinate
            randRow = randInt(m_rooms[u].leftCornerR(), (m_rooms[u].leftCornerR()+m_rooms[u].height()-1));
            randCol = randInt(m_rooms[u].leftCornerC(), (m_rooms[u].leftCornerC()+m_rooms[u].width()-1));
            while(isOccupied(randRow, randCol))
            {
                randRow = randInt(m_rooms[u].leftCornerR(), (m_rooms[u].leftCornerR()+m_rooms[u].height()-1));
                randCol = randInt(m_rooms[u].leftCornerC(), (m_rooms[u].leftCornerC()+m_rooms[u].width()-1));
            }
        }
    }
    r = randRow;
    c = randCol;
}

void Dungeon::playerStats()
{
    cout << "Dungeon Level: " << m_level << ", Hit Points: " << m_player->hitPoints() << ", Armor: " << m_player->armorPoints() << ", Strength: " << m_player->strengthPoints() << ", Dexterity: " << m_player->dexPoints() << endl;
    
        
}

void Dungeon::insert(int r, int c, char ch)
{
    if(ch == ')' || ch == '?')
        m_objGrid[r][c] = ch;
    else
    {
        m_grid[r][c] = ch;
    }
}

Actor* Dungeon::findMonster(int r, int c)
{
    for(int i = 0; i < monsters.size(); i++)
    {
        int mrow = monsters[i]->row();
        int mcol = monsters[i]->col();
        if(mrow == r && mcol == c)
        {
            return monsters[i];
        }
    }
    
    return nullptr;
}

string Dungeon::pickUpObject(int r, int c)
{
    string result = "";
        // if the space the player is on is an object
    if(m_objGrid[r][c] == ')' || m_objGrid[r][c] == '?')
    {
            // find the object that matches row and col within vector
        for(int i = 0; i < objects.size(); i++)
        {
            if(objects[i]->row() == r && objects[i]->col() == c)
            {
                result = m_player->addToInventory(objects[i]);
                objects.erase(objects.begin() + i);
                m_objGrid[r][c] = ' ';
                break;
            }
        }
    }
    return result;
}


void Dungeon::addObject(GameObject *g)
{
    objects.push_back(g);
}

int Dungeon::createRoom(int sr, int sc, int width, int height)
{
    // recursive create rooms
    // generate random dimensions
    // call create room with sr + height + randint within certain range and sc + width and rand int
    // if if sr > 18 or sr+height > 18 return false or similarly sc > 70 or sc+width> 70 false
    // else find vert corridor or horizontal corridor respectively
    
    // base cases:
    if(sr >= 18  || (sr+height) >= 18)
        return 0;
    if(sc >= 70 || (sc+width) >= 70)
        return 0;
    
    if(checkOverlap(sr, sc, width, height))
        return 0;
    
    // picks random coordinates within the room's range to carve out a corridor
    int rightCor = randInt(sr, sr+height-1);
    int downCor = randInt(sc, sc+width-1);
    
    // determine random dimensions for the rooms to be drawn
    int rightWid = randInt(8, 17);
    int rightHeig = randInt(3, 7);
    int rightR;
    if(sr > 3)
        rightR = sr + randInt(-2,3);
    else
        rightR = sr + randInt(1, 3);
    // if the room's dimensions do not accomodate the corridor return 0
    if(rightR > rightCor || rightCor > (rightR+rightHeig) || rightR <= 0)
    {
        return 0;
    }
    int rightC = sc + width + randInt(2, 4);

    
    // generate random dimensions for the room below the current room
    int downWid = randInt(8, 17);
    int downHeig = randInt(4, 7);
    int downR = sr + height + randInt(2, 4);
    int downC;
    if(sc > 3)
        downC = sc + randInt(-2, 7);
    else
        downC = sc + randInt(1,7);
    
    // if the new room's dimensions do not accomodate the corridor then return 0
    if(downC > downCor || downCor > (downC+downWid) || downC <= 0)
    {
        return 0;
    }
    
    
    // if generating a new right and down room was semi successful  create a room with current dimensions and
    //  push it into the vector
    Room r(sr, sc, width, height);
    m_rooms.push_back(r);
    
    // clear out the grid for the dimensions of the room
    for(int i = sr; i < sr+height; i++)
    {
        for(int j = sc; j < sc+width; j++)
        {
            m_grid[i][j] = ' ';
        }
    }

    
    // recursively create rooms for the right room and the down room
    int rightRoom = createRoom(rightR, rightC, rightWid, rightHeig);
    int downRoom = createRoom(downR, downC, downWid, downHeig);
    
    
    // if the right room exists carve out a corridor
    if(rightRoom != 0)
    {
        int curCol = sc+width;
        while(curCol < 70 && m_grid[rightCor][curCol] == '#')
        {
            m_grid[rightCor][curCol] = ' ';
            curCol++;
        }
    }
    
    // if the room below exists carve out a corridor
    if(downRoom != 0)
    {
        int curRow = sr+height;
        while(curRow < 18 && m_grid[curRow][downCor] == '#')
        {
            m_grid[curRow][downCor] = ' ';
            curRow++;
        }
    }
    
    // return the sum of the rooms added with this current room
    return 1 + rightRoom + downRoom;
    
}

string Dungeon::moveMonsters()
{
    string result = "";
    
    for(int i = 0; i < monsters.size(); i++)
    {
        Actor* current = monsters[i];
        int r = current->row();
        int c = current->col();
        
        // check monsters health to determine if it should be removed from the vector and from the grid
        if(current->hitPoints() <= 0)
        {
            current->dropItem(this, r, c);
            // remove monster from vector
            delete current;
            monsters.erase(monsters.begin() + i);
            m_grid[r][c] = ' ';
        }
        else if(current->sleepTime() > 0)
        {
            current->setSleep(current->sleepTime()-1);
        }
        // otherwise attempt to move it
        else
        {
            // determine the direction the monster should travel as determined by their virtual reachable functions
            char dir = current->reachable(this, r, c);
            if(dir != ' ')
            {
                // move in the direction calculated by the reachable function
                // also if monster did something other than just move (i.e. attack) push the resulting string onto a queue
                result = current->move(this, dir, r, c);
                if(result != "")
                    m_monStatus.push(result);
            }
            
        }
        if(m_player->hitPoints() <= 0)
        {
            break;
        }
    }
    
    
    return result;
}

void Dungeon::displayMonsterStatus()
{
    // displays every string in the queue
    while(!m_monStatus.empty())
    {
        cout << m_monStatus.front() << endl;
        m_monStatus.pop();
    }
}

// Updates after a monster moves so m_grid is accurate for other computations
void Dungeon::updateGrid()
{
    if(m_player->hitPoints()>0)
    {
        int pRow = m_player->row();
        int pCol = m_player->col();
        m_grid[pRow][pCol] = '@';
    }
    
    
    // traverse through monsters
    // display each monster on the grid
    for(int k = 0; k < monsters.size(); k++)
    {
        int mrow = monsters[k]->row();
        int mcol = monsters[k]->col();
        string cl = monsters[k]->classification();
        if(cl == "Bogeyman")
            m_grid[mrow][mcol] = 'B';
        else if(cl == "Dragon")
            m_grid[mrow][mcol] = 'D';
        else if(cl == "Goblin")
            m_grid[mrow][mcol] = 'G';
        else if(cl == "Snakewoman")
            m_grid[mrow][mcol] = 'S';
    }
    
}
