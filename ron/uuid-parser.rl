#include "uuid.hpp"

namespace ron {

%% machine RONID;
%% write data;

PROC Uuid::ParseUuid(Uuid& id, span8c data) {
    auto p = data.begin(); // parsed
    auto pd = p; // token start
    auto pe = data.end(); // end of data
    auto eof = pe; // end of file
    int cs = 0;

    %%{
        include RONID "uuid-grammar.rl";
        main := RONID;
        write init;
        write exec;
    }%%
    NEED (cs>=RONID_first_final, BADSYNTAX.Ext(BASE_U));
    DONE;
}

}

