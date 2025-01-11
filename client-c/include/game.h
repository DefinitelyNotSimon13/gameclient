#ifndef GAME_H
#define GAME_H

#include "movement.h"
#include "network.h"
#include "packet.h"

typedef PacketV0 OtherPlayer;

typedef struct {
    int active_players;
    OtherPlayer players[];
} GameState;

void init_game(int screen_width, int screen_height);

void run_game_loop(Player *player, Camera *camera, connection_t *conn);

#endif // GAME_H
