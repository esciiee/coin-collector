# Coin Collector

This project implements a minimal real-time multiplayer Coin Collector game with:

- Authoritative Server
- Client that used SDL2 for rendering and input
- Raw TCP Networking
- Introduces tests in adverse network condtions

The video demo of the same can be found [here](https://youtu.be/bPqbqADVev0)

## Project Structure

The project is divided in 4 sub-directories each serving different purpose

- ./core/   - Game Rules, Contains the true state of the game
- ./net/    - Raw TCP sockets and serializers
- ./server  - Server lobby, contains update loop and authoritative logic
- ./client  - SDL2 renderer for input and display

## Dependencies

- The project uses C++20, hence GCC 11+, clang 12+ is recommended
- SDL2 for client side
    - Ubuntu Debian
        ```bash
        sudo apt install libsdl2-dev
        ```
    - macOS
        ```bash
        brew install sdl2
        ```

    - For other OS, follow detailed instructions at sdl [install page](https://wiki.libsdl.org/SDL2/Installation)
## Build
The game can be built and runned using the following instructions

```bash
mkdir build
cd build
cmake ..
make -j
```

### Running the Server

```bash
./bin/server
```

### Running the Client
```bash
./bin/client
```

Note that the game can be runnned using self instroduced latency to test the game under adverse network conditions, ex: a 200ms latency can be introduced as follows
```bash
./bin/server --latency=200
./bin/client --latency=200
```


