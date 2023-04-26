#pragma once
#include<lua5.4/lua.hpp> // Lua 5.4
#include<string>
#include<vector>
#include<unordered_set>
#include"types.h"

#include<SDL2/SDL.h>

struct entity {
    // Information about the type of entity
    std::vector<SDL_Texture *> textures;
    std::string env;

    // Information about entity state
    int textureState;
    coord pos;
    float dir;
};

namespace EntityManager {
    lua_State *L;
    int init_lua();
    int add_entity();
    std::unordered_set<std::string> envs;

    // Helpers for lua functions
    int get_entity_pos(int entity_ID);
    int set_entity_pos(int entity_ID, float x, float y);
};
