#include "tcp_socket.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

TcpSocket::TcpSocket() : fd_(-1) {}
TcpSocket::TcpSocket(int fd) : fd_(fd) {}

TcpSocket::~TcpSocket() {
    if (fd_ >= 0) ::close(fd_); // free's file disc
}

TcpSocket::TcpSocket(TcpSocket&& other) noexcept : fd_(other.fd_) {
    other.fd_ = -1; // when moved, the other resource transfers ownership of fd to new object
}

TcpSocket& TcpSocket::operator=(TcpSocket&& other) noexcept {
    if (this != &other) {
        if (fd_ >= 0) ::close(fd_);
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

bool TcpSocket::valid() const {
    return fd_ >= 0;
}

bool TcpSocket::connect(const std::string& host, uint16_t port) {
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (::inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
        return false;

    return ::connect(fd_, (sockaddr*)&addr, sizeof(addr)) >= 0;
}

bool TcpSocket::bind_and_listen(uint16_t port, int backlog) {
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) return false;

    int opt = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(fd_, (sockaddr*)&addr, sizeof(addr)) < 0) return false;
    if (::listen(fd_, backlog) < 0) return false;

    return true;
}

TcpSocket TcpSocket::accept_client() {
    int cfd = ::accept(fd_, nullptr, nullptr);
    return TcpSocket(cfd);
}

bool TcpSocket::send_all(const uint8_t* data, size_t size) {
    while (size > 0) {
        ssize_t sent = ::send(fd_, data, size, 0);
        if (sent <= 0) return false;
        data += sent;
        size -= sent;
    }
    return true;
}

bool TcpSocket::recv_all(uint8_t* data, size_t size) {
    while (size > 0) {
        ssize_t rec = ::recv(fd_, data, size, 0);
        if (rec <= 0) return false;
        data += rec;
        size -= rec;
    }
    return true;
}

void TcpSocket::close() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}
