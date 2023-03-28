#include"types.h"
#pragma once

extern const int MAP[10][10];

rayHit raycast(coord start, float direction);
int getSquare(int x, int y);