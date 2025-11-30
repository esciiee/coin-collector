#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <sys/types.h>

class TcpSocket {
public:
    TcpSocket();
    explicit TcpSocket(int fd);
    ~TcpSocket();

    // disables copy
    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;

    // enable move, ensure only one object owns file disc
    TcpSocket(TcpSocket&& other) noexcept;
    TcpSocket& operator=(TcpSocket&& other) noexcept;

    // valid when fd >= -1
    bool valid() const;

    // sends connection req to host, client side
    bool connect(const std::string& host, uint16_t port);
    // binds and listens for connections, server side
    bool bind_and_listen(uint16_t port, int backlog = 2);
    TcpSocket accept_client();

    bool send_all(const uint8_t* data, size_t size);
    bool recv_all(uint8_t* data, size_t size);

    void close();

private:
    int fd_;
};
