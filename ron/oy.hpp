//
// Created by gritzko on 9/16/20.
//

#ifndef PULSAR_OY_HPP
#define PULSAR_OY_HPP

#include "const.hpp"

namespace ron {

enum AXIS : uint8_t {
    OBJECT = 1,
    YARN = 0,
};

/** an op's position: the log offset, O/Y index */
struct Locus {
    Position pos;
    Index ndx;

    Locus operator-(Locus b) const { return Locus{pos - b.pos, ndx - b.ndx}; }
    Locus operator+(Locus b) const { return Locus{pos + b.pos, ndx + b.ndx}; }
};

/** An iterable Range of ops along one of the oplog axes: Y or O.
 *  It might be a segment, prefix or postfix of an object log (O axis)
 *  or a segment etc of a yarn log (Y axis).
 *  In a pulsar oplog, ranges can be iterated end-to-beginning
 *  naturally or beginning-to-end using an axial bangee.
 *  Either way, a Range variable mentions the position of the
 *  end op (to start the iteration from) and the range of indices to
 *  iterate (either ondx or yndx, O- and Y-axis respectively).
 *  @deprecated
 */
struct Range : Double {
    Range() = default;
    Range(Position pos, Index e) : Double{pos, e} {}
    Range(Position pos, Index b, Index e) : Double{pos, Halves{e, b}.word} {}

    bool IsEmpty() const { return Pos() == 0; }
    /**  Ranges that start with index 0 are prefixes. Postfixes end at the
     *  last op of a yarn or an object. */
    bool IsPrefix() const { return second <= MAX_INDEX; }

    Index Ndx() const { return Halves::Of(second).ls_half; }
    Position Pos() const { return first; }
    Index& Ndx() { return Halves::Of(second).ls_half; }
    Position& Pos() { return first; }
    IndexRange NdxRange() const {
        Halves eb{second};
        return IndexRange{eb.ms_half, eb.ls_half};
    }
    Index Limit() const { return Halves::Of(second).ms_half; }
    Index& Limit() { return Halves::Of(second).ms_half; }
    std::string String() const {
        char tmp[32]{};
        sprintf(tmp, "ndx %i..%i pos %li", Limit(), Ndx(), Pos());
        return tmp;
    }
};

}  // namespace ron

#endif  // PULSAR_OY_H
