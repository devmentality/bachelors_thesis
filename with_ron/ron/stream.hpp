//
// Created by gritzko on 2/6/21.
//

#ifndef PULSAR_STREAM_HPP
#define PULSAR_STREAM_HPP

#include "const.hpp"
#include "fcntl.h"
#include "netinet/in.h"
#include "poll.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "unistd.h"
#include "utf8.hpp"
//#include "url.hpp"

namespace ron {

/** Stream abstracts away all input-output operations, be it network
 *  sockets, other processes, or files. May be a mere buffer with no
 *  backing file. Fill() ands data to the stream buffer, DrainToString()
 *  removes data. Memory: [consumed|filled|free].
 *
 *  This fill-consume dynamics makes sense even for read-only mmaps,
 *  as we may open a prefix of a log. Thus our valid range will be
 *  shorter than the mmapped range, but we may extend that range as
 *  other process keeps writing data -- in real time, without
 *  remapping the entire range (no syscalls).
 *
 *  Stream buffers must support:
 *
 *    - [ ] old plain buffers
 *    - [ ] file io by mmapping
 *    - [ ] file io by read/write
 *    - [ ] network io by read/write
 *    - [ ] process io by socket read/write
 *
 *  TODO ZIP/UNZIP modes
 *  */
class Stream {
   public:
    enum MODE : uint8_t {
        /** not initialized */
        EMPTY = 0,
        /** mmapped memory, no file, writable */
        TMP,
        /** mmapped file, writable */
        MMAP_RW,
        /** mmapped file, read only */
        MMAP_RO,
        /** file descriptor, mmapped buffer, see read(2) */
        READ,
        /** write(2) */
        WRITE,
        /** write(2), append-only mode */
        APPEND,
        /** shared memory, circular buffer, writer */
        SHARED_WRITE,  // TODO
        /** shared memory, circular buffer, reader */
        SHARED_READ,  // TODO
        /** does not own the buffer or the file */
        NONOWNING,  // TODO
        /** no buffer, sends straight from the file */
        SENDFILE,
        /** no buffer, receives */
        RECVFILE,  // TODO
    };
    enum FLAGS : uint8_t {
        PLAIN,
        /** does not own the file descriptor */
        NOOWN = 1 << 1,
        /** writes to the file automatically */
        AUTO_DRAIN = 1 << 2,
        /** reads new data automatically */
        AUTO_FILL = 1 << 3,
    };

    static constexpr int CLOSED = -1;
    static constexpr int DEFAULT_ACCESS = S_IRUSR | S_IWUSR | S_IRGRP;
    static Position ToPageSize(Position size) {
        auto floor = size & ~(RAM_PAGE_SIZE - 1);
        if (size == floor) return size;
        return floor + RAM_PAGE_SIZE;
    };

   protected:
    span8 mmap_;
    /** map_ [...DRAINED..._d...FILLED...._f...IDLE...] */
    span8::const_iterator from_;
    span8::iterator till_;
    int file_;
    MODE mode_;
    uint8_t flag_;

    static PROC MapMem(span8& mem, Position size) {
        void* m = ::mmap(nullptr, size, PROT_READ | PROT_WRITE,
                         MAP_ANONYMOUS | MAP_PRIVATE, CLOSED, 0);
        CALL_C(m != MAP_FAILED, FAILMMAP);
        mem = span8{(uint8_t*)m, size};
        DONE;
    }

    PROC MapMem(Position size = RAM_PAGE_SIZE) {
        NEED(IsEmpty(), BADSTATE);  // FIXME which
        CALL(MapMem(mmap_, size));
        from_ = mmap_.begin();
        till_ = mmap_.begin();
        DONE;
    }

    PROC MapFile(Position size, bool writable = false) {
        NEED(IsEmpty(), BADSTATE);
        auto mode = writable ? PROT_READ | PROT_WRITE : PROT_READ;
        void* m = ::mmap(nullptr, size, mode, MAP_FILE | MAP_SHARED, file_, 0);
        if (m == MAP_FAILED) {
            close(file_);
            file_ = CLOSED;
            FAIL(FAILMMAP);
        }
        mmap_ = span8{(uint8_t*)m, size};
        from_ = mmap_.begin();
        till_ = mmap_.end();
        DONE;
    }

