#pragma once
#include<lua5.4/lua.hpp>

// Functions designed to be called by lua
namespace Engine_API {
    // Graphics
    static int draw_sprite(lua_State *L); // Takes sprite name, position, rendering properties
    static int set_camera(lua_State *L); // Takes position and rotation
    static int draw_ui_sprite(lua_State *L); // Takes sprite name, screen position, scale, rendering properties
    static int draw_ui_text(lua_State *L); // Takes string, screen position, font, etc.

    // Sound
    static int play_sound(lua_State *L); // Takes sound name (callback on end?)
    static int set_music(lua_State *L); // Takes sound name (callback on end?)

    // Map
    static int check_los(lua_State *L); // Takes two positions, returns True if there is no wall blocking them
    static int set_map(lua_State *L); // Takes a position and a tile, edits map data in real time
    static int get_map(lua_State *L); // Takes a position and returns a tile

    // Controls
    static int get_vjoy(lua_State *L); // Returns input state as a virtual joystick w/ buttons
};