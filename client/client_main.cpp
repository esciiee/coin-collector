#include "client.h"
#include <iostream>

int main() {
    GameClient client;

    if (!client.connect(kServerAddress, kPORT)) {
        std::cout << "Failed to connect.\n";
        return 1;
    }

    client.run();
    return 0;
}
