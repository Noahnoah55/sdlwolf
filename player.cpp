#include"player.h"

player Player = {{5,5}, 90, 100};

void movePlayer(float dt, inputState is) {
    Player.pos.x += (is.straferight - is.strafeleft) * PLAYER_SPEED * dt;
    Player.pos.y += (is.backward - is.forward) * PLAYER_SPEED * dt;
    Player.direction += (is.turnright - is.turnleft) * PLAYER_TURNSPEED * dt;
}