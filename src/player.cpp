#include"player.h"

player Player = {{5,5}, 91, 100};

void movePlayer(float dt, inputState is) {
    coord mov;
    mov.x = (is.straferight - is.strafeleft) * PLAYER_SPEED * dt;
    mov.y = (is.backward - is.forward) * PLAYER_SPEED * dt;
    coord dir;
    dir = fromPolar(-mov.y, Player.direction);
    dir = dir + fromPolar(mov.x, Player.direction+90);
    Player.pos = Player.pos + dir;
    Player.direction += (is.turnright - is.turnleft) * PLAYER_TURNSPEED * dt;
}