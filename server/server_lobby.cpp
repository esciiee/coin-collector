#include "connection.h"
#include "game_state.h"
#include "server.h"
#include "tcp_socket.h"
#include <cstdint>

GameServer::GameServer() {
    state_ = init_game_state();
}
GameServer::~GameServer() {
    running_ = false;
}

bool GameServer::wait_for_players(uint16_t port) {
    TcpSocket listner;

    if (!listner.bind_and_listen(port)) {
        return false;
    }

    for (int i = 0; i < kMPlayers; i++) {
        TcpSocket client_sockect = listner.accept_client();

        if (!client_sockect.valid()) {
            return false;
        }

        clients_[i] = Connection(std::move(client_sockect));

        // state mods
        state_.players[i].connected = true;
    }

    return true;
}

