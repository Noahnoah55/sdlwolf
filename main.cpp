#include<SDL2/SDL.h>
#include<iostream>
#include<cstdlib>

#include"types.h"
#include"render.h"
#include"player.h"
#include"map.h"

inputState InputState;


int main () {
    // Init SDL
    if (initialize() != 0) {
        return EXIT_FAILURE;
    }
    // Load textures, sounds, etc
    if (loadMedia() != 0) {
        return EXIT_FAILURE;
    }
    map CURR_MAP;
    if (CURR_MAP.loadMap("maps/sample.map")) {
        std::cout << "Failed to load test map" << std::endl;
        return EXIT_FAILURE;
    }

    // Main Loop
    SDL_Event e;
    bool quit = false;
    Uint32 now = 0;
    Uint32 last = 0;
    while (!quit) {
        last = now;
        now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;

        // Accept Input
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case (SDL_QUIT):
                    quit = true;
                    break;
                case (SDL_KEYDOWN):
                case (SDL_KEYUP):
                    switch (e.key.keysym.scancode) {
                        case (SDL_SCANCODE_W):
                            InputState.forward = (e.key.state == SDL_PRESSED);
                            break;
                        case (SDL_SCANCODE_S):
                            InputState.backward = (e.key.state == SDL_PRESSED);
                            break;
                        case (SDL_SCANCODE_A):
                            InputState.strafeleft = (e.key.state == SDL_PRESSED);
                            break;
                        case (SDL_SCANCODE_D):
                            InputState.straferight = (e.key.state == SDL_PRESSED);
                            break;
                        case (SDL_SCANCODE_Q):
                            InputState.turnleft = (e.key.state == SDL_PRESSED);
                            break;
                        case (SDL_SCANCODE_E):
                            InputState.turnright = (e.key.state == SDL_PRESSED);
                            break;
                        case (SDL_SCANCODE_UP):
                            if (e.key.state == SDL_PRESSED) DRAW_SCALE *= 2;
                            break;
                        case (SDL_SCANCODE_DOWN):
                            if (e.key.state == SDL_PRESSED) DRAW_SCALE *= .5;
                            break;
                        case (SDL_SCANCODE_SPACE):
                            if (e.key.state == SDL_PRESSED) {
                                Player.direction = 1;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        
        // Game Logic
        movePlayer(dt, InputState);

        // Render
        drawFrame(&CURR_MAP);

        SDL_Delay(10);
    }

    closeWindow();
    return EXIT_SUCCESS;
}
