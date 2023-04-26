#pragma once
#include<SDL2/SDL.h>
#include<vector>
#include<string>
#include<map>

class Texture {
    public:
        SDL_Texture* getFrame(int f);
        int addFrame(std::string path);
        int getFrameWidth(int f = 0);
        int getFrameHeight(int f = 0);
        int draw(SDL_Rect *src, SDL_Rect *dst, int frame = 0);
        int draw(SDL_Rect *src, SDL_FRect *dst, int frame = 0);
        Texture();
        Texture(SDL_Renderer *renderer);
        Texture(SDL_Renderer *renderer, std::string path);
        void clear();
    private:
        SDL_Renderer *r;
        std::vector<SDL_Texture *> frames;
        std::vector<std::pair<int, int>> dimensions;
};

// Texturemanager handles loading textures and allows them to be accessed by string tokens
class TextureManager {
    public:
        TextureManager();
        Texture& getTexture(std::string key);
        void setTexture(std::string key, Texture tex);
    private:
        std::map<std::string, Texture> textures;
};