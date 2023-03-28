#pragma once
#include<SDL2/SDL.h>
#include"types.h"

int initialize();
void drawFrame();
void closeWindow();

extern float DRAW_SCALE;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;