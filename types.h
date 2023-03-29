#include<vector>
#pragma once

struct coord {
    float x;
    float y;
    struct coord& operator+(const coord& rhs) {x += rhs.x; y += rhs.y; return *this;}
};

enum cardinal {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

struct rayHit {
    float euclidDist;
    float perpDist;
    coord collisionPoint;
    float wallX;
    std::vector<coord> testPoints;
    int hitType;
    cardinal normal;
};

struct inputState {
    bool forward = false;
    bool backward = false;
    bool turnleft = false;
    bool turnright = false;
    bool strafeleft = false;
    bool straferight = false;
};

coord fromPolar(float dist, float angle_deg);
float dist(coord p1, coord p2);