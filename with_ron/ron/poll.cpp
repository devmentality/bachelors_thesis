//
// Created by gritzko on 2/9/21.
//
#include "stream.hpp"

namespace ron {

using namespace std;

#ifndef EPOLLIN

bool Poll::IsOpen() const { return true; }

PROC Poll::Open() { DONE; }

PROC Poll::Close() { DONE; }

PROC Poll::AddFile(int fd, int flags) {
    short ev{};
    switch (flags) {
        case O_RDWR:
            ev = POLLOUT | POLLIN;
            break;
        case O_RDONLY:
            ev = POLLIN;
            break;
        case O_WRONLY:
            ev = POLLOUT;
            break;
        default:
            FAIL(NOTIMPLTD);
    }
    files_.push_back(pollfd{.fd = fd, .events = ev, .revents = 0});
    DONE;
}

PROC Poll::PollFiles(Files &reads, Files &writes, Files &errors, long ms) {
    auto c = poll(files_.data(), files_.size(), ms);
    CALL_C(c != -1, FAILINOUT);
    for (auto &p : files_) {
        if (p.revents == 0) continue;
        if (p.revents & POLLIN) reads.push_back(p.fd);
        if (p.revents & POLLOUT) reads.push_back(p.fd);
        p.revents &= ~(POLLOUT | POLLIN);
        if (p.revents) errors.push_back(p.fd);
        p.revents = 0;
    }
    DONE;
}

PROC Poll::RemoveFile(int fd) {
    for (int i = 0; i < files_.size(); ++i) {
        if (files_[i].fd != fd) continue;
        files_[i] = files_.back();
        files_.pop_back();
        DONE;
    }
    FAIL(NOTFOUND);
}

#else
// epoll impl

#endif

}  // namespace ron