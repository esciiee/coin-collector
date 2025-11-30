#include "connection.h"
#include "serializer.h"
#include <vector>

Connection::Connection(TcpSocket sock) : sock_(std::move(sock)) {}

bool Connection::send_frame(MsgType type, const std::vector<uint8_t>& payload) {
    bytes frame = frame_message(type, payload);
    return sock_.send_all(frame.data(), frame.size());
}

bool Connection::recv_frame(MsgType& out_type, std::vector<uint8_t>& out_payload) {
    // decode header
    uint8_t header[5];
    if (!sock_.recv_all(header, 5))
        return false;

    uint32_t len;
    if (!parse_frame_header(header, 5, len, out_type))
        return false;

    // original len contains payload size + 1 byte for msg type
    out_payload.resize(len - 1);
    return sock_.recv_all(out_payload.data(), out_payload.size());
}
