#include"types.h"
#pragma once

struct player {
    coord pos;
    float direction = 0;
    int health = 100;
};

const float PLAYER_SPEED = 5;
const float PLAYER_TURNSPEED = 180;
const float PLAYER_WIDTH = .5;

extern player Player;
void movePlayer(float dt, inputState is);