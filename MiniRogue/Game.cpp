// Game.cpp

#include "Game.h"
#include "utilities.h"
#include "Dungeon.h"
#include "Player.h"
#include "Scroll.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
{
    // initialize the dungeon for the game, player status message and the boolean that indicates whther the player has won
    m_dungeon = new Dungeon(0, goblinSmellDistance);
    m_status = "";
    m_playerWon = false;
}

Game::~Game()
{
    delete m_dungeon;
}

void Game::play()
{
    char c;
    m_dungeon->display();
    // Get user's inputted character and use accordingly:
    while ((c = getCharacter()) != 'q')
    {
        // check that the player is not dead:
        if(m_dungeon->player()->hitPoints()>0)
        {
            clearScreen();
            takePlayerTurn(c);
            // if the player has not won yet, move all of the monsters in the dungeon, and display the dungeon when done
            if(!m_playerWon)
            {
                m_dungeon->moveMonsters();
                m_dungeon->display();
                cout << endl;
                
                // write status messages:
                if(m_status != "")
                {
                    cout << m_status << endl;
                }
                m_dungeon->displayMonsterStatus();
                if(m_dungeon->player()->hitPoints() < 0)
                    cout << "Press q to quit game. " << endl;
            }
            else
            {
                m_dungeon->display();
                cout << m_status << endl;
                cout << "Press q to quit game. " << endl;
            }
        }
        else
        {
            cout << "Press q to exit game." << endl;
        }

            
    }
    
}

void Game::takePlayerTurn(char com)
{
    // store any status messages into this string variable:
    string result = "";
    
    Player* player = m_dungeon->player();

        
    int r = player->row();
    int c = player->col();
    
    // check player sleep time, subtract one for every turn that player is "asleep"
    if(player->sleepTime() > 0)
    {
        player->setSleep(player->sleepTime() - 1);
        m_status = "";
        return;
    }
        

    // if the command is to pick up an object
    if(com == 'g')
    {
        // check if the object being picked up is the golden idol
        if(m_dungeon->charAtPoint(r, c) == '&')
        {
            m_playerWon = true;
            result = "You pick up the golden idol\nCongratulations, you won!";
        }
        // otherwise call the dungeon's pick up object function
        else
            result = m_dungeon->pickUpObject(r, c);
    }
        
    // command to display inventory:
    else if(com == 'i')
    {
        player->displayInventory();
        getCharacter();
        clearScreen();
    }
    
    // command to read a scroll, call player's read scroll function based off of the letter inputted from the user
    // (read scroll function will handle if the chosen item is not actually a scroll)
    else if(com == 'r')
    {
        player->displayInventory();
        char c = getCharacter();
        result = player->readScroll(c, m_dungeon);
        clearScreen();
    }
    
    // command to wield a weapon, call player's wield weapon function based off of the letter inputted from the user
    // (wield weapon function will handle if the chosen item is not actually a weapon)
    else if(com == 'w')
    {
        player->displayInventory();
        char c = getCharacter();
        result = player->wieldWeapon(c);
        clearScreen();
    }
    
    // if user types command to cheat, set stats accordingly
    else if(com == 'c')
    {
        player->setStrength(9);
        player->setMaxHP(50);
        player->setHP(50);
    }
    
    
    // if user types the > character, need to check if it is indeed a staircase and then create a new dungeon level
    else if(com == '>')
    {
        if(m_dungeon->charAtPoint(r, c) == '>')
        {
            m_dungeon->createLevel(m_dungeon->level()+1);
        }
    }
    
    // if the player's command is none of the above, move the player in the direction inputted
    // (if it is not a valid direction the player's move function will handle it)
    else
    {
        result = player->move(m_dungeon, com, r, c);
    }
    
    // call this function that randomly change whether player's max hp should increase at the end of every player turn
    player->randomIncreaseHP();
    
    m_status = result;
    
}

// function helpful in determining if a GameObject is a Scroll using dynamic cast
bool isScroll(GameObject *g)
{
    Scroll* s = dynamic_cast<Scroll*>(g);
    if(s != nullptr)
    {
        return true;
    }
    else
        return false;
}


// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
