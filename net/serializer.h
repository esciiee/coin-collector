#include <cstddef>
#include <cstdint>
#include <vector>
#include "protocol.h"
#include "snapshot.h"

using bytes = std::vector<std::uint8_t>;

bytes serialize_input(const Pair& msg);
bytes serialize_snapshot(const Snapshot& snapshot);

bool deserialize_input(const std::uint8_t* data, std::size_t size, Pair& out);
bool deserialize_snapshot(const std::uint8_t* data, std::size_t size, Snapshot& out);

// wraps payload into a frame [len][type][payload]
bytes frame_message(MsgType type, const bytes& payload);

// parse the frame header and sets payload and msg type
bool parse_frame_header(const std::uint8_t* data, std::size_t size, std::uint32_t& out_len, MsgType& out_type);

