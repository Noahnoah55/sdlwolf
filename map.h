#pragma once
#include"types.h"
#include<vector>

class Map {
    public:
        int getSquare(int x, int y);
        int setSquare(int x, int y, int val);
        int loadMap(const char* path);
        int addSprite(coord pos, int sprite);
        rayHit raycast(coord start, coord through);
        rayHit raycast(coord start, float direction);
        Map();
        std::vector<std::pair<coord, int>> sprites;
    
    private:
        void clearMap();
        int walls[256][256];
};