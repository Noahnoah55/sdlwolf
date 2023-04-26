#include"map.h"
#include"types.h"
#include<iostream>
#include<fstream>
#include<sstream>

const int defaultMap[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

int Map::getSquare(int x, int y) {
    if (0 <= x && x <= 255) {
        if (0 <= y && y <= 255) {
            return this->walls[x][y];
        }
    }
    return -1;
}

int Map::setSquare(int x, int y, int val)
{
    if (0 <= x && x <= 255) {
        if (0 <= y && y <= 255) {
            walls[x][y] = val;
            return 0;
        }
    }
    return -1;
}

void Map::clearMap() {
    for (int x = 0; x < 256; x++) {
        for (int y = 0; y < 256; y++) {
            this->walls[x][y] = 2;
        }
    }
}

int Map::loadMap(const char* path) {
    this->clearMap();
    std::string line;
    int y = 0;
    std::ifstream mapFile;
    mapFile.open(path);
    if (!mapFile.is_open()) {
        perror("Error opening map file");
        return EXIT_FAILURE;
    }
    while (std::getline(mapFile, line)) {
        std::istringstream linestream(line);
        int x = 0;
        int tile;
        while (linestream >> tile) {
            if (x > 100) {
                std::cout << "X out of range" << std::endl;
                break;
            }
            this->walls[x][y] = tile;
            x++;
        }
        if (y > 100) {
            std::cout << "Y out of range" << std::endl;
            break;
        }
        y++;
    }
    return EXIT_SUCCESS;
}

int Map::addSprite(coord pos, int sprite)
{
    sprites.push_back({pos, sprite});
    return 0;
}

Map::Map() {
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            this->walls[x][y] = defaultMap[x][y];
        }
    }
}

rayHit Map::raycast(coord start, coord raydir) {
    rayHit result;
    float raydirlen = (dist({0,0}, raydir));

    int mapX = (int)start.x;
    int mapY = (int)start.y;
    result.testPoints.push_back((coord){(float)mapX, (float)mapY});
    float sideDistX;
    float sideDistY;
    float deltaDistX = std::abs(raydirlen / raydir.x);
    float deltaDistY = std::abs(raydirlen / raydir.y);
    int stepX;
    int stepY;
    if (raydir.x > 0) {
        stepX = 1;
        sideDistX = ((float)mapX + 1.0 - start.x) * deltaDistX;
    }
    else {
        stepX = -1;
        sideDistX = ((float)start.x - mapX) * deltaDistX;
    }
    if (raydir.y > 0) {
        stepY = 1;
        sideDistY = ((float)mapY + 1.0 - start.y) * deltaDistY;
    }
    else {
        stepY = -1;
        sideDistY = ((float)start.y - mapY) * deltaDistY;
    }
    int hit = 0;
    int side;
    int timer = 0;
    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
            result.testPoints.push_back((coord){(float)mapX, (float)mapY});
        }
        else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
            result.testPoints.push_back((coord){(float)mapX, (float)mapY});
        }
        hit = getSquare(mapX, mapY);
        timer++;
        if (timer > 1000) {
            std::cout << "Failed to raycast";
            hit = -1;
        }
    }
    coord target;
    if (side == 0) {
        target.x = mapX + (1 - stepX) / 2;
        float dx = target.x - start.x;
        float dy = (raydir.y / raydir.x) * dx;
        target.y = start.y + dy;
    }
    else {
        target.y = mapY + (1 - stepY) / 2;
        float dy = target.y - start.y;
        float dx = (raydir.x / raydir.y) * dy;
        target.x = start.x + dx;
    }
    result.collisionPoint = target;
    result.euclidDist = dist(target, start);
    if (side == 0) {
        result.perpDist = ((target.x-start.x) / raydir.x);
        if (stepX == 1) {
            result.normal = cardinal::EAST;
            result.wallX = result.collisionPoint.y - mapY;
        }
        else {
            result.normal = cardinal::WEST;
            result.wallX = 1.0f - (result.collisionPoint.y - mapY);
        }
    }
    else {
        result.perpDist = ((target.y-start.y) / raydir.y);
        if (stepY == 1) {
            result.normal = cardinal::SOUTH;
            result.wallX = 1.0f - (result.collisionPoint.x - mapX);
        }
        else {
            result.normal = cardinal::NORTH;
            result.wallX = result.collisionPoint.x - mapX;
        }
    }
    result.hitType = hit;
    return result;
}

rayHit Map::raycast(coord start, float direction) {
    return raycast(start, fromPolar(1.0f, direction));
}