#include "connection.h"
#include "latency.h"
#include "serializer.h"
#include <vector>

Connection::Connection(TcpSocket sock) : sock_(std::move(sock)) {}

bool Connection::send_frame(MsgType type, const std::vector<uint8_t>& payload) {
    // the round trip delay is more than intended
    // when both send and recv are delayed,
    // for every tick we send input(200ms) + recv_input(200ms) + send_snapshot(200ms) + recv_snapshot(200ms)
    // adding upto 800ms update time per tick + the main simulation loops
    // is blocked for 200ms for sending snapshot to 1 client, so + 200*2ms delay here
    // so it is not just a 200ms latency
    // but 200ms per client per tick per direction per message, massively accumulating total lataency
    maybe_delay();
    bytes frame = frame_message(type, payload);
    return sock_.send_all(frame.data(), frame.size());
}

bool Connection::recv_frame(MsgType& out_type, std::vector<uint8_t>& out_payload) {
    maybe_delay();
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
