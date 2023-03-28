#include"render.h"
#include"types.h"
#include"player.h"
#include"map.h"
#include<iostream>

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;

// Private drawing methods
void drawMap();
void draw3D();

float DRAW_SCALE = 10;

// Starts up the rendering window
int initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! Error: " << SDL_GetError();
        return EXIT_FAILURE;
    }

    // Create Window
    int result = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
    if (result != 0) {
        std::cout << "Window or Renderer could not be created! Error: " << SDL_GetError();
        return EXIT_FAILURE;
    }
    SDL_SetWindowTitle(window, "Video Game");
    return EXIT_SUCCESS;
}

// Closes the rendering window
void closeWindow() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawFrame() {
    // Draw Background
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    draw3D();
    drawMap();
    SDL_RenderPresent(renderer);
}

void drawMap() {
    // Draw Map
    SDL_Rect mapSquare = {0, 0, (int)DRAW_SCALE, (int)DRAW_SCALE};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            if (MAP[x][y]) {
                mapSquare.x = x * DRAW_SCALE;
                mapSquare.y = y * DRAW_SCALE;
                SDL_RenderFillRect(renderer, &mapSquare);
            }
        }
    }

    // Draw Player Icon
    coord pvisual = Player.pos;
    pvisual.x *= DRAW_SCALE;
    pvisual.y *= DRAW_SCALE;
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_Rect pRect = {(int)(pvisual.x - (PLAYER_WIDTH * DRAW_SCALE * .5)),
        (int)(pvisual.y - (PLAYER_WIDTH * DRAW_SCALE * .5)),
        (int)(DRAW_SCALE * PLAYER_WIDTH),
        (int)(DRAW_SCALE * PLAYER_WIDTH)};
    SDL_RenderFillRect(renderer, &pRect);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    coord facing = fromPolar(DRAW_SCALE * PLAYER_WIDTH * 3, Player.direction);
    facing.x += pvisual.x;
    facing.y += pvisual.y;
    SDL_RenderDrawLine(renderer,
        pvisual.x,
        pvisual.y,
        facing.x,
        facing.y);

    // Draw raycasting debug info
    #define DEBUG_RAYCAST
    #ifdef DEBUG_RAYCAST
    rayHit testRay = raycast(Player.pos, Player.direction);
    coord looking = testRay.collisionPoint;
    SDL_Rect rayRect = {(int)(looking.x * DRAW_SCALE) - 5,
        (int)(looking.y * DRAW_SCALE) - 5,
        10, 10};
    SDL_RenderDrawRect(renderer, &rayRect);
    for (coord c : testRay.testPoints) {
        rayRect.x = c.x * DRAW_SCALE;
        rayRect.y = c.y * DRAW_SCALE;
        rayRect.w = DRAW_SCALE;
        rayRect.h = DRAW_SCALE;
        SDL_RenderDrawRect(renderer, &rayRect);
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer,
        pvisual.x,
        pvisual.y,
        testRay.collisionPoint.x * DRAW_SCALE,
        testRay.collisionPoint.y * DRAW_SCALE);
    #endif
}


void draw3D() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float camX = 1.5f*((float)x/(float)SCREEN_WIDTH)-.75;
        coord raydir = fromPolar(1.5, Player.direction);
        raydir = raydir + fromPolar(camX, Player.direction+90);
        rayHit r = raycast(Player.pos, raydir);

        int height = (int)(SCREEN_HEIGHT / r.perpDist);
        int top = -height / 2 + SCREEN_HEIGHT / 2;
        if (top < 0) top = 0;
        int bottom = height / 2 + SCREEN_HEIGHT / 2;
        if (bottom > SCREEN_HEIGHT) bottom = SCREEN_HEIGHT;

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, x, 0, x, top);
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, x, top, x, bottom);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderDrawLine(renderer, x, bottom, x, SCREEN_HEIGHT);
    }
}