    PROC RemapFile(Position size, bool writable = false) {
        CALL(UnMap());
        CALL(MapFile(size, writable));
        DONE;
    }

    PROC UnMap() {
        CALL(UnMap(mmap_));
        mmap_ = span8{};
        DONE;
    }

    static PROC UnMap(span8 map) {
        CALL_C(munmap((void*)map.data(), map.size()) == 0, FAILMMAP);
        DONE;
    }

    PROC FeedDataUnsafe(span8c data) {
        auto w = Idle();
        assert(w.size_bytes() >= data.size_bytes());
        memcpy(w.begin(), data.begin(), data.size_bytes());
        Fed(data.size_bytes());
        DONE;
    }

    void FeedUnsafe(uint8_t x) {
        *till_ = x;
        ++till_;
    }

    void FeedUnsafe16(uint16_t x) {
        *(uint16_t*)till_ = x;
        till_ += 2;
    }

    void FeedUnsafe32(uint32_t x) {
        *(uint32_t*)till_ = x;
        till_ += 4;
    }

    void FeedUnsafe64(uint64_t x) {
        *(uint64_t*)till_ = x;
        till_ += 8;
    }

    void Verify() {
        assert(from_ <= till_);
        assert(mmap_.begin() <= from_);
        assert(till_ <= mmap_.end());
    }

   public:
    Stream() = default;
    Stream(Stream&& b) noexcept
        : mmap_{b.mmap_},
          file_{b.file_},
          mode_{b.mode_},
          from_{b.from_},
          till_{b.till_},
          flag_{b.flag_} {
        b.mmap_ = span8{};
        b.file_ = CLOSED;
        b.mode_ = EMPTY;
    }

    ~Stream() {
        if (!IsEmpty()) Close();
    }

    Stream& operator=(Stream&& b) noexcept {
        mmap_ = b.mmap_;
        b.mmap_ = span8{};
        file_ = b.file_;
        b.file_ = CLOSED;
        mode_ = b.mode_;
        b.mode_ = EMPTY;
        from_ = b.from_;
        till_ = b.till_;
        flag_ = b.flag_;
        return *this;
    }

    bool IsOpen() const {
        // A stream memset to zeros will have file_!=CLOSED
        return !mmap_.empty() && file_ != CLOSED;
    }
    bool IsFileOpen() const { return file_ != CLOSED; }
    MODE Mode() const { return mode_; }
    int Descriptor() const { return file_; }
    Position NonIdleSize() const { return till_ - mmap_.begin(); }
    span8c Filled() const { return span8c{from_, till_}; }
    Position FilledSize() const { return till_ - from_; }
    span8 Idle() { return span8{till_, mmap_.end()}; }
    Position IdleSize() const { return mmap_.end() - till_; }
    /** begin of the filled part, end of the drained part */
    span8c::iterator At() const { return from_; }
    /** begin of the idle part, end of the filled part */
    span8::iterator To() const { return till_; }
    bool IsEmpty() const { return mode_ == EMPTY; }
    bool IsDrained() const { return from_ == till_; }
    Position DrainedSize() const { return from_ - mmap_.begin(); }
    Position Size() const { return mmap_.size_bytes(); }
    span8c Data() const { return mmap_; }
    span8 Data() { return mmap_; }
    const void* RawData() const { return mmap_.data(); }
    void* RawData() { return mmap_.data(); }
    void SetFlag(FLAGS flag) { flag_ |= flag; }
    void UnsetFlag(FLAGS flag) { flag_ &= ~flag; }
    bool HasFlag(FLAGS flag) const { return flag_ & flag; }

    PROC Create(const std::string& path, MODE mode,
                Position size = RAM_PAGE_SIZE);

