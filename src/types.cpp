#include"types.h"
#include<cmath>

coord fromPolar(float dist, float angle_deg) {
    coord result;
    result.x = dist * cos(angle_deg * (M_PI/180.0f));
    result.y = dist * sin(angle_deg * (M_PI/180.0f));
    return result;
}

float dist(coord p1, coord p2) {
    coord delta {p1.x - p2.x, p1.y - p2.y};
    return sqrt(delta.x * delta.x + delta.y * delta.y);
}