#include"render.h"
#include"types.h"
#include"player.h"
#include"map.h"

#include<iostream>
#include<string>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<map>

std::map<int, SDL_Texture*> textures;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;

TTF_Font* font = NULL;

// Private drawing methods
void drawMap(map *m);
void draw3D(map *m);
void drawText();

float DRAW_SCALE = 10;

int loadMedia() {
    int returnCode = EXIT_SUCCESS;

    font = TTF_OpenFont("fonts/Castle.ttf", 30);
    if (font == NULL) {
        std::cout << "Failed to load font Castle.ttf! Error: " << TTF_GetError() << std::endl;
        returnCode = EXIT_FAILURE;
    }

    textures[0] = IMG_LoadTexture(renderer, "textures/eagle.png");
    textures[1] = IMG_LoadTexture(renderer, "textures/redbrick.png");
    textures[2] = IMG_LoadTexture(renderer, "textures/purplestone.png");
    textures[3] = IMG_LoadTexture(renderer, "textures/greystone.png");
    textures[4] = IMG_LoadTexture(renderer, "textures/bluestone.png");
    textures[5] = IMG_LoadTexture(renderer, "textures/mossy.png");
    textures[6] = IMG_LoadTexture(renderer, "textures/wood.png");
    textures[7] = IMG_LoadTexture(renderer, "textures/colorstone.png");
    return returnCode;
}

// Starts up the rendering window
int initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    
    if (TTF_Init() < 0) {
        std::cout << "TTF could not initialize! Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! Error: " << SDL_GetError() << std::endl;
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
    for (auto texPair : textures) {
        SDL_DestroyTexture(texPair.second);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawText(int x, int y, justify j, std::string text) {
    SDL_Color text_color = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), text_color);
    if (textSurface == NULL) {
        std::cout << "Failed to create text surface! Error: " << SDL_GetError() << std::endl;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect srcRect = {0,0,textSurface->w, textSurface->h};
    SDL_Rect destRect;
    switch (j) {
        case (left):
             destRect = {x,y,textSurface->w, textSurface->h};
            break;
        case (right):
             destRect = {x-textSurface->w,y,textSurface->w, textSurface->h};
            break;
        case (center):
             destRect = {x-(textSurface->w/2),y,textSurface->w, textSurface->h};
            break;
    }
    if (SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect)) {
        std::cout << "Failed to render text! Error: " << SDL_GetError() << std::endl;
    }
    SDL_DestroyTexture(textTexture);
}

void drawFrame(map *m) {
    // Draw Background
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    draw3D(m);
    drawMap(m);
    SDL_RenderPresent(renderer);
}

void drawMap(map *m) {
    int mapX = (int)Player.pos.x;
    int mapY = (int)Player.pos.y;
    // Draw Map
    SDL_Rect mapSquare = {0, 0, (int)DRAW_SCALE, (int)DRAW_SCALE};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            if (m->getSquare(x+mapX-5,y+mapY-5)) {
                mapSquare.x = x * DRAW_SCALE;
                mapSquare.y = y * DRAW_SCALE;
                SDL_RenderFillRect(renderer, &mapSquare);
            }
        }
    }

    // Draw Player Icon
    coord pvisual = Player.pos;
    pvisual.x -= mapX-5;
    pvisual.y -= mapY-5;
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
    rayHit testRay = raycast(m, Player.pos, Player.direction);
    coord looking = testRay.collisionPoint;
    SDL_Rect rayRect = {(int)((looking.x-mapX+5) * DRAW_SCALE) - 5,
        (int)((looking.y-mapY+5) * DRAW_SCALE) - 5,
        10, 10};
    SDL_RenderDrawRect(renderer, &rayRect);
    for (coord c : testRay.testPoints) {
        rayRect.x = (c.x-mapX+5) * DRAW_SCALE;
        rayRect.y = (c.y-mapY+5) * DRAW_SCALE;
        rayRect.w = DRAW_SCALE;
        rayRect.h = DRAW_SCALE;
        SDL_RenderDrawRect(renderer, &rayRect);
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer,
        pvisual.x,
        pvisual.y,
        (testRay.collisionPoint.x-mapX+5) * DRAW_SCALE,
        (testRay.collisionPoint.y-mapY+5) * DRAW_SCALE);
    #endif
}


void draw3D(map *m) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float camX = 1.5f*((float)x/(float)SCREEN_WIDTH)-.75;
        coord raydir = fromPolar(1.5, Player.direction);
        raydir = raydir + fromPolar(camX, Player.direction+90);
        rayHit r = raycast(m, Player.pos, raydir);

        // Pick out the right texture
        SDL_Texture *wallTex = textures[r.hitType-1];
        int texW, texH;
        SDL_QueryTexture(wallTex, NULL, NULL, &texW, &texH);

        // wallSrc is the strip of the texture to be drawn here
        SDL_Rect wallSrc;
        // wallDest is the section of the screen to draw the texture to
        SDL_FRect wallDest;

        wallSrc.x = r.wallX * texW;
        wallSrc.y = 0;
        wallSrc.h = texH;
        wallSrc.w = 1;

        wallDest.x = x;
        wallDest.h = ((float)SCREEN_HEIGHT / r.perpDist);
        wallDest.y = (float)(SCREEN_HEIGHT - wallDest.h) / 2.0f;
        wallDest.w = 1;

        // Draw ceiling and floor
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT/2);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderDrawLine(renderer, x, SCREEN_HEIGHT/2, x, SCREEN_HEIGHT);

        // Draw wall texture
        SDL_RenderCopyF(renderer, wallTex, &wallSrc, &wallDest);
    }
}