    /** the universal Create() as per CFOC; TODO make all Opens aliases */
    PROC Open(int fd, MODE mode, FLAGS mode_flags = PLAIN);

    PROC OpenFileRead(int fd) { return Open(fd, READ); }

    PROC OpenFileWrite(int fd) { return Open(fd, WRITE); }

    PROC Open(const std::string& path, MODE mode);

    PROC Create() { return Open(CLOSED, TMP); }

    void Swap(Stream& b);

    PROC CreateBuffer(Position size = RAM_PAGE_SIZE) {
        CALL(MapMem(size));
        mode_ = TMP;
        from_ = till_ = mmap_.begin();
        DONE;
    }

    Position FileSize() const {
        if (file_ == CLOSED) return 0;
        struct stat sb {};
        fstat(file_, &sb);
        return sb.st_size;
    }

    PROC Sync() const;

    static PROC CreateProcess(Stream& out, Stream& in,
                              const std::string& command);
    /** Create a socket for an incoming connection */
    static PROC OpenSocket(int& fd, sockaddr_in& addr, int listen_sock);
    static PROC OpenSocket(Stream& out, Stream& in, sockaddr_in& addr,
                           int listen_sock) {
        int fd;
        CALL(OpenSocket(fd, addr, listen_sock));
        CALL(OpenReadWritePair(in, out, fd));
        DONE;
    }

    static PROC CreateSocket(int& fd, const in_addr& host, uint32_t port,
                             int type);
    static PROC CreateSocket(int& fd, const in_addr& host, uint32_t port) {
        return CreateSocket(fd, host, port, SOCK_STREAM);
    }
    /** create/connect a non-blocking TCP socket;
     *  will return immediately, for the fact of connection use Poll */
    static PROC CreateNonBlockingSocket(int& fd, in_addr addr, uint32_t port) {
        return CreateSocket(fd, addr, port, SOCK_STREAM | SOCK_NONBLOCK);
    }
    static PROC CreateSocket(int& fd, const std::string& host, uint32_t port) {
        in_addr address{};
        CALL(InternetAddress(address, host.c_str()));
        return CreateSocket(fd, address, port);
    }
    /** Create a socket for a new outgoing connection */
    static PROC CreateSocket(Stream& out, Stream& in, const std::string& host,
                             uint32_t port) {
        int fd;
        CALL(CreateSocket(fd, host, port));
        return OpenReadWritePair(in, out, fd);
    }

    static PROC CreateListeningSocket(int& sock, in_addr addr, uint32_t port);
    static PROC CreateListeningSocket(int& socket, uint32_t port) {
        return CreateListeningSocket(socket, in_addr{htonl(INADDR_ANY)}, port);
    }

    static struct sockaddr_in TCPSocketAddress(in_addr address, uint16_t port) {
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(struct sockaddr_in));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr = address;
        serv_addr.sin_port = htons(port);
        return serv_addr;
    }

    static PROC InternetAddress(in_addr& addr, const char* hostname);

    static PROC CreateNonBlockingSocket(int& fd, const std::string& addr,
                                        uint32_t port) {
        in_addr a{};
        CALL(InternetAddress(a, addr.c_str()));
        return CreateNonBlockingSocket(fd, a, port);
    }

    static PROC MakeNonBlocking(int fd) {
        CALL_C(fcntl(fd, F_SETFL, O_NONBLOCK) == 0, FAILINOUT);
        DONE;
    }

    static PROC OpenReadWritePair(Stream& read, Stream& write, int fd) {
        NEED(read.IsEmpty() && write.IsEmpty(), BADSTATE);
        CALL(read.OpenFileRead(fd));
        CALL(write.OpenFileWrite(fd));
        read.SetFlag(NOOWN);
        DONE;
    }

    void Clear() { from_ = till_ = mmap_.begin(); }

    /** for writers: send the data to the file; in case of compressed files,
     *  flips the mode to "deflated" if not yet (on empty buf flips back)
     *  @return OK, NOSPACE(EAGAIN, non-blocking), FAILINOUT(error) */
    PROC Drain();

