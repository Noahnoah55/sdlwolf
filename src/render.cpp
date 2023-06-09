#include"render.h"
#include"types.h"
#include"player.h"
#include"map.h"
#include"texture.hpp"

#include<iostream>
#include<string>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<map>
#include<algorithm>

#include"includes/imgui/imgui.h"
#include"includes/imgui/backends/imgui_impl_sdl2.h"
#include"includes/imgui/backends/imgui_impl_sdlrenderer.h"

TextureManager textureManager;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;

TTF_Font* font = NULL;

// Private drawing methods
void draw3D(Map &m);
void drawText();

float DRAW_SCALE = 10;

int loadMedia() {
    int returnCode = EXIT_SUCCESS;

    font = TTF_OpenFont("fonts/Castle.ttf", 30);
    if (font == NULL) {
        std::cout << "Failed to load font Castle.ttf! Error: " << TTF_GetError() << std::endl;
        returnCode = EXIT_FAILURE;
    }

    // TODO: Automate loading textures based on input file
    Texture walls(renderer);
    walls.addFrame("textures/eagle.png");
    walls.addFrame("textures/redbrick.png");
    walls.addFrame("textures/purplestone.png");
    walls.addFrame("textures/greystone.png");
    walls.addFrame("textures/bluestone.png");
    walls.addFrame("textures/mossy.png");
    walls.addFrame("textures/wood.png");
    walls.addFrame("textures/colorstone.png");

    Texture sprites(renderer);
    sprites.addFrame("textures/barrel.png");

    textureManager.setTexture("walls", walls);
    textureManager.setTexture("sprites", sprites);

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

    // Set up IMGUI
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    return EXIT_SUCCESS;
}

// Closes the rendering window
void closeWindow() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
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

void drawFrame(Map &m) {
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    ImGui::Begin("Map");

    for (auto spr : m.sprites) {
        ImGui::Text("%f, %f", spr.first.x, spr.first.y);
    }


    ImGui::End();

    draw3D(m);

    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}
bool cmpSpr(const std::pair<float, std::pair<coord, int>> a, const std::pair<float, std::pair<coord, int>> b) {
    return a.first > b.first;
}

void draw3D(Map &m) {
    float zBuffer[SCREEN_WIDTH];
    Texture wallTex = textureManager.getTexture("walls");
    Texture spriteTex = textureManager.getTexture("sprites");
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float camX = 1.5f*((float)x/(float)SCREEN_WIDTH)-.75;
        coord raydir = fromPolar(1, Player.direction);
        raydir = raydir + fromPolar(camX, Player.direction+90);
        rayHit r = m.raycast(Player.pos, raydir);

        int texW = wallTex.getFrameWidth();
        int texH = wallTex.getFrameHeight();

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

        zBuffer[x] = r.perpDist;

        // Draw ceiling and floor
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT/2);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderDrawLine(renderer, x, SCREEN_HEIGHT/2, x, SCREEN_HEIGHT);

        // Draw wall texture
        wallTex.draw(&wallSrc, &wallDest, r.hitType-1);
    }
    std::vector<std::pair<float, std::pair<coord, int>>> sprites;
    for (unsigned int x = 0; x < m.sprites.size(); x++) {
        auto s = m.sprites.at(x);
        sprites.push_back({dist(Player.pos, s.first), s});
    }
    std::sort(sprites.begin(), sprites.end(), cmpSpr); // Make sure this is furthest -> nearest
    coord dir = fromPolar(1.0f, Player.direction);
    coord plane = fromPolar(.75f, Player.direction + 90);
    float det = (plane.x * dir.y) - (dir.x * plane.y);
    det = 1.0f / det;
    float camMa[2][2] = {
        {dir.y*det, -dir.x*det},
        {-plane.y*det, plane.x*det}
    };
    for (auto spr : sprites) {
        coord relPos = Player.pos - spr.second.first;
        coord camPos; // X is screen space, Y is depth (compare Y with Z buffer)
        camPos.x = relPos.x*camMa[0][0] + relPos.y*camMa[0][1];
        camPos.y = relPos.x*camMa[1][0] + relPos.y*camMa[1][1];
        if (camPos.y < -.1) {
            int screenSpriteX = (SCREEN_WIDTH / 2) * (1 + camPos.x / camPos.y);
            int screenSpriteW = abs(SCREEN_HEIGHT / camPos.y);
            int drawStartX = screenSpriteX - (screenSpriteW/2);
            int drawEndX = screenSpriteX + (screenSpriteW/2);
            int spw = spriteTex.getFrameWidth(spr.second.second);
            int sph = spriteTex.getFrameHeight(spr.second.second);
            SDL_FRect destStrip;
            destStrip.h = abs(SCREEN_HEIGHT / camPos.y);
            destStrip.y = -destStrip.h/2 + SCREEN_HEIGHT/2;
            destStrip.w = 1;
            SDL_Rect sourceStrip;
            sourceStrip.h = sph;
            sourceStrip.y = 0;
            sourceStrip.w = 1;
            sourceStrip.x = 0;
            for (int i = std::min(drawStartX, 0); i < std::max(drawEndX, SCREEN_WIDTH); i++) {
                destStrip.x = i;
                sourceStrip.x = (i-drawStartX)*spw / screenSpriteW;
                if (-camPos.y < zBuffer[i]) {
                    spriteTex.draw(&sourceStrip, &destStrip, spr.second.second);
                }
            }
            ImGui::Begin("Spr");
            ImGui::Text("%f, %f, %d", camPos.y, zBuffer[SCREEN_WIDTH/2], screenSpriteW);
            ImGui::End();
        }
    }
}
