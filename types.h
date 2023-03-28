#include<vector>
#pragma once

struct coord {
    float x;
    float y;
    struct coord& operator+(const coord& rhs) {x += rhs.x; y += rhs.y; return *this;}
};

struct rayHit {
    float euclidDist;
    float perpDist;
    coord collisionPoint;
    std::vector<coord> testPoints;
    int hitType;
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