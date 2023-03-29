#pragma once
#include<SDL2/SDL.h>
#include"types.h"
#include"map.h"

int initialize();
int loadMedia();
void drawFrame(map *m);
void closeWindow();

enum justify {
    left,
    right,
    center
};

extern float DRAW_SCALE;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;