#ifndef _MAP_H_
#define _MAP_H_


#include "Direction.h"
#include "Common.h"

class Map
{
public:
    Map();
    bool LoadFromMemory(const map_buffer_desc& mbd);
    bool LoadFromFile(const char * path);


    bool MoveTo(int x, int y);
    bool Wall(int side) const;

    void setDirection(const direction& d)
    {
       dir = d;
    }

    int Width() const
    {
       return width;
    }

    int Height() const
    {
       return  height;
    }
    ~Map();
private:
    int ** cells;
    int width;
    int height;
    bool walls[4];
    direction dir;
};

#endif
