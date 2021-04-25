//
// Created by gritzko on 2/6/21.
//
#include "stream.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "netdb.h"
#include "sys/file.h"

namespace ron {
using namespace ron;
using namespace std;

PROC Stream::ParseIP(in_addr &into, const std::string &ip_str) {
    NEED(inet_aton(ip_str.data(), &into), BADSYNTAX.Ext(BASE_i));
    DONE;
}

PROC Stream::RenderIP(std::string &into, in_addr address) {
    auto a = inet_ntoa(address);
    into.clear();
    into.append(a);
    DONE;
}
PROC Stream::Lock() {
    NEED(IsOpen(), BADSTATE);
    CALL_C(0 == flock(file_, LOCK_EX), FAILINOUT);
    DONE;
}

PROC Stream::Open(int fd, MODE mode, FLAGS mode_flags) {
    NEED(IsEmpty(), BADSTATE);
    file_ = fd;
    switch (mode) {
        case TMP:
            NEED(fd == -1, BADARGMNT);
            // no break
        case READ:
        case WRITE:
        case APPEND:
            CALL(MapMem());
            break;
        case MMAP_RW:
        case MMAP_RO:
            CALL(MapFile(FileSize(), mode == MMAP_RW));
            // TODO prefix open, use mode flags
            break;
        default:
            FAIL(BADARGMNT);
    }
    mode_ = mode;
    flag_ = mode_flags;
    DONE;
}

PROC Stream::Resize(Position new_size) {
    span8 m;
    Status mapped;
    size_t size;

    switch (mode_) {
        case MMAP_RO:
            return NOTOPEN_W;
        case MMAP_RW:
        case SHARED_WRITE:
            CALL_C(0 == ftruncate(file_, new_size), FAILINOUT);
            CALL(UnMap());
            CALL(MapFile(new_size, true));
            DONE;
        case READ:
        case WRITE:
        case TMP:
            CALL(MapMem(m, new_size));
            size = FilledSize();
            memcpy(m.begin(), from_, size);
            CALL(UnMap(mmap_));
            mmap_ = m;
            from_ = mmap_.begin();
            till_ = mmap_.begin() + size;
            DONE;
        case EMPTY:
            return CreateBuffer(ToPageSize(new_size));
        case APPEND:
        case SHARED_READ:
            FAIL(BADSTATE_o);
        default:
            FAIL(NOTIMPLTD);
    }
    DONE;
}

struct sockaddr_in TCPSocketAddress(in_addr address, uint16_t port) {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr = address;
    serv_addr.sin_port = htons(port);
    return serv_addr;
}

PROC Stream::InternetAddress(in_addr &addr, const char *hostname) {
    struct hostent *he;
    struct in_addr **addr_list;

    if ((he = gethostbyname(hostname)) == nullptr) {
        herror("gethostbyname");
        FAIL(FAILNET);
    }

    addr_list = (struct in_addr **)he->h_addr_list;

    auto l = 0;
    while (addr_list[l] != nullptr) ++l;
    auto k = rand() % l;
    addr = *addr_list[k];

    DONE;
}

PROC Stream::CreateSocket(int &sock, const in_addr &host, uint32_t port,
                          int type) {
    auto serv_addr = TCPSocketAddress(host, port);
    auto fd = socket(AF_INET, type, 0);
    CALL_C(fd != CLOSED, FAILNET);
    auto p = (struct sockaddr *)(&serv_addr);
    auto c = connect(fd, p, sizeof(struct sockaddr_in));
    if (c == -1 && errno != EINPROGRESS) {
        close(fd);
        CALL_C(false, FAILNET);
    }
    sock = fd;
    DONE;
}

PROC Stream::Open(const std::string &path, MODE mode) {
    NEED(IsEmpty(), BADSTATE);
    int flag{O_RDONLY}, fd{CLOSED};
    switch (mode) {
        case APPEND:
            flag = O_APPEND | O_WRONLY;
            break;
        case SHARED_WRITE:
            flag = O_WRONLY;
            break;
        case WRITE:
            flag = O_WRONLY;
            if (path=="-")
                return Open(STDOUT_FILENO, mode);
            break;
        case SHARED_READ:
        case MMAP_RW:
            flag = O_RDWR;
            break;
        case MMAP_RO:
            flag = O_RDONLY;
            break;
        case READ:
            flag = O_RDONLY;
            if (path=="-")
                return Open(STDIN_FILENO, mode);
            break;
        case TMP:
            return Open(CLOSED, TMP);
        default:
            FAIL(BADARGMNT);
    }
    fd = open(path.c_str(), flag);
    CALL_C(fd != CLOSED, FAILOPEN);
    auto opening = Open(fd, mode);
    if (opening != OK) {
        close(fd);
        FAIL(opening);
    }
    DONE;
}

PROC Stream::OpenSocket(int &fd, sockaddr_in &addr, int listen_sock) {
    struct sockaddr peer_addr;
    socklen_t asz{};
    auto sock = accept(listen_sock, &peer_addr, &asz);
    NEED(sock >= 0, FAILINOUT.Ext(BASE_a));
    auto p = (struct sockaddr *)&addr;
    socklen_t len = sizeof(sockaddr_in);
    CALL_C(0 == getpeername(sock, p, &len), FAILNET);
    fd = sock;
    DONE;
}

PROC Stream::CreateProcess(Stream &out, Stream &in,
                           const std::string &command) {
    int fds[2];

    CALL_C(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0, NOTAVAILB);
    auto pid = vfork();
    if (pid == -1) {
        close(fds[0]);
        close(fds[1]);
        FAIL(FAILEXEC);
    }
    if (pid == 0) { /* child */
        close(fds[0]);
        dup2(fds[1], 0);
        dup2(fds[1], 1);
        close(fds[1]);
        execl("/bin/sh", "sh", "-c", command.c_str(), NULL);
        _exit(127);
    }
    /* parent */
    close(fds[1]);
    return OpenReadWritePair(in, out, fds[0]);
}

PROC Stream::CreateListeningSocket(int &sock, in_addr address, uint32_t port) {
    static constexpr unsigned BACKLOG = 128;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    CALL_C(sock != CLOSED, FAILNET);
    int v{1};
    CALL_C(0 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(int)),
           FAILNET);
    auto serv_addr = TCPSocketAddress(address, port);
    auto p = (struct sockaddr *)(&serv_addr);
    CALL_C(0 == bind(sock, p, sizeof(struct sockaddr_in)), FAILNET);

