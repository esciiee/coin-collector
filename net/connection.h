#pragma once

#include "tcp_socket.h"
#include "protocol.h"
#include <vector>
#include <cstdint>

class Connection {
public:
    Connection() = default;
    explicit Connection(TcpSocket sock);

    bool send_frame(MsgType type, const std::vector<uint8_t>& payload);
    bool recv_frame(MsgType& out_type, std::vector<uint8_t>& out_payload);

    bool valid() const { return sock_.valid(); }
    void close() { sock_.close(); }

private:
    TcpSocket sock_;
};
