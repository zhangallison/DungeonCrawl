
#ifndef Dungeon_h
#define Dungeon_h

#include "Room.h"
#include <vector>
#include <string>
#include <queue>
using namespace std;

class Player;
class Monster;
class Actor;
class GameObject;
class Weapon;
class Scroll;

class Dungeon
{
public:
    Dungeon(int level, int gobDist);// probably pass a starting row and col number for the player
    ~Dungeon();
    Dungeon(const Dungeon& other);
    Dungeon& operator=(const Dungeon& other);
    
    // Accessors:
    Player* player() const;
    int level() const;
    
    void createLevel(int newLevel);
    void display();
    void addMonster();
    
    bool isWall(int r, int c) const;
    bool isOccupied(int r, int c) const;
    bool hasObject(int r, int c) const;
    char charAtPoint(int r, int c) const;
    bool isOccupiable(int r, int c) const;
    bool isTravelable(int r, int c) const;
    bool checkOverlap(int sr, int sc, int width, int height);
    void generateRandomCoord(int& r, int& c);
    
    void playerStats();
    
    void insert(int r, int c, char ch);
    Actor* findMonster(int r, int c); // returns pointer to monster at specific point
    string pickUpObject(int r, int c);
    void addObject(GameObject* g);
    int createRoom(int sr, int sc, int width, int height);
    
    string moveMonsters();
    void displayMonsterStatus();
    void updateGrid();
    
private:
    int m_level;
    int m_numRooms;
    char m_grid[18][70];
    char m_objGrid[18][70];
    int m_gobDistance;
    queue<string> m_monStatus;
    int m_levelObjRow;
    int m_levelObjCol;
    Player* m_player;
    vector<Actor*> monsters;
    vector<GameObject*> objects;
    vector<Room> m_rooms;
    
};

#endif /* Dungeon_h */
