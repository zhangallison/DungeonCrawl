
#include "GameObject.h"
using namespace std;

GameObject::GameObject(int r, int c, string type)
{
    m_row = r;
    m_col = c;
    m_type = type;
}

GameObject::~GameObject()
{
    
}

string GameObject::description()
{
    return "";
}

int GameObject::row() const
{
    return m_row;
}

int GameObject::col() const
{
    return m_col;
}

string GameObject::type() const
{
    return m_type;
}

string GameObject::classification()
{
    return "";
}
