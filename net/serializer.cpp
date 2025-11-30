#include "serializer.h"
#include "snapshot.h"
#include <cstring>

static void write_u8(bytes& buf, std::uint8_t v) {
    buf.push_back(v);
}

static void write_u32(bytes& buf, std::uint8_t v) {
    for (int i = 0; i < 4; i++)
        buf.push_back((v >> (i * 8)) & 0xFF);
}

static void write_u64(bytes& buf, std::uint8_t v) {
    for (int i = 0; i < 8; i++)
        buf.push_back((v >> (i * 8)) & 0xFF);
}

static void write_f32(bytes& buf, float f) {
    std::uint32_t v;
    std::memcpy(&v, &f, 4);
    write_u32(buf, v);
}

static bool read_u8(const std::uint8_t*& p, const std::uint8_t* end, std::uint8_t& out) {
    if (p >= end) return false;
    out = *p++;
    return true;
}

static bool read_u32(const std::uint8_t*& p, const std::uint8_t* end, std::uint32_t& out) {
    if (p + 4 > end) return false;
    out = 0;
    for (int i = 0; i < 4; ++i)
        out |= (std::uint32_t)p[i] << (8 * i);
    p += 4;
    return true;
}

static bool read_u64(const std::uint8_t*& p, const std::uint8_t* end, std::uint64_t& out) {
    if (p + 8 > end) return false;
    out = 0;
    for (int i = 0; i < 8; ++i)
        out |= (std::uint64_t)p[i] << (8 * i);
    p += 8;
    return true;
}

static bool read_f32(const std::uint8_t*& p, const std::uint8_t* end, float& out) {
    std::uint32_t v;
    if (!read_u32(p, end, v)) return false;
    std::memcpy(&out, &v, 4);
    return true;
}


bytes serialize_input(const Pair& msg) {
    bytes out;
    write_f32(out, msg.x);
    write_f32(out, msg.y);
    return out;
}

bool deserialize_input(const std::uint8_t* data, std::size_t size, Pair& out) {
    const std::uint8_t* p = data;
    const std::uint8_t* end = data + size;
    return read_f32(p, end, out.x)
        && read_f32(p, end, out.y);
}

bytes serialize_shotshot(const Snapshot& snapshot) {
    bytes out;

    write_u64(out, snapshot.tick);

    for (int i = 0; i < kMPlayers; ++i) {
        write_f32(out, snapshot.player_pos[i].x);
        write_f32(out, snapshot.player_pos[i].y);
        write_u32(out, snapshot.player_score[i]);
        write_u8(out,  snapshot.player_connected[i]);
    }

    for (int i = 0; i < kMCoins; ++i) {
        write_f32(out, snapshot.coin_pos[i].x);
        write_f32(out, snapshot.coin_pos[i].y);
        write_u8(out,  snapshot.coin_active[i]);
    }

    return out;
}

bool deserialize_snapshot(const std::uint8_t* data, std::size_t size, Snapshot& out) {
    const std::uint8_t* p = data;
    const std::uint8_t* end = data + size;

    if (!read_u64(p, end, out.tick)) return false;

    for (int i = 0; i < kMPlayers; ++i) {
        if (!read_f32(p, end, out.player_pos[i].x)) return false;
        if (!read_f32(p, end, out.player_pos[i].y)) return false;
        std::uint32_t score;
        if (!read_u32(p, end, score)) return false;
        out.player_score[i] = score;
        if (!read_u8(p, end, out.player_connected[i])) return false;
    }

    for (int i = 0; i < kMCoins; ++i) {
        if (!read_f32(p, end, out.coin_pos[i].x)) return false;
        if (!read_f32(p, end, out.coin_pos[i].y)) return false;
        if (!read_u8(p, end, out.coin_active[i])) return false;
    }

    return true;
}

// framed msg is of type [len][msg_type][payload]
bytes make_framed_message(
    MsgType type,
    const bytes& payload)
{
    bytes buf;
    // 1byte for msg type and the rest is payload
    std::uint32_t length = 1 + payload.size();

    write_u32(buf, length);
    write_u8(buf, static_cast<std::uint8_t>(type));

    buf.insert(buf.end(), payload.begin(), payload.end());
    return buf;
}

bool parse_frame_header(const std::uint8_t* data, std::size_t size,
                        std::uint32_t& out_len, MsgType& out_type,
                        const std::uint8_t*& out_payload)
{
    if (size < 5) return false;

    const std::uint8_t* p = data;
    const std::uint8_t* end = data + size;

    if (!read_u32(p, end, out_len)) return false;

    std::uint8_t t;
    if (!read_u8(p, end, t)) return false;
    out_type = static_cast<MsgType>(t);

    out_payload = p;  // payload starts just after msg_type
    return true;
}

