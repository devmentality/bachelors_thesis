//
// Created by gritzko on 3/7/21.
//

#ifndef PULSAR_RON_STREAMS_HPP
#define PULSAR_RON_STREAMS_HPP
#include "basic-streams.hpp"
#include "op.hpp"
#include "oy.hpp"

namespace ron {

/** */
class RONtStream : public TextStream {
    // special states; regular parser states are defined by Ragel
    enum STATE : int {
        INNOTHING = 0,
        INFRAME = MAX_INDEX,  // all ragel states are lower than this
        FRAME_OVER = INFRAME + 80,
        QUERY_FRAME_OVER = FRAME_OVER + TERM::QUERY,
        FACT_FRAME_OVER = FRAME_OVER + TERM::FACT,
        ARTEFACT_FRAME_OVER = FRAME_OVER + TERM::ARTEFACT,
        IN_BLOB = INFRAME + 90,
        MAYBE_STRING_SPREAD = INFRAME + 100,
        IN_STRING_SPREAD = MAYBE_STRING_SPREAD + 1,
        IN_IFU_SPREAD = MAYBE_STRING_SPREAD + 2,
    };

    /** the current op's metadata */
    Spec spec_;
    Uuid id_;
    /** the current op's values */
    Tuple values_;
    /** remaining span length */
    Index span_len_;
    /** parser state (mostly an opaque value) */
    int state_;
    /** advance of the parser's current position rel to the drained position */
    Index advance_[2];
    /** codepoint parsing can be interrupted - keep it here (TODO as INT)*/
    Codepoint cp_;
    /** the current op's term */
    TERM term_;
    ATOM spread_;

   protected:
    PROC FeedTuple(const Tuple& tuple);
    PROC DrainOpLong();
    PROC DrainOpSpecial();
    PROC DrainEOF() {
        if (term_ == ARTEFACT) {
            state_ = -MAX_INDEX;
            Integer l;
            CALL(values_.GetInteger(l));
            NEED(l >= 0 && l <= MAX_INDEX, OUTORANGE);
            span_len_ = l;
        } else {
            state_ = 0;
            span_len_ = 0;
        }
        values_.Clear();
        spec_.Clear();
        advance_[0] = advance_[1] = 0;
        auto ret = ENDOFRAME.Ext(term_);
        term_ = PART;
        return ret;
    }

   public:
    RONtStream() = default;
    RONtStream(int fd, Stream::MODE mode) : RONtStream{} {
        Open(fd, mode);  // ignores the completion status
    }

    const Tuple& Values() const { return values_; }
    const Spec& Meta() const { return spec_; }

    PROC FeedOp(const Op& feed);
    /** Closes the frame; on an empty frame creates @0:0;*/
    PROC FeedEOF(TERM term = FACT);
    PROC FeedBlob(Stream& blob) {
        NEED(state_ == IN_BLOB, BADSTATE);
        auto d = blob.Filled();
        if (d.size() > IdleSize()) {
            d = d.subspan(0, IdleSize());
        } else {
            state_ = INNOTHING;
        }
        CALL(Stream::FeedFromSpan(d));
        blob.Drained(d.size());
        DONE;
    }

    PROC DrainOp() {
        if (spread_ == 0) return DrainOpLong();
        spec_.Next();
        if (spread_ == STRING) {
            span32 s = values_.Raw32();
            s[2] = s[4 + advance_[0]];
        }
        ++advance_[0];
        if (advance_[0] == span_len_-1) {
            spread_ = ATOM(0);
            advance_[0] = 0;
            span_len_ = 0;
            DONE;
        }
        DONE;
    }

    PROC DrainOp(Op& op) {
        CALL(DrainOp());
        op = Op{spec_, values_};
        DONE;
    }

    PROC DrainBlob(Stream& blob) {
        NEED(state_ == IN_BLOB, BADSTATE);
        auto f = Filled();
        if (f.size() > span_len_) f = f.subspan(0, span_len_);
        auto s = f.size();
        CALL(blob.FeedSpan(f));
        auto drained = s - f.size();
        Drained(drained);
        span_len_ -= drained;
        if (span_len_ == 0) state_ = INNOTHING;
        DONE;
    }

    void Reset() {
        Clear();
        spec_ = Spec{};
        values_.Clear();
        span_len_ = 0;
        state_ = 0;
        advance_[0] = advance_[1] = 0;
    }
};

class RONvStream : public VarIntStream {
   protected:
    enum STATE : uint8_t {
        HEAD_BOX = 0,
        SPEC_ID = 1,
        SPEC_REF = 2,
        NEXT_BOX = 3,
        VALUE = 4,
        VALUE_ID = VALUE + ATOM::ID,
        VALUE_INT = VALUE + ATOM::INT,
        VALUE_STRING = VALUE + ATOM::STRING,
        VALUE_FLOAT = VALUE + ATOM::FLOAT,
        OP_END = 8,
        BLOB = 9,
    };

    MicroCache cache_;
    Uuid prev_id_;
    STATE state_;
    Tuple::BoxDesc box_;

   public:
    RONvStream() = default;

    PROC DrainTuple(Tuple& tuple);
    PROC DrainOp(Op& op);
    PROC DrainBlob(Stream& blob);

    PROC FeedTuple(const Tuple& tuple);
    PROC FeedOp(const Op& op);
    /** Closes the frame; on an empty frame creates @0:0;*/
    PROC FeedEOF(TERM term = FACT);
    PROC FeedBlob(Stream& blob);

    PROC Reset() {
        Clear();
        cache_.Clear();
        prev_id_ = NIL;
        state_ = HEAD_BOX;
        box_ = Tuple::BoxDesc{};
        DONE;
    }
};

}  // namespace ron

#endif  // PULSAR_RON_STREAMS_HPP
