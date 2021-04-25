%%{

    machine RONt;
    alphtype unsigned char;

    include RONID "./uuid-grammar.rl";
    include UTF8 "./utf8-grammar.rl";

    action begin_uuid {
        pc = p;
    }
    action end_uuid {
        pc = p;
    }
    action end_value_uuid {
        tuple.PushUuid(id);
    }
    action end_id {
        spec.id = id;
    }
    action end_ref {
        spec.ref = id;
    }

    action begin_int {
        pc = p;
    }
    action end_int {
        CALL(DrainInteger(integer, p));
        tuple.PushInteger(integer);
    }
    action begin_string {
        pc = p;
        tuple.PushNewString();
    }
    action end_string {
        pc = p;
        //if (i>MAX_INDEX) return BADSYNTAXS;
    }
    action begin_float {
        pc = p;
    }
    action end_float {
        CALL(DrainFloat(ron_float, p));
        tuple.PushFloat(ron_float);
    }
    action end_cp {
        tuple.PushCodepoint(cp);
        pc = p;
    }
    action string_esc {
        CALL(DrainAsciiEscape(cp,p));
    }
    action string_uesc {
        CALL(DrainUnicodeEscape(cp,p));
    }

    action term {
        term_ = TERM(ABC[fc]);
        ++p;
        pc = p;
        state_ = ftargs;
        advance_[0] = advance_[1] = 0;
        DONE;
    }

    action newline {
        // TODO error reporting
    }

    action begin_spread_size {
        NEED(spec_.id.IsEvent(), BADSYNTAXE);
        pc = p;
    }
    action end_spread_size {
        CALL(DrainInteger(integer, p));
        NEED(integer>=2 && integer<=MAX_SPAN_SIZE, BADSYNTAXS);
        span_len_ = integer;
        spread_ = ANY;
    }

    action begin_string_spread {
        NEED(spec_.id.IsEvent(), BADSYNTAXE);
        pc = p;
    }
    action spread_cp_1 {
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
    action spread_cp {
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
    action end_string_spread {
        NEED(span_len_>=2 && span_len_<=MAX_SPAN_SIZE, BADSYNTAXS);
        tuple.TrimLastBox(1);
        spread_ = STRING;
    }

    action badsyntax {
        FAIL(BADSYNTAX);
    }

    WS = [ \r\t] | [\n] @newline | '#' [^\n]* '\n' ;

    # int64_t
    SGN = '-' | '+';
    NATURAL = [1-9] digit*;
    INT = ( '0' | SGN? NATURAL ) >begin_int %end_int;

    # 64-bit (double) float
    EXP = [eE] SGN? digit+;
    FLOAT = ( SGN? digit+ ( "." digit+ EXP? | EXP ) ) >begin_float %end_float;

    UUID = RONID >begin_uuid %end_uuid;

    UNQUOTED_ID = (UUID - INT - FLOAT) %end_value_uuid;
    QUOTED_ID = [{] RONID %end_value_uuid [}];

    # JSON-ey string
    HEX = [0-9a-fA-F];
    SURROGATES = [dD] [8-9a-fA-F] HEX{2};
    UNIESC = "\\u" (HEX{4} - SURROGATES) %string_uesc;
    ESC = "\\" [0aevnrt\\bf'/"] %string_esc;
    SAFE_CODEPOINT = CODEPOINT - (0x0..0x1f) - ['\n\r\\\0];
    CP = (UNIESC|ESC|SAFE_CODEPOINT);
    STRING = ['] ( (CP %end_cp)* ) >begin_string %end_string ['];

    # value atom (payload) - int, float, string, UUID
    UATOM = INT | FLOAT | UNQUOTED_ID; # unquoted atom
    QATOM = STRING | QUOTED_ID; # quoted atom

    # see Tuple::CoerceAtom()
    COERCED_ATOM = QATOM | UATOM;

    # op metadata
    ID = '@' WS* UUID %end_id;
    REF = ':' WS* UUID %end_ref;
    SPEC = ID WS* REF | ID | REF;

    # op spreads
    STRING_SPREAD = [(] WS* ['] %begin_string_spread CP %spread_cp_1 (CP %spread_cp)* ['] WS* [)] %end_string_spread;
    SPREAD_SIZE = [(] WS* (NATURAL >begin_spread_size %end_spread_size) WS* [)];

    # op term (PART/FACT/QUERY/CLAIM)
    TERM = [,;!?] @term;

    # RON op: an immutable unit of change
    OP = (
        (SPEC|UATOM)? ( WS* QATOM UATOM? | WS+ UATOM )* WS* SPREAD_SIZE? |
        SPEC? WS* STRING_SPREAD
        ) WS* TERM;

    # RON frame (open text coding)
    RONt = ( WS* OP )+ $!badsyntax;

}%%
