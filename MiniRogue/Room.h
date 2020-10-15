
#ifndef Room_hpp
#define Room_hpp

#include <stdio.h>

class Room
{
public:
    Room(int sr, int sc, int width, int height);

    int leftCornerR();
    int leftCornerC();
    int width();
    int height();

private:
    int m_sr;
    int m_sc;
    int m_width;
    int m_height;


};

#endif /* Room_hpp */
