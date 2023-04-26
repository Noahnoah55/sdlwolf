#include"texture.hpp"
#include<stdexcept>
#include<iostream>
#include<SDL2/SDL_image.h>

SDL_Texture *Texture::getFrame(int f)
{
    try {
        return frames.at(f);
    }
    catch (std::out_of_range e) {
        return NULL;
    }
}

/* Loads a texture from the current path and returns the frame number it was assigned to */
int Texture::addFrame(std::string path)
{
    SDL_Texture *frame = IMG_LoadTexture(r, path.c_str());
    if (frame == NULL) {
        return -1;
    }
    else frames.push_back(frame);
    int fw, fh;
    SDL_QueryTexture(frame, NULL, NULL, &fw, &fh);
    dimensions.push_back(std::pair<int,int>(fw, fh));
    return frames.size()-1;
}

int Texture::getFrameWidth(int f)
{
    try {
        return dimensions.at(f).first;
    }
    catch (std::out_of_range) {
        return -1;
    }
}

int Texture::getFrameHeight(int f)
{
    try {
        return dimensions.at(f).second;
    }
    catch (std::out_of_range) {
        return -1;
    }
}

int Texture::draw(SDL_Rect *src, SDL_Rect *dst, int frame)
{
    return SDL_RenderCopy(r, getFrame(frame), src, dst);
}

int Texture::draw(SDL_Rect *src, SDL_FRect *dst, int frame)
{
    int fw, fh;
    SDL_QueryTexture(getFrame(frame), NULL, NULL, &fw, &fh);
    return SDL_RenderCopyF(r, getFrame(frame), src, dst);
}

Texture::Texture()
{
    r = NULL;
}

Texture::Texture(SDL_Renderer *renderer) // Fun fact all textures are bound to a renderer!
{
    r = renderer;
}

Texture::Texture(SDL_Renderer *renderer, std::string path)
{
    r = renderer;
    addFrame(path);
}

void Texture::clear()
{
    for (auto tex : frames) {
        SDL_DestroyTexture(tex);
    }
}

TextureManager::TextureManager()
{
}

Texture& TextureManager::getTexture(std::string key)
{
    return textures[key];
}

void TextureManager::setTexture(std::string key, Texture tex)
{
    textures.emplace(key, tex);
}
