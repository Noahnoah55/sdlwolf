#include"types.h"
#pragma once

struct player {
    coord pos;
    float direction = 1;
    int health = 100;
};

const float PLAYER_SPEED = 2;
const float PLAYER_TURNSPEED = 90;
const float PLAYER_WIDTH = .5;

extern player Player;
void movePlayer(float dt, inputState is);