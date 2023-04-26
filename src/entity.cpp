#include"entity.hpp"
#include<iostream>

using std::cout, std::endl;

int EntityManager::init_lua()
{
    L = luaL_newstate();
    if (L == NULL) {
        cout << "Failed to initialize Lua state" << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