    CALL_C(0 == listen(sock, BACKLOG), FAILNET);

    DONE;
}

PROC Stream::Sync() const {
    switch (mode_) {
        case TMP:
        case READ:
        case MMAP_RO:
        case SHARED_READ:
            DONE;
        case APPEND:
        case WRITE:
        case SHARED_WRITE:
            CALL_C(0 == fsync(file_), FAILINOUT);
            DONE;
        case MMAP_RW:
            CALL_C(0 == msync(mmap_.data(), mmap_.size(), MS_SYNC), FAILINOUT);
            DONE;
        case EMPTY:
        default:
            FAIL(BADSTATE);
    }
}

PROC Stream::DrainToFile(int into_fd) {
    auto f = Filled();
    auto bytes_written = write(into_fd, f.data(), f.size());
    if (bytes_written >= 0) {
        Drained(bytes_written);
    } else if (errno == EAGAIN) {
        FAIL(NOSPACE);
    } else {
        CALL_C(false, FAILINOUT);
    }
    DONE;
}

PROC Stream::Drain() {
    Index *cf, d;
    switch (mode_) {
        case WRITE:
        case APPEND:
            return DrainToFile(file_);
        case SHARED_WRITE:
            cf = (Index *)mmap_.data();
            NEED(cf[0] >= DrainedSize(), BADSTATE_x);
            d = cf[0] - DrainedSize();
            from_ += d;
            DONE;
        default:
            // manual consumption only
            DONE;
    }
}

PROC Stream::DrainAll() {
    int us = 1;
    switch (mode_) {
        case APPEND:
        case WRITE:
            while (FilledSize()) CALL(Drain());
            DONE;
        case SHARED_WRITE:
            CALL(Drain());
            while (FilledSize()) {
                usleep(us);
                CALL(Drain());
                if (us < 1000) us <<= 1;
            }
            DONE;
        default:
            DONE;
    }
}

PROC Stream::FeedDeflated(span8c raw_data) { FAIL(NOTIMPLTD); }

PROC Stream::FeedFromFile(int from_fd) {
    auto f = Idle();
    auto bytes_read = read(from_fd, f.data(), f.size());
    if (bytes_read > 0) {
        Fed(bytes_read);
    } else if (bytes_read == 0) {
        FAIL(ENDOFRAMEE);
    } else if (errno == EAGAIN) {
        FAIL(NODATA);
    } else {
        CALL_C(false, FAILINOUT);
    }
    DONE;
}

PROC Stream::Feed() {
    switch (mode_) {
        case READ:
            CALL(MayShift());
            return FeedFromFile(file_);
        default:
            FAIL(NODATA);
    }
}

PROC Stream::Create(const std::string &path, MODE mode, Position size) {
    NEED(IsEmpty(), BADSTATE);
    // FIXME fsync the dir entry
    int flag;
    switch (mode) {
        case MMAP_RO:
        case READ:
            FAIL(BADARGMNT);
        case WRITE:
        case APPEND:
            CALL(MapMem());
            flag = mode==WRITE ? O_TRUNC : O_APPEND;
            file_ =
                open(path.c_str(), O_CREAT | O_RDWR | flag, DEFAULT_ACCESS);
            CALL_C(file_!=CLOSED, FAILOPEN);
            break;
        case TMP:
            CALL(MapMem(size));
            file_ = CLOSED;
            break;
        case MMAP_RW:
            file_ = open(path.c_str(), O_CREAT | O_RDWR, DEFAULT_ACCESS);
            CALL_C(file_!=CLOSED, FAILOPEN);
            CALL_C(0 == ftruncate(file_, size), FAILINOUT);
            CALL(MapFile(size, true));
            break;
        case SHARED_WRITE:
            file_ = shm_open(path.c_str(), O_CREAT | O_RDWR | O_APPEND,
                             DEFAULT_ACCESS);
            CALL_C(file_ != CLOSED, FAILOPEN);
            CALL_C(0 == ftruncate(file_, size), FAILINOUT);
            CALL(MapFile(size, true));
            till_ += sizeof(Position) * 2;
            from_ += sizeof(Position) * 2;
            DONE;
        default:
            FAIL(BADARGMNT);
    }
    mode_ = mode;
    DONE;
}

PROC Stream::Rename(const std::string &from, const std::string &to) {
    CALL_C(0 == rename(from.c_str(), to.c_str()), FAILINOUT);
    // FIXME fsync dir entry
    DONE;
}

void Stream::Swap(Stream &b) {
    swap( mmap_, b.mmap_);
    swap(from_, b.from_);
    swap(till_, b.till_);
    swap(file_, b.file_);
    swap(mode_, b.mode_);
    swap(flag_, b.flag_);
}

}  // namespace ron
