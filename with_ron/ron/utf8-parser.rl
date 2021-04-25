#include "utf8.hpp"

namespace ron {

%% machine UTF8;
%% write data;

PROC ParseUtf8(string32& into, span8c utf8) {
    auto p = utf8.begin();
    auto pe = utf8.end();
    auto eof = pe;
    Codepoint cp = 0;
    int cs = 0;
    %%write init;
    %%{
        include UTF8 "./utf8-grammar.rl";

        action push{ into.push_back(cp); }

        UTF8STRING = (CODEPOINT %push)*;

        main := UTF8STRING;
        write exec;
    }%%
    NEED(cs>=UTF8_first_final, BADUNICODE);
    DONE;
}

}
