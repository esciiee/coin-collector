#include "server.h"

int main() {
    GameServer game_server;
    if (!game_server.wait_for_players(kPORT)) {
        return 1;
    }

    game_server.run_loop();
    return 0;
}
