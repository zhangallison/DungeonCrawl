
#include "Room.h"
#include "utilities.h"
#include <cstdlib>
using namespace std;

Room::Room(int sr, int sc, int width, int height)
{
    m_sr = sr;
    m_sc = sc;
    m_width = width;
    m_height = height;
}


int Room::leftCornerR()
{
    return m_sr;
}

int Room::leftCornerC()
{
    return m_sc;
}

int Room::width()
{
    return m_width;
}

int Room::height()
{
    return m_height;
}
