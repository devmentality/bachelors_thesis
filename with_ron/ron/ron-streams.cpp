#include "ron-streams.hpp"

#include "op.hpp"

namespace ron {

PROC RONtStream::DrainOpSpecial() {
    switch (state_) {
        case IN_BLOB:
            FAIL(BADSTATE);
    }
    FAIL(NOTIMPLTD);
}

PROC RONtStream::FeedOp(const Op& feed) {
    switch (state_) {
        case IN_IFU_SPREAD:
            // we reserved when starting the spread
            if (!feed.IsChainedTo(spec_) || feed.Values() != values_ ||
                span_len_ == MAX_SPAN_SIZE) {
                if (span_len_ > 1) {
                    FeedUnsafe(SPAN_BRACKET_OPEN);
                    FeedInteger(span_len_);
                    FeedUnsafe(SPAN_BRACKET_CLOSE);
                }
                goto no_spread;
            }
            spec_ = feed.Meta();
            ++span_len_;
            DONE;
        case MAYBE_STRING_SPREAD:
            CALL(ReserveSpace(32));  //?
            if (!feed.IsChainedTo(spec_) || !feed.Values().IsSingularString()) {
                FeedUnsafe(' ');
                FeedUnsafe(STRING_QUOTE);
                FeedRONEscapedCodepoint(values_.GetSingularString());
                FeedUnsafe(STRING_QUOTE);
                goto no_spread;
            }
            state_ = IN_STRING_SPREAD;
            FeedUnsafe(' ');
            FeedUnsafe(SPAN_BRACKET_OPEN);
            FeedUnsafe(STRING_QUOTE);
            FeedRONEscapedCodepoint(values_.GetSingularString());
            FeedRONEscapedCodepoint(feed.Values().GetSingularString());
            spec_ = feed.Meta();
            ++span_len_;
            DONE;
        case IN_STRING_SPREAD:
            CALL(ReserveSpace(32));
            if (!feed.IsChainedTo(spec_) || !feed.Values().IsSingularString() ||
                span_len_ == MAX_SPAN_SIZE) {
                FeedUnsafe(STRING_QUOTE);
                FeedUnsafe(SPAN_BRACKET_CLOSE);
                goto no_spread;
            }
            FeedRONEscapedCodepoint(feed.Values().GetSingularString());
            spec_ = feed.Meta();
            ++span_len_;
            DONE;
        no_spread:
            values_.Clear();
            state_ = INFRAME;
            span_len_ = 0;
        case INFRAME:
            CALL(ReserveSpace(32));
            FeedUnsafe(FRAME_PART_PUNCT);
            FeedUnsafe('\n');
            // no break
        case INNOTHING:
            CALL(ReserveSpace(Uuid::MAX_BASE64_SIZE * 2 + 30));  // spec+punct
            if (spec_.id.IsEvent()) {
                if (feed.ID() != spec_.id.Inc()) {
                    FeedUnsafe(ID_PUNCT);
                    FeedUuid(feed.ID());
                }
                if (feed.Ref() != spec_.id) {
                    if (feed.ID() != spec_.id.Inc()) FeedUnsafe(' ');
                    FeedUnsafe(REF_PUNCT);
                    FeedUuid(feed.Ref());
                }
            } else {
                if (feed.ID() != spec_.id) {
                    FeedUnsafe(ID_PUNCT);
                    FeedUuid(feed.ID());
                }
                if (feed.Ref() != spec_.ref) {
                    if (feed.ID() != spec_.id) FeedUnsafe(' ');
                    FeedUnsafe(REF_PUNCT);
                    FeedUuid(feed.Ref());
                }
            }
            if (feed.ID().IsEvent() && feed.Values().IsSpreadable()) {
                if (feed.Values().IsSingularString()) {
                    state_ = MAYBE_STRING_SPREAD;
                } else {
                    CALL(FeedTuple(feed.Values()));
                    state_ = IN_IFU_SPREAD;
                    CALL(ReserveSpace(32));  // reserve once
                }
                values_ = feed.Values();
                span_len_ = 1;
            } else {
                CALL(FeedTuple(feed.Values()));  // no reserve, so check
                state_ = INFRAME;
            }
            spec_ = feed.Meta();
            break;
        default:
            FAIL(BADSTATE);
    }
    DONE;
}

PROC RONvStream::FeedEOF(TERM term) {
    uint8_t term_desc = (term & 3U) << 1U;
    MUST(term_desc != TERM::PART, "incorrect frame termination");
    CALL(Stream::FeedByte(term_desc));
    state_ = HEAD_BOX;
    prev_id_ = NIL;
    DONE;
    // return DrainTuple();  // ensure the data goes to channel/storage
}

PROC RONtStream::FeedEOF(TERM term) {
    MUST(term != TERM::PART, "incorrect frame termination");
    ReserveSpace(32);
    switch (state_) {
        case MAYBE_STRING_SPREAD:
            FeedUnsafe(' ');
            FeedUnsafe('\'');
            FeedRONEscapedCodepoint(values_.GetSingularString());
            FeedUnsafe('\'');
            break;
        case IN_STRING_SPREAD:
            FeedUnsafe('\'');
            FeedUnsafe(')');
            break;
        case IN_IFU_SPREAD:
            if (span_len_ > 1) {
                FeedUnsafe('(');
                FeedInteger(span_len_);
                FeedUnsafe(')');
            }
            span_len_ = 0;
            break;
        default:
            break;
    }
    FeedUnsafe(TERM_PUNCT[term]);
    FeedUnsafe('\n');
    state_ = INNOTHING;
    spec_ = Spec{};
    values_.Clear();
    span_len_ = 0;
    DONE;
}

PROC RONtStream::FeedTuple(const Tuple& tuple) {
    auto r = tuple.ReadBoxes();
    Tuple::Box box{};
    while (OK == r.ReadBox(box)) {
        Index size = box.Size(), i;
        switch (box.Type()) {
            case INT:
                assert(size);
                for (i = 0; i < size; ++i) {
                    CALL(FeedByte(' '));
                    CALL(FeedInteger(box.IntegerAt(i)));
                }
                break;
            case FLOAT:
                assert(size);
                for (i = 0; i < size; ++i) {
                    FeedByte(' ');
                    CALL(FeedFloat(box.FloatAt(i)));
                }
                break;
            case ID:
                for (i = 0; i < size; ++i) {
                    ReserveSpace(Uuid::MAX_BASE64_SIZE + 3);
                    FeedByte(' ');
                    if (!box.UuidAt(i).IsAmbiguous()) {
                        FeedUuid(box.UuidAt(i));
                    } else {
                        FeedByte('{');
                        FeedUuid(box.UuidAt(i));
                        FeedByte('}');
                    }
                }
                break;
            case STRING:
                CALL(ReserveSpace(4 + size));  // TODO optimize checks (Reserve)
                FeedByte(' ');
                FeedByte(ATOM_PUNCT[ATOM::STRING]);
                for (i = 0; i < size; ++i)
                    CALL(FeedRONEscapedCodepoint(box.CodepointAt(i)));
                CALL(FeedByte(ATOM_PUNCT[ATOM::STRING]));
                break;
            default:
                abort();
        }
    }
    DONE;
}

/** differently from parsing, serialization can not be resumed at half the op */
PROC RONvStream::FeedOp(const Op& op) {
    Index spec_uuids{0};
    if (op.ID() != prev_id_.Inc()) {
        spec_uuids = 2;
    } else if (op.Ref() != prev_id_) {
        spec_uuids = 1;
    }
    bool have_spec = spec_uuids;
    Tuple::BoxDesc val1desc = op.Values().FirstBoxDesc();
    have_spec |= val1desc.IsEmpty();
    have_spec |= val1desc.Type() == ID;
    if (have_spec) {
        BoxDescriptor spec_desc{ATOM::ID, spec_uuids, op.Values().IsEmpty()};
        CALL(VarIntStream::FeedWord(spec_desc.Coded()));
        if (spec_uuids) {
            if (spec_uuids == 2) CALL(VarIntStream::FeedUuid(op.ID()));
            CALL(VarIntStream::FeedUuid(op.Ref()));
        }
    }
    if (!op.Values().IsEmpty()) CALL(FeedTuple(op.Values()));
    prev_id_ = op.ID();
    state_ = OP_END;
    DONE;
}

/** A streaming RONv parser; can not always invoke Load methods that
 *  expect several varints to be available.
 */
PROC RONvStream::DrainOp(Op& op) {
    // ( [id box] [value box]* | [spread box] )* [frame term]
    // TODO inline singular op reads
    // FIXME internalize op a la RONtStream
    auto& vi = *(VarIntStream*)this;
    using BD = BoxDescriptor;
    Uuid id;
    Integer ai;
    Float af;
    Codepoint cp;
    auto& cs = state_;
    uint8_t ab;
    do {
        switch (cs) {
            case HEAD_BOX:
                CALL(vi.DrainWord(box_.coded_));
                op.Clear();
                if (box_.Type() == ID) {  // the spec
                    switch (box_.Size()) {
                        case 0:
                            op.SetRef(prev_id_);
                            ++prev_id_;
                            op.SetID(prev_id_);
                            cs = box_.IsLast() ? HEAD_BOX : NEXT_BOX;
                            break;
                        case 1:
                            ++prev_id_;
                            op.SetID(prev_id_);
                            cs = SPEC_REF;
                            break;
                        case 2:
                            cs = SPEC_ID;
                            break;
                        default:
                            FAIL(BADRECORD.Ext(BASE_v));
                    }
                } else if (box_.IsEmpty()) {  // a term
                    cs = box_.Type() == ATOM(TERM::ARTEFACT) ? BLOB : HEAD_BOX;
                    prev_id_ = NIL;
                    // TODO maybe: clear the microcache
                    return ENDOFRAME.Ext(box_.Type());
                } else {  // tuple
                    op.SetRef(prev_id_);
                    ++prev_id_;
                    op.SetID(prev_id_);
                    cs = STATE(STATE::VALUE + box_.Type());
                }
                break;
            case NEXT_BOX:
                CALL(vi.DrainWord(box_.coded_));
                if (box_.Type() == STRING) {
                    op.Values().PushNewString();
                    if (box_.IsEmpty()) {
                        cs = box_.IsLast() ? HEAD_BOX : NEXT_BOX;
                        break;
                    }
                } else {
                    NEED(!box_.IsEmpty(), BADRECORDv);
                }
                cs = STATE(STATE::VALUE + box_.Type());
                break;
            case SPEC_ID:
                CALL(CheckDrainable(3));  // abuse the fact there is a term
                CALL(vi.DrainUuid(id));
                if (id.Variant() == Uuid::DELTA)
                    CALL(cache_.ResolveDelta(id, id));
                op.SetID(id);
                prev_id_ = id;
                cache_.Store(id);
                box_.Dec();
                cs = box_.IsEmpty() ? NEXT_BOX : SPEC_REF;
                break;
            case SPEC_REF:
                CALL(CheckDrainable(3));
                CALL(vi.DrainUuid(id));
                if (id.Variant() == Uuid::DELTA)
                    CALL(cache_.ResolveDelta(id, id));
                op.SetRef(id);
                cs = NEXT_BOX;
                break;
            case VALUE_ID:
                CALL(CheckDrainable(3));
                CALL(vi.DrainUuid(id));
                op.Values().PushUuid(id);
                box_.Dec();
                if (box_.IsEmpty()) cs = box_.IsLast() ? HEAD_BOX : NEXT_BOX;
                break;
            case VALUE_INT:
                CALL(vi.DrainInteger(ai));
                op.Values().PushInteger(ai);
                box_.Dec();
                if (box_.IsEmpty()) cs = box_.IsLast() ? HEAD_BOX : NEXT_BOX;
                break;
            case VALUE_STRING:
                CALL(vi.DrainCodepoint(cp));
                NEED(cp <= MAX_UNICODE_CODEPOINT, BADUNICODE.Ext(BASE_v));
                op.Values().PushCodepoint(cp);
                box_.Dec();
                if (box_.IsEmpty()) cs = box_.IsLast() ? HEAD_BOX : NEXT_BOX;
                break;
            case VALUE_FLOAT:
                CALL(DrainFlippedWord((Word&)af));
                op.Values().PushFloat(af);
                box_.Dec();
                if (box_.IsEmpty()) cs = box_.IsLast() ? HEAD_BOX : NEXT_BOX;
                break;
            case BLOB:
                FAIL(BADSTATE);  // FIXME special Status
            default:
                FAIL(BADSTATE.Ext(BASE_v));
        }
    } while (cs != HEAD_BOX);
    DONE;
}

}  // namespace ron
