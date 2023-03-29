#include"types.h"
#pragma once

class map {
    public:
        int getSquare(int x, int y);
        int loadMap(const char* path);
        map();
    
    private:
        void clearMap();
        int walls[256][256];
};

rayHit raycast(map *m, coord start, coord through);
rayHit raycast(map *m, coord start, float direction);