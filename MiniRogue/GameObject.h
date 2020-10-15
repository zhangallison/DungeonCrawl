

#ifndef GameObjects_hpp
#define GameObjects_hpp

#include <string>
using namespace std;

class GameObject
{
public:
    GameObject(int r, int c, string type);
    virtual ~GameObject();

    
    virtual string description();
    virtual string classification();
    
    // accessors:
    int row() const;
    int col() const;
    string type() const;
    
private:
    int m_row;
    int m_col;
    string m_type;
};

#endif /* GameObjects_hpp */
