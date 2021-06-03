#include <iostream>
#include "ron-streams.hpp"

namespace ron {

%% machine RONt;
%% write data;

PROC RONtStream::DrainOpLong() {
    static constexpr int CSBLOB = MAX_INDEX + 8;
    static constexpr int CS_EOF = MAX_INDEX;
    static constexpr int CS_EOF_FACT = CS_EOF+TERM::FACT;
    static constexpr int CS_EOF_QUERY = CS_EOF+TERM::QUERY;
    static constexpr int CS_EOF_CLAIM = CS_EOF+TERM::ARTEFACT;

    Uuid &id = id_;
    Integer integer;
    Float ron_float;
    Codepoint &cp = cp_;
    Index ndx;

    auto &cs = state_;
    auto &tuple = values_;
    auto &spec = spec_;

    auto p = At() + advance_[0];
    auto &pc = from_;
    auto pd = At() + advance_[1];
    auto pe = Filled().end();
    auto eof = nullptr;

    if (cs<=0) {
        if (cs==0) {
            tuple.Clear();
            spec.Clear();
            %%write init;
        } else if (cs==-MAX_INDEX) {
            FAIL(BADSTATE);
        } else { // resumed parsing after NODATA
            cs = -cs;
        }
    } else {
        pc = p;
        if (term_!=PART) {
            return DrainEOF();
        } else {
            tuple.Clear();
            spec_.Next();
        }
    }

more_data:
    %%{
        include RONt "./ront-grammar.rl";
        main := RONt;
        write exec;
    }%%
    NEED(cs!=RONt_error, BADSYNTAXR);
    advance_[0] = p - At();
    advance_[1] = pd - At();
    if (p==pe) {
        // This may eat the last op in an unterminated frame,
        // like 3 in `@1-2-test 1, 2, 3,`
        // But, frames are atomic/transactional units, so an
        // unfinished frame MUST NOT be processed at all.
        // So, this is not an issue (see OPLOG COMMITS).
        cs = -cs; // if read fails, we may resume parsing later
        CALL(Feed());
        cs = -cs;
        pc = At();
        p = At() + advance_[0];
        pd = At() + advance_[1];
        pe = Filled().end();
        goto more_data;
    }
    DONE;
}

}

