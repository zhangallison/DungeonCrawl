// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.

#include "Dungeon.h"
#include <string>
using namespace std;

class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game();
    void play();
    void takePlayerTurn(char com);
    
private:
    Dungeon* m_dungeon;
    string m_status;
    bool m_playerWon;
};

bool isScroll(GameObject* g);

#endif // GAME_INCLUDED
