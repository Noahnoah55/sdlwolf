#include"map.h"
#include"types.h"
#include<iostream>

const int MAP[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

int getSquare(int x, int y) {
    if (0 <= x && x <= 9) {
        if (0 <= y && y <= 9) {
            return MAP[x][y];
        }
    }
    return 2;
}
rayHit raycast(coord start, coord raydir) {
    rayHit result;

    int mapX = (int)start.x;
    int mapY = (int)start.y;
    result.testPoints.push_back((coord){(float)mapX, (float)mapY});
    float sideDistX;
    float sideDistY;
    float deltaDistX = std::abs((dist({0,0}, raydir) / raydir.x));
    float deltaDistY = std::abs(dist({0,0}, raydir) / raydir.y);
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
        if (timer > 200) {
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
    if (side == 0) result.perpDist = (sideDistX - deltaDistX);
    else result.perpDist = (sideDistY - deltaDistY);
    result.hitType = hit;
    return result;

}

rayHit raycast(coord start, float direction) {
    return raycast(start, fromPolar(1.0f, direction));
}