    PROC DrainToFile(int into_fd);

    PROC DrainData(span8& into) {
        auto c = Filled();
        auto s = std::min(c.size(), into.size());
        memcpy(into.data(), c.data(), s);
        from_ += s;
        into = into.subspan(0, s);
        DONE;
    }

    PROC DrainAll();

    /** for readers: receives the data from the underlying stream */
    PROC Feed();

    PROC FeedByte(uint8_t x) {
        if (Idle().empty()) CALL(ReserveSpace(1));
        Idle().front() = x;
        Fed(1);
        DONE;
    }

    PROC UnfeedByte() {
        NEED(till_ >= from_, BADSTATE);
        --till_;
        DONE;
    }

    PROC FeedFromFile(int from_fd);

    /** Writes the slice to the buf. Does NOT shift or reallocate the buf */
    PROC FeedSpan(span8c data) {
        CALL(ReserveSpace(data.size()));
        auto w = Idle();
        //        NEED(w.size_bytes() >= data.size_bytes(),
        //        NOSPACE.Ext(BASE_b));
        memcpy(w.begin(), data.begin(), data.size_bytes());
        Fed(data.size_bytes());
        DONE;
    }

    PROC FeedFromStream(Stream& src, Position max_size = MAX_POSITION) {
        NEED(IdleSize(), NOSPACE);
        NEED(src.FilledSize(), NODATA);
        auto f = src.Filled();
        if (max_size > IdleSize()) max_size = IdleSize();
        if (max_size < f.size_bytes()) f = f.subspan(0, max_size);
        CALL(FeedSpan(f));
        src.Drained(f.end());
        DONE;
    }

    PROC FeedAllFromStream(Stream& src, Position max_size = RAM_PAGE_SIZE) {
        NEED(src.FilledSize() > 0, NODATA);
        auto sz = std::min(src.FilledSize(), max_size);
        if (IdleSize() < sz) CALL(ReserveSpace(src.FilledSize()));
        return FeedFromStream(src, max_size);
    }

    /** Takes as much data as possible from the specified range.
     * Sets the range to the unconsumed data (or empty) */
    PROC FeedFromSpan(span8c& data) {
        auto w = Idle();
        auto s = std::min(w.size_bytes(), data.size_bytes());
        memcpy(w.data(), data.data(), s);
        till_ += s;
        data = data.subspan(s);
        DONE;
    }

    PROC FeedString(const std::string& data) {
        return FeedSpan(as_span8c(data));
    }

    PROC FeedDeflated(span8c raw_data);

    PROC Fill(const std::string& data) { return FeedSpan(as_span8c(data)); }

    PROC DrainToString(std::string& data) {
        auto c = Filled();
        auto s = data.size();
        data.resize(s + c.size());
        auto r = as_span8(data).subspan(s);
        return DrainData(r);
    }

    PROC DrainToSpan(span8& span) {
        auto f = Filled();
        auto size = std::min(f.size(), span.size_bytes());
        memcpy(span.data(), f.data(), size);
        span = span.subspan(size);
        Drained(size);
        DONE;
    }

    bool IsShiftable() const {
        return Filled().begin() > mmap_.begin() + (mmap_.size() >> 1U);
    }

    /** shifts the filled range to the beginning of the buffer */
    PROC Shift() {
        assert(IsShiftable());
        auto filled = Filled();
        if (!filled.empty())
            memcpy(mmap_.begin(), filled.begin(), filled.size());
        from_ = mmap_.begin();
        till_ = mmap_.begin() + filled.size();
        DONE;
    }

    PROC MayShift() {
        NEED(IsShiftable(), OK);
        return Shift();
    }

    /*  Resizes the buffer, shifts the content, may also
     *  need to resize the file in the mmapped mode. */
    PROC Resize(Position new_size);

    /** Ensure there is this much of fillable space. May shift and
     *  reallocate, so pointers will change. */
    PROC ReserveSpace(Position size = RAM_PAGE_SIZE) {
        NEED(IdleSize() < size, OK);
        if (flag_ & AUTO_DRAIN) {
            TRY(draining, Drain());
            NEED(draining == OK || draining.Is(NOSPACE), draining);
        }
        CALL(MayShift());
        auto new_size = ToPageSize(NonIdleSize() + size);
        return Resize(new_size);
    }

    void Drained(unsigned size) {
        MUST(size <= FilledSize(), "overdrain");
        from_ += size;
    }
    /**  */
    void Drained(span8::const_iterator p) {
        MUST(p >= from_ && p <= till_, "overdrain ptr");
        from_ = p;
    }

    void Fed(span8::pointer p) {
        MUST(p >= till_ && p <= mmap_.end(), "overfill ptr");
        till_ = p;
    }
    void Fed(unsigned size) {
        MUST(size <= IdleSize(), "overfill sz");
        till_ += size;
    }

    PROC FeedCString(const char* str) {
        return FeedSpan(span8c{(uint8_t*)str, strlen(str)});
    }

    PROC Lock();

    PROC Close() {
        if (IsEmpty()) DONE;
        //        if (FilledSize()) {
        //            std::cerr << "warning: closing an undrained stream\n";
        //        }
        CALL(UnMap());
        if (file_ != CLOSED) {
            if (!(flag_ & NOOWN)) close(file_);
            file_ = CLOSED;
        }
        mode_ = EMPTY;
        from_ = nullptr;
        till_ = nullptr;
        flag_ = 0;
        DONE;
    }

    static PROC Rename(const std::string& from, const std::string& to);

    static PROC ParseIP(in_addr& into, const std::string& ip_str);
    static PROC RenderIP(std::string& into, in_addr address);

    static PROC RenderAddress(std::string& into, const sockaddr_in& addr) {
        char port[8];
        NEED(addr.sin_family == AF_INET, BADARGMNT);
        CALL(RenderIP(into, addr.sin_addr));
        into.push_back(':');
        sprintf(port, "%i", addr.sin_port);
        into.append(port);
        DONE;
    }

    static std::string AddressAsString(const sockaddr_in& sockaddrIn) {
        std::string ret{};
        ret.reserve(32);
        RenderAddress(ret, sockaddrIn);
        return ret;
    }

    template <typename T>
    friend class StreamOf;
};

/** A poll loop backed by either poll() or epoll() */
class Poll {
   public:
    using Files = std::vector<int>;

   protected:
#ifndef EPOLLIN
    std::vector<pollfd> files_;
#else
    int fd_;
#endif
   public:
    Poll() {}

    bool IsOpen() const;

    PROC Open();

    PROC AddFile(int fd, int flags);
    PROC PollFiles(Files& reads, Files& writes, Files& errors, long ms = 1000);
    PROC RemoveFile(int fd);

    PROC Close();
};

/** a Stream of anything; it is highly recomended sizeof(T) to be 2^k */
template <typename T>
class StreamOf {
   protected:
    Stream file_;

   public:
    StreamOf() = default;

    T& operator[](Index i) { return ((T*)file_.mmap_.data())[i]; }

    Index RecordSize() const { return file_.mmap_.size_bytes() / sizeof(T); }

    PROC Create(const std::string& path,
                Index record_size = RAM_PAGE_SIZE / sizeof(T)) {
        return file_.Create(path, Stream::MMAP_RW, record_size * sizeof(T));
    }

    PROC Create(Index record_size = RAM_PAGE_SIZE / sizeof(T)) {
        return file_.Create("", Stream::TMP, record_size * sizeof(T));
    }

    PROC Open(const std::string& path) {
        return file_.Open(path, Stream::MMAP_RW);
    }

    PROC Reserve(Index new_size) {
        return file_.ReserveSpace(new_size * sizeof(T));
    }

    PROC ReserveRecords(Index count = 1);

    PROC FeedRecord(const T& rec);

    PROC Sync() { return file_.Sync(); }

    PROC Close() { return file_.Close(); }
};

}  // namespace ron

#endif  // PULSAR_STREAM_HPP
