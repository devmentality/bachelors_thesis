
#line 1 "/home/gritzko/Projects/ron/ron/ront-parser.rl"
#include <iostream>
#include "ron-streams.hpp"

namespace ron {


#line 7 "/home/gritzko/Projects/ron/ron/ront-parser.rl"

#line 12 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
static const int RONt_start = 1;
static const int RONt_first_final = 132;
static const int RONt_error = 0;

static const int RONt_en_main = 1;


#line 8 "/home/gritzko/Projects/ron/ron/ront-parser.rl"

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
            
#line 50 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	{
	cs = RONt_start;
	}

#line 37 "/home/gritzko/Projects/ron/ron/ront-parser.rl"
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
    
#line 73 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	{
	if ( p == pe )
		goto _test_eof;
	goto _resume;

_again:
	switch ( cs ) {
		case 1: goto st1;
		case 0: goto st0;
		case 132: goto st132;
		case 2: goto st2;
		case 3: goto st3;
		case 4: goto st4;
		case 5: goto st5;
		case 6: goto st6;
		case 7: goto st7;
		case 8: goto st8;
		case 9: goto st9;
		case 10: goto st10;
		case 11: goto st11;
		case 12: goto st12;
		case 13: goto st13;
		case 14: goto st14;
		case 15: goto st15;
		case 16: goto st16;
		case 17: goto st17;
		case 18: goto st18;
		case 19: goto st19;
		case 20: goto st20;
		case 21: goto st21;
		case 22: goto st22;
		case 23: goto st23;
		case 24: goto st24;
		case 25: goto st25;
		case 26: goto st26;
		case 27: goto st27;
		case 28: goto st28;
		case 29: goto st29;
		case 30: goto st30;
		case 31: goto st31;
		case 32: goto st32;
		case 33: goto st33;
		case 34: goto st34;
		case 35: goto st35;
		case 36: goto st36;
		case 37: goto st37;
		case 38: goto st38;
		case 39: goto st39;
		case 40: goto st40;
		case 41: goto st41;
		case 42: goto st42;
		case 43: goto st43;
		case 44: goto st44;
		case 45: goto st45;
		case 46: goto st46;
		case 47: goto st47;
		case 48: goto st48;
		case 49: goto st49;
		case 50: goto st50;
		case 51: goto st51;
		case 52: goto st52;
		case 53: goto st53;
		case 54: goto st54;
		case 55: goto st55;
		case 56: goto st56;
		case 57: goto st57;
		case 58: goto st58;
		case 59: goto st59;
		case 60: goto st60;
		case 61: goto st61;
		case 62: goto st62;
		case 63: goto st63;
		case 64: goto st64;
		case 65: goto st65;
		case 66: goto st66;
		case 67: goto st67;
		case 68: goto st68;
		case 69: goto st69;
		case 70: goto st70;
		case 71: goto st71;
		case 72: goto st72;
		case 73: goto st73;
		case 74: goto st74;
		case 75: goto st75;
		case 76: goto st76;
		case 77: goto st77;
		case 78: goto st78;
		case 79: goto st79;
		case 80: goto st80;
		case 81: goto st81;
		case 82: goto st82;
		case 83: goto st83;
		case 84: goto st84;
		case 85: goto st85;
		case 86: goto st86;
		case 87: goto st87;
		case 88: goto st88;
		case 89: goto st89;
		case 90: goto st90;
		case 91: goto st91;
		case 92: goto st92;
		case 93: goto st93;
		case 94: goto st94;
		case 95: goto st95;
		case 96: goto st96;
		case 97: goto st97;
		case 98: goto st98;
		case 99: goto st99;
		case 100: goto st100;
		case 101: goto st101;
		case 102: goto st102;
		case 103: goto st103;
		case 104: goto st104;
		case 105: goto st105;
		case 106: goto st106;
		case 107: goto st107;
		case 108: goto st108;
		case 109: goto st109;
		case 110: goto st110;
		case 111: goto st111;
		case 112: goto st112;
		case 113: goto st113;
		case 114: goto st114;
		case 115: goto st115;
		case 116: goto st116;
		case 117: goto st117;
		case 118: goto st118;
		case 119: goto st119;
		case 120: goto st120;
		case 121: goto st121;
		case 122: goto st122;
		case 123: goto st123;
		case 124: goto st124;
		case 125: goto st125;
		case 126: goto st126;
		case 127: goto st127;
		case 128: goto st128;
		case 129: goto st129;
		case 130: goto st130;
		case 131: goto st131;
	default: break;
	}

	if ( ++p == pe )
		goto _test_eof;
_resume:
	switch ( cs )
	{
tr2:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 232 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st1;
		case 10u: goto tr2;
		case 13u: goto st1;
		case 32u: goto st1;
		case 33u: goto tr3;
		case 35u: goto st2;
		case 39u: goto st3;
		case 40u: goto st66;
		case 44u: goto tr3;
		case 48u: goto tr8;
		case 58u: goto st106;
		case 59u: goto tr3;
		case 63u: goto tr3;
		case 64u: goto st119;
		case 95u: goto tr12;
		case 123u: goto st21;
		case 126u: goto tr12;
	}
	if ( (*p) < 49u ) {
		if ( 43u <= (*p) && (*p) <= 45u )
			goto tr7;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto tr12;
		} else if ( (*p) >= 65u )
			goto tr12;
	} else
		goto tr9;
	goto tr0;
tr0:
#line 103 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        FAIL(BADSYNTAX);
    }
	goto st0;
#line 270 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
st0:
cs = 0;
	goto _out;
tr3:
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr51:
#line 43 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainFloat(ron_float, p));
        tuple.PushFloat(ron_float);
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr78:
#line 28 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        tuple.PushInteger(integer);
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr92:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr104:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr112:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr120:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr197:
#line 97 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(span_len_>=2 && span_len_<=MAX_SPAN_SIZE, BADSYNTAXS);
        tuple.TrimLastBox(1);
        spread_ = STRING;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr278:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr293:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr305:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr314:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr325:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr341:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr354:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
tr364:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
#line 58 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        term_ = TERM(ABC[(*p)]);
        ++p;
        pc = p;
        state_ = 132;
        advance_[0] = advance_[1] = 0;
        DONE;
    }
	goto st132;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
#line 626 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st1;
		case 10u: goto tr2;
		case 13u: goto st1;
		case 32u: goto st1;
		case 33u: goto tr3;
		case 35u: goto st2;
		case 39u: goto st3;
		case 40u: goto st66;
		case 44u: goto tr3;
		case 48u: goto tr8;
		case 58u: goto st106;
		case 59u: goto tr3;
		case 63u: goto tr3;
		case 64u: goto st119;
		case 95u: goto tr12;
		case 123u: goto st21;
		case 126u: goto tr12;
	}
	if ( (*p) < 49u ) {
		if ( 43u <= (*p) && (*p) <= 45u )
			goto tr7;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto tr12;
		} else if ( (*p) >= 65u )
			goto tr12;
	} else
		goto tr9;
	goto tr0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10u )
		goto st1;
	goto st2;
tr53:
#line 43 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainFloat(ron_float, p));
        tuple.PushFloat(ron_float);
    }
	goto st3;
tr80:
#line 28 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        tuple.PushInteger(integer);
    }
	goto st3;
tr94:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st3;
tr106:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st3;
tr114:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st3;
tr122:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st3;
tr280:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st3;
tr295:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st3;
tr307:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st3;
tr316:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st3;
tr327:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st3;
tr343:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st3;
tr356:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st3;
tr366:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 863 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr15;
		case 92u: goto tr16;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr14;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr19;
		} else if ( (*p) >= 224u )
			goto tr18;
	} else
		goto tr17;
	goto tr0;
tr14:
#line 32 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        tuple.PushNewString();
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st4;
tr20:
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st4;
tr136:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st4;
tr142:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st4;
tr145:
	cs = 4;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto _again;
tr154:
	cs = 4;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto _again;
tr165:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 973 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr21;
		case 92u: goto tr22;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr20;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr25;
		} else if ( (*p) >= 224u )
			goto tr24;
	} else
		goto tr23;
	goto tr0;
tr27:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st5;
tr15:
#line 32 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        tuple.PushNewString();
    }
#line 36 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        //if (i>MAX_INDEX) return BADSYNTAXS;
    }
	goto st5;
tr21:
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 36 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        //if (i>MAX_INDEX) return BADSYNTAXS;
    }
	goto st5;
tr49:
#line 43 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainFloat(ron_float, p));
        tuple.PushFloat(ron_float);
    }
	goto st5;
tr50:
#line 43 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainFloat(ron_float, p));
        tuple.PushFloat(ron_float);
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st5;
tr63:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st5;
tr66:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st5;
tr72:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st5;
tr75:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st5;
tr76:
#line 28 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        tuple.PushInteger(integer);
    }
	goto st5;
tr77:
#line 28 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        tuple.PushInteger(integer);
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st5;
tr90:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st5;
tr91:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st5;
tr102:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st5;
tr103:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st5;
tr110:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st5;
tr111:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st5;
tr118:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st5;
tr119:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st5;
tr137:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 36 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        //if (i>MAX_INDEX) return BADSYNTAXS;
    }
	goto st5;
tr146:
	cs = 5;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 36 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        //if (i>MAX_INDEX) return BADSYNTAXS;
    }
	goto _again;
tr155:
	cs = 5;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 36 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        //if (i>MAX_INDEX) return BADSYNTAXS;
    }
	goto _again;
tr166:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 36 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        //if (i>MAX_INDEX) return BADSYNTAXS;
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1319 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st5;
		case 10u: goto tr27;
		case 13u: goto st5;
		case 32u: goto st5;
		case 33u: goto tr3;
		case 35u: goto st6;
		case 39u: goto st3;
		case 40u: goto st7;
		case 44u: goto tr3;
		case 48u: goto tr8;
		case 59u: goto tr3;
		case 63u: goto tr3;
		case 95u: goto tr12;
		case 123u: goto st21;
		case 126u: goto tr12;
	}
	if ( (*p) < 49u ) {
		if ( 43u <= (*p) && (*p) <= 45u )
			goto tr7;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto tr12;
		} else if ( (*p) >= 65u )
			goto tr12;
	} else
		goto tr9;
	goto tr0;
tr52:
#line 43 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainFloat(ron_float, p));
        tuple.PushFloat(ron_float);
    }
	goto st6;
tr79:
#line 28 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        tuple.PushInteger(integer);
    }
	goto st6;
tr93:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st6;
tr105:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st6;
tr113:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st6;
tr121:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1427 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( (*p) == 10u )
		goto st5;
	goto st6;
tr30:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st7;
tr54:
#line 43 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainFloat(ron_float, p));
        tuple.PushFloat(ron_float);
    }
	goto st7;
tr81:
#line 28 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        tuple.PushInteger(integer);
    }
	goto st7;
tr95:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st7;
tr107:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st7;
tr115:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st7;
tr123:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1515 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st7;
		case 10u: goto tr30;
		case 13u: goto st7;
		case 32u: goto st7;
		case 35u: goto st8;
	}
	if ( 49u <= (*p) && (*p) <= 57u )
		goto tr32;
	goto tr0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 10u )
		goto st7;
	goto st8;
tr32:
#line 71 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(spec_.id.IsEvent(), BADSYNTAXE);
        pc = p;
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 1544 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr33;
		case 10u: goto tr34;
		case 13u: goto tr33;
		case 32u: goto tr33;
		case 35u: goto tr35;
		case 41u: goto tr36;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st9;
	goto tr0;
tr39:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st10;
tr33:
#line 75 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        NEED(integer>=2 && integer<=MAX_SPAN_SIZE, BADSYNTAXS);
        span_len_ = integer;
        spread_ = ANY;
    }
	goto st10;
tr34:
#line 75 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        NEED(integer>=2 && integer<=MAX_SPAN_SIZE, BADSYNTAXS);
        span_len_ = integer;
        spread_ = ANY;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 1588 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st10;
		case 10u: goto tr39;
		case 13u: goto st10;
		case 32u: goto st10;
		case 35u: goto st11;
		case 41u: goto st12;
	}
	goto tr0;
tr35:
#line 75 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        NEED(integer>=2 && integer<=MAX_SPAN_SIZE, BADSYNTAXS);
        span_len_ = integer;
        spread_ = ANY;
    }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 1611 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( (*p) == 10u )
		goto st10;
	goto st11;
tr42:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st12;
tr36:
#line 75 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        NEED(integer>=2 && integer<=MAX_SPAN_SIZE, BADSYNTAXS);
        span_len_ = integer;
        spread_ = ANY;
    }
	goto st12;
tr195:
#line 97 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(span_len_>=2 && span_len_<=MAX_SPAN_SIZE, BADSYNTAXS);
        tuple.TrimLastBox(1);
        spread_ = STRING;
    }
	goto st12;
tr196:
#line 97 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(span_len_>=2 && span_len_<=MAX_SPAN_SIZE, BADSYNTAXS);
        tuple.TrimLastBox(1);
        spread_ = STRING;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 1654 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st12;
		case 10u: goto tr42;
		case 13u: goto st12;
		case 32u: goto st12;
		case 33u: goto tr3;
		case 35u: goto st13;
		case 44u: goto tr3;
		case 59u: goto tr3;
		case 63u: goto tr3;
	}
	goto tr0;
tr198:
#line 97 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(span_len_>=2 && span_len_<=MAX_SPAN_SIZE, BADSYNTAXS);
        tuple.TrimLastBox(1);
        spread_ = STRING;
    }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 1679 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( (*p) == 10u )
		goto st12;
	goto st13;
tr7:
#line 25 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 40 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 1697 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( (*p) == 48u )
		goto st15;
	if ( 49u <= (*p) && (*p) <= 57u )
		goto st31;
	goto tr0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 46u: goto st16;
		case 69u: goto st18;
		case 101u: goto st18;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st15;
	goto tr0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st17;
	goto tr0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 9u: goto tr49;
		case 10u: goto tr50;
		case 13u: goto tr49;
		case 32u: goto tr49;
		case 33u: goto tr51;
		case 35u: goto tr52;
		case 39u: goto tr53;
		case 40u: goto tr54;
		case 44u: goto tr51;
		case 59u: goto tr51;
		case 63u: goto tr51;
		case 69u: goto st18;
		case 101u: goto st18;
		case 123u: goto tr55;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st17;
	goto tr0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 43u: goto st19;
		case 45u: goto st19;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st20;
	goto tr0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st20;
	goto tr0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 9u: goto tr49;
		case 10u: goto tr50;
		case 13u: goto tr49;
		case 32u: goto tr49;
		case 33u: goto tr51;
		case 35u: goto tr52;
		case 39u: goto tr53;
		case 40u: goto tr54;
		case 44u: goto tr51;
		case 59u: goto tr51;
		case 63u: goto tr51;
		case 123u: goto tr55;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st20;
	goto tr0;
tr55:
#line 43 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainFloat(ron_float, p));
        tuple.PushFloat(ron_float);
    }
	goto st21;
tr82:
#line 28 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainInteger(integer, p));
        tuple.PushInteger(integer);
    }
	goto st21;
tr97:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st21;
tr109:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st21;
tr116:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st21;
tr125:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 15 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushUuid(id);
    }
	goto st21;
tr286:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st21;
tr298:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st21;
tr310:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st21;
tr319:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st21;
tr334:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st21;
tr347:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st21;
tr360:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st21;
tr370:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 1982 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr58;
		case 126u: goto tr58;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr58;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr58;
	} else
		goto tr58;
	goto tr0;
tr58:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 2006 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 43u: goto tr59;
		case 45u: goto tr60;
		case 94u: goto tr62;
		case 95u: goto st22;
		case 125u: goto tr63;
		case 126u: goto st22;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st22;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st22;
	} else
		goto st22;
	goto tr0;
tr59:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 2034 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr64;
		case 126u: goto tr64;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr64;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr64;
	} else
		goto tr64;
	goto tr0;
tr64:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 2058 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto st24;
		case 125u: goto tr66;
		case 126u: goto st24;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st24;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st24;
	} else
		goto st24;
	goto tr0;
tr60:
#line 9 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.UpperIndex(), span8c{pd,p} ));
    }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 2083 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr67;
		case 126u: goto tr67;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr67;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr67;
	} else
		goto tr67;
	goto tr0;
tr67:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 2107 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 45u: goto tr68;
		case 95u: goto st26;
		case 126u: goto st26;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st26;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st26;
	} else
		goto st26;
	goto tr0;
tr68:
#line 12 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.LowerIndex(), span8c{pd,p} ));
    }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 2132 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr70;
		case 126u: goto tr70;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr70;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr70;
	} else
		goto tr70;
	goto tr0;
tr70:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 2156 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto st28;
		case 125u: goto tr72;
		case 126u: goto st28;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st28;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st28;
	} else
		goto st28;
	goto tr0;
tr62:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 2181 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr73;
		case 126u: goto tr73;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr73;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr73;
	} else
		goto tr73;
	goto tr0;
tr73:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 2205 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto st30;
		case 125u: goto tr75;
		case 126u: goto st30;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st30;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st30;
	} else
		goto st30;
	goto tr0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 9u: goto tr76;
		case 10u: goto tr77;
		case 13u: goto tr76;
		case 32u: goto tr76;
		case 33u: goto tr78;
		case 35u: goto tr79;
		case 39u: goto tr80;
		case 40u: goto tr81;
		case 44u: goto tr78;
		case 46u: goto st16;
		case 59u: goto tr78;
		case 63u: goto tr78;
		case 69u: goto st18;
		case 101u: goto st18;
		case 123u: goto tr82;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st31;
	goto tr0;
tr8:
#line 25 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 40 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 2266 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr76;
		case 10u: goto tr77;
		case 13u: goto tr76;
		case 32u: goto tr76;
		case 33u: goto tr78;
		case 35u: goto tr79;
		case 39u: goto tr80;
		case 40u: goto tr81;
		case 43u: goto tr83;
		case 44u: goto tr78;
		case 45u: goto tr84;
		case 46u: goto st16;
		case 59u: goto tr78;
		case 63u: goto tr78;
		case 69u: goto st43;
		case 94u: goto tr88;
		case 95u: goto st40;
		case 101u: goto st43;
		case 123u: goto tr82;
		case 126u: goto st40;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st39;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st40;
	} else
		goto st40;
	goto tr0;
tr83:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 2308 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr89;
		case 126u: goto tr89;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr89;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr89;
	} else
		goto tr89;
	goto tr0;
tr89:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 2332 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr90;
		case 10u: goto tr91;
		case 13u: goto tr90;
		case 32u: goto tr90;
		case 33u: goto tr92;
		case 35u: goto tr93;
		case 39u: goto tr94;
		case 40u: goto tr95;
		case 44u: goto tr92;
		case 59u: goto tr92;
		case 63u: goto tr92;
		case 95u: goto st34;
		case 123u: goto tr97;
		case 126u: goto st34;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st34;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st34;
	} else
		goto st34;
	goto tr0;
tr84:
#line 9 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.UpperIndex(), span8c{pd,p} ));
    }
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 2368 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr98;
		case 126u: goto tr98;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr98;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr98;
	} else
		goto tr98;
	goto tr0;
tr98:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 2392 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 45u: goto tr99;
		case 95u: goto st36;
		case 126u: goto st36;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st36;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st36;
	} else
		goto st36;
	goto tr0;
tr99:
#line 12 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.LowerIndex(), span8c{pd,p} ));
    }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 2417 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr101;
		case 126u: goto tr101;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr101;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr101;
	} else
		goto tr101;
	goto tr0;
tr101:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 2441 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr102;
		case 10u: goto tr103;
		case 13u: goto tr102;
		case 32u: goto tr102;
		case 33u: goto tr104;
		case 35u: goto tr105;
		case 39u: goto tr106;
		case 40u: goto tr107;
		case 44u: goto tr104;
		case 59u: goto tr104;
		case 63u: goto tr104;
		case 95u: goto st38;
		case 123u: goto tr109;
		case 126u: goto st38;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st38;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st38;
	} else
		goto st38;
	goto tr0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	switch( (*p) ) {
		case 9u: goto tr110;
		case 10u: goto tr111;
		case 13u: goto tr110;
		case 32u: goto tr110;
		case 33u: goto tr112;
		case 35u: goto tr113;
		case 39u: goto tr114;
		case 40u: goto tr115;
		case 43u: goto tr83;
		case 44u: goto tr112;
		case 45u: goto tr84;
		case 46u: goto st16;
		case 59u: goto tr112;
		case 63u: goto tr112;
		case 69u: goto st43;
		case 94u: goto tr88;
		case 95u: goto st40;
		case 101u: goto st43;
		case 123u: goto tr116;
		case 126u: goto st40;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st39;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st40;
	} else
		goto st40;
	goto tr0;
tr12:
#line 9 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 2516 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr110;
		case 10u: goto tr111;
		case 13u: goto tr110;
		case 32u: goto tr110;
		case 33u: goto tr112;
		case 35u: goto tr113;
		case 39u: goto tr114;
		case 40u: goto tr115;
		case 43u: goto tr83;
		case 44u: goto tr112;
		case 45u: goto tr84;
		case 59u: goto tr112;
		case 63u: goto tr112;
		case 94u: goto tr88;
		case 95u: goto st40;
		case 123u: goto tr116;
		case 126u: goto st40;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st40;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st40;
	} else
		goto st40;
	goto tr0;
tr88:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 2555 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr117;
		case 126u: goto tr117;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr117;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr117;
	} else
		goto tr117;
	goto tr0;
tr117:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 2579 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr118;
		case 10u: goto tr119;
		case 13u: goto tr118;
		case 32u: goto tr118;
		case 33u: goto tr120;
		case 35u: goto tr121;
		case 39u: goto tr122;
		case 40u: goto tr123;
		case 44u: goto tr120;
		case 59u: goto tr120;
		case 63u: goto tr120;
		case 95u: goto st42;
		case 123u: goto tr125;
		case 126u: goto st42;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st42;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st42;
	} else
		goto st42;
	goto tr0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 9u: goto tr110;
		case 10u: goto tr111;
		case 13u: goto tr110;
		case 32u: goto tr110;
		case 33u: goto tr112;
		case 35u: goto tr113;
		case 39u: goto tr114;
		case 40u: goto tr115;
		case 43u: goto tr126;
		case 44u: goto tr112;
		case 45u: goto tr127;
		case 59u: goto tr112;
		case 63u: goto tr112;
		case 94u: goto tr88;
		case 95u: goto st40;
		case 123u: goto tr116;
		case 126u: goto st40;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st48;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st40;
	} else
		goto st40;
	goto tr0;
tr126:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 2647 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr89;
		case 126u: goto tr89;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr129;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr89;
	} else
		goto tr89;
	goto tr0;
tr129:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 2671 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr49;
		case 10u: goto tr50;
		case 13u: goto tr49;
		case 32u: goto tr49;
		case 33u: goto tr51;
		case 35u: goto tr52;
		case 39u: goto tr53;
		case 40u: goto tr54;
		case 44u: goto tr51;
		case 59u: goto tr51;
		case 63u: goto tr51;
		case 95u: goto st34;
		case 123u: goto tr55;
		case 126u: goto st34;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st45;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st34;
	} else
		goto st34;
	goto tr0;
tr127:
#line 9 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.UpperIndex(), span8c{pd,p} ));
    }
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 2707 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr98;
		case 126u: goto tr98;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr131;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr98;
	} else
		goto tr98;
	goto tr0;
tr131:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 2731 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr49;
		case 10u: goto tr50;
		case 13u: goto tr49;
		case 32u: goto tr49;
		case 33u: goto tr51;
		case 35u: goto tr52;
		case 39u: goto tr53;
		case 40u: goto tr54;
		case 44u: goto tr51;
		case 45u: goto tr99;
		case 59u: goto tr51;
		case 63u: goto tr51;
		case 95u: goto st36;
		case 123u: goto tr55;
		case 126u: goto st36;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st47;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st36;
	} else
		goto st36;
	goto tr0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 9u: goto tr49;
		case 10u: goto tr50;
		case 13u: goto tr49;
		case 32u: goto tr49;
		case 33u: goto tr51;
		case 35u: goto tr52;
		case 39u: goto tr53;
		case 40u: goto tr54;
		case 43u: goto tr83;
		case 44u: goto tr51;
		case 45u: goto tr84;
		case 59u: goto tr51;
		case 63u: goto tr51;
		case 94u: goto tr88;
		case 95u: goto st40;
		case 123u: goto tr55;
		case 126u: goto st40;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st48;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st40;
	} else
		goto st40;
	goto tr0;
tr9:
#line 25 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 40 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 2812 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr76;
		case 10u: goto tr77;
		case 13u: goto tr76;
		case 32u: goto tr76;
		case 33u: goto tr78;
		case 35u: goto tr79;
		case 39u: goto tr80;
		case 40u: goto tr81;
		case 43u: goto tr83;
		case 44u: goto tr78;
		case 45u: goto tr84;
		case 46u: goto st16;
		case 59u: goto tr78;
		case 63u: goto tr78;
		case 69u: goto st43;
		case 94u: goto tr88;
		case 95u: goto st40;
		case 101u: goto st43;
		case 123u: goto tr82;
		case 126u: goto st40;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st49;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st40;
	} else
		goto st40;
	goto tr0;
tr16:
#line 32 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        tuple.PushNewString();
    }
	goto st50;
tr22:
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
	goto st50;
tr138:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
	goto st50;
tr147:
	cs = 50;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
	goto _again;
tr156:
	cs = 50;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
	goto _again;
tr167:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 2921 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 34u: goto st51;
		case 39u: goto st51;
		case 92u: goto st51;
		case 110u: goto st51;
		case 114u: goto st51;
		case 117u: goto st60;
	}
	if ( (*p) < 97u ) {
		if ( 47u <= (*p) && (*p) <= 48u )
			goto st51;
	} else if ( (*p) > 98u ) {
		if ( (*p) > 102u ) {
			if ( 116u <= (*p) && (*p) <= 118u )
				goto st51;
		} else if ( (*p) >= 101u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 39u: goto tr137;
		case 92u: goto tr138;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr136;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr141;
		} else if ( (*p) >= 224u )
			goto tr140;
	} else
		goto tr139;
	goto tr0;
tr17:
#line 32 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        tuple.PushNewString();
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st52;
tr23:
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st52;
tr139:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st52;
tr148:
	cs = 52;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto _again;
tr157:
	cs = 52;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto _again;
tr168:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 3051 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr142;
	goto tr0;
tr18:
#line 32 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        tuple.PushNewString();
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st53;
tr24:
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st53;
tr140:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st53;
tr149:
	cs = 53;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto _again;
tr158:
	cs = 53;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto _again;
tr169:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 3144 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr143;
	goto tr0;
tr143:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 3156 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr144;
	goto tr0;
tr144:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 3168 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr146;
		case 92u: goto tr147;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr145;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr150;
		} else if ( (*p) >= 224u )
			goto tr149;
	} else
		goto tr148;
	goto tr0;
tr19:
#line 32 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
        tuple.PushNewString();
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st56;
tr25:
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st56;
tr141:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st56;
tr150:
	cs = 56;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto _again;
tr159:
	cs = 56;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto _again;
tr170:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 47 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 3274 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr151;
	goto tr0;
tr151:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st57;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
#line 3286 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr152;
	goto tr0;
tr152:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 3298 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr153;
	goto tr0;
tr153:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st59;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
#line 3310 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr155;
		case 92u: goto tr156;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr154;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr159;
		} else if ( (*p) >= 224u )
			goto tr158;
	} else
		goto tr157;
	goto tr0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 68u: goto st65;
		case 100u: goto st65;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st61;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st61;
	} else
		goto st61;
	goto tr0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st62;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st62;
	} else
		goto st62;
	goto tr0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st63;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st63;
	} else
		goto st63;
	goto tr0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st64;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st64;
	} else
		goto st64;
	goto tr0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 39u: goto tr166;
		case 92u: goto tr167;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr165;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr170;
		} else if ( (*p) >= 224u )
			goto tr169;
	} else
		goto tr168;
	goto tr0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( 48u <= (*p) && (*p) <= 55u )
		goto st62;
	goto tr0;
tr171:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st66;
tr281:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st66;
tr296:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st66;
tr308:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st66;
tr317:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st66;
tr328:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st66;
tr344:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st66;
tr357:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st66;
tr367:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st66;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
#line 3540 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st66;
		case 10u: goto tr171;
		case 13u: goto st66;
		case 32u: goto st66;
		case 35u: goto st67;
		case 39u: goto st68;
	}
	if ( 49u <= (*p) && (*p) <= 57u )
		goto tr32;
	goto tr0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 10u )
		goto st66;
	goto st67;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 92u )
		goto tr175;
	if ( (*p) < 192u ) {
		if ( (*p) > 38u ) {
			if ( 40u <= (*p) && (*p) <= 127u )
				goto tr174;
		} else if ( (*p) >= 32u )
			goto tr174;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr178;
		} else if ( (*p) >= 224u )
			goto tr177;
	} else
		goto tr176;
	goto tr0;
tr255:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st69;
tr174:
#line 82 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(spec_.id.IsEvent(), BADSYNTAXE);
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 3597 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr180;
		case 92u: goto tr181;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr179;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr184;
		} else if ( (*p) >= 224u )
			goto tr183;
	} else
		goto tr182;
	goto tr0;
tr207:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st70;
tr179:
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st70;
tr185:
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st70;
tr201:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st70;
tr210:
	cs = 70;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto _again;
tr219:
	cs = 70;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto _again;
tr230:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st70;
tr238:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st70;
tr249:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto st70;
tr258:
	cs = 70;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto _again;
tr267:
	cs = 70;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 8 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p); }
	goto _again;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
#line 3789 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr186;
		case 92u: goto tr187;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr185;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr190;
		} else if ( (*p) >= 224u )
			goto tr189;
	} else
		goto tr188;
	goto tr0;
tr192:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st71;
tr180:
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto st71;
tr186:
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto st71;
tr202:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto st71;
tr211:
	cs = 71;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto _again;
tr220:
	cs = 71;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto _again;
tr231:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto st71;
tr239:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto st71;
tr250:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto st71;
tr259:
	cs = 71;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto _again;
tr268:
	cs = 71;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto _again;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 3963 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st71;
		case 10u: goto tr192;
		case 13u: goto st71;
		case 32u: goto st71;
		case 35u: goto st72;
		case 41u: goto st73;
	}
	goto tr0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 10u )
		goto st71;
	goto st72;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 9u: goto tr195;
		case 10u: goto tr196;
		case 13u: goto tr195;
		case 32u: goto tr195;
		case 33u: goto tr197;
		case 35u: goto tr198;
		case 44u: goto tr197;
		case 59u: goto tr197;
		case 63u: goto tr197;
	}
	goto tr0;
tr181:
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto st74;
tr187:
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto st74;
tr203:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto st74;
tr212:
	cs = 74;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto _again;
tr221:
	cs = 74;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto _again;
tr232:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
	goto st74;
tr240:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto st74;
tr251:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto st74;
tr260:
	cs = 74;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto _again;
tr269:
	cs = 74;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
	goto _again;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 4147 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 34u: goto st75;
		case 39u: goto st75;
		case 92u: goto st75;
		case 110u: goto st75;
		case 114u: goto st75;
		case 117u: goto st84;
	}
	if ( (*p) < 97u ) {
		if ( 47u <= (*p) && (*p) <= 48u )
			goto st75;
	} else if ( (*p) > 98u ) {
		if ( (*p) > 102u ) {
			if ( 116u <= (*p) && (*p) <= 118u )
				goto st75;
		} else if ( (*p) >= 101u )
			goto st75;
	} else
		goto st75;
	goto tr0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 39u: goto tr202;
		case 92u: goto tr203;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr201;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr206;
		} else if ( (*p) >= 224u )
			goto tr205;
	} else
		goto tr204;
	goto tr0;
tr182:
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st76;
tr188:
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st76;
tr204:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st76;
tr213:
	cs = 76;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto _again;
tr222:
	cs = 76;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto _again;
tr233:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st76;
tr241:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st76;
tr252:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st76;
tr261:
	cs = 76;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto _again;
tr270:
	cs = 76;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto _again;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 4359 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr207;
	goto tr0;
tr183:
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st77;
tr189:
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st77;
tr205:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st77;
tr214:
	cs = 77;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto _again;
tr223:
	cs = 77;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto _again;
tr234:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st77;
tr242:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st77;
tr253:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st77;
tr262:
	cs = 77;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto _again;
tr271:
	cs = 77;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto _again;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 4534 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr208;
	goto tr0;
tr208:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 4546 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr209;
	goto tr0;
tr209:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 4558 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr211;
		case 92u: goto tr212;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr210;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr215;
		} else if ( (*p) >= 224u )
			goto tr214;
	} else
		goto tr213;
	goto tr0;
tr184:
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st80;
tr190:
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st80;
tr206:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st80;
tr215:
	cs = 80;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto _again;
tr224:
	cs = 80;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto _again;
tr235:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 92 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        ++span_len_;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st80;
tr243:
#line 51 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainAsciiEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st80;
tr254:
#line 54 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        CALL(DrainUnicodeEscape(cp,p));
    }
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st80;
tr263:
	cs = 80;
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto _again;
tr272:
	cs = 80;
#line 12 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 86 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        tuple.PushCodepoint(cp);
        tuple.PushCodepoint(0);
        span_len_ = 1;
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto _again;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 4746 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr216;
	goto tr0;
tr216:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 4758 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr217;
	goto tr0;
tr217:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 4770 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr218;
	goto tr0;
tr218:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 4782 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr220;
		case 92u: goto tr221;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr219;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr224;
		} else if ( (*p) >= 224u )
			goto tr223;
	} else
		goto tr222;
	goto tr0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 68u: goto st89;
		case 100u: goto st89;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st85;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st85;
	} else
		goto st85;
	goto tr0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st86;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st86;
	} else
		goto st86;
	goto tr0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st87;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st87;
	} else
		goto st87;
	goto tr0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st88;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st88;
	} else
		goto st88;
	goto tr0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 39u: goto tr231;
		case 92u: goto tr232;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr230;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr235;
		} else if ( (*p) >= 224u )
			goto tr234;
	} else
		goto tr233;
	goto tr0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( 48u <= (*p) && (*p) <= 55u )
		goto st86;
	goto tr0;
tr175:
#line 82 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(spec_.id.IsEvent(), BADSYNTAXE);
        pc = p;
    }
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
#line 4893 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 34u: goto st91;
		case 39u: goto st91;
		case 92u: goto st91;
		case 110u: goto st91;
		case 114u: goto st91;
		case 117u: goto st92;
	}
	if ( (*p) < 97u ) {
		if ( 47u <= (*p) && (*p) <= 48u )
			goto st91;
	} else if ( (*p) > 98u ) {
		if ( (*p) > 102u ) {
			if ( 116u <= (*p) && (*p) <= 118u )
				goto st91;
		} else if ( (*p) >= 101u )
			goto st91;
	} else
		goto st91;
	goto tr0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 39u: goto tr239;
		case 92u: goto tr240;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr238;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr243;
		} else if ( (*p) >= 224u )
			goto tr242;
	} else
		goto tr241;
	goto tr0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 68u: goto st97;
		case 100u: goto st97;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st93;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st93;
	} else
		goto st93;
	goto tr0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st94;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st94;
	} else
		goto st94;
	goto tr0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st95;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st95;
	} else
		goto st95;
	goto tr0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st96;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st96;
	} else
		goto st96;
	goto tr0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 39u: goto tr250;
		case 92u: goto tr251;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr249;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr254;
		} else if ( (*p) >= 224u )
			goto tr253;
	} else
		goto tr252;
	goto tr0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( 48u <= (*p) && (*p) <= 55u )
		goto st94;
	goto tr0;
tr176:
#line 82 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(spec_.id.IsEvent(), BADSYNTAXE);
        pc = p;
    }
#line 9 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
#line 5030 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr255;
	goto tr0;
tr177:
#line 82 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(spec_.id.IsEvent(), BADSYNTAXE);
        pc = p;
    }
#line 10 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
#line 5047 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr256;
	goto tr0;
tr256:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
#line 5059 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr257;
	goto tr0;
tr257:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st101;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
#line 5071 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr259;
		case 92u: goto tr260;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr258;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr263;
		} else if ( (*p) >= 224u )
			goto tr262;
	} else
		goto tr261;
	goto tr0;
tr178:
#line 82 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        NEED(spec_.id.IsEvent(), BADSYNTAXE);
        pc = p;
    }
#line 11 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st102;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
#line 5101 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr264;
	goto tr0;
tr264:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st103;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
#line 5113 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr265;
	goto tr0;
tr265:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st104;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
#line 5125 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr266;
	goto tr0;
tr266:
#line 7 "/home/gritzko/Projects/ron/ron/././utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st105;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
#line 5137 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 39u: goto tr268;
		case 92u: goto tr269;
	}
	if ( (*p) < 192u ) {
		if ( 32u <= (*p) && (*p) <= 127u )
			goto tr267;
	} else if ( (*p) > 223u ) {
		if ( (*p) > 239u ) {
			if ( 240u <= (*p) && (*p) <= 247u )
				goto tr272;
		} else if ( (*p) >= 224u )
			goto tr271;
	} else
		goto tr270;
	goto tr0;
tr273:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st106;
tr332:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st106;
tr346:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st106;
tr359:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st106;
tr369:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st106;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
#line 5224 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st106;
		case 10u: goto tr273;
		case 13u: goto st106;
		case 32u: goto st106;
		case 35u: goto st107;
		case 95u: goto tr275;
		case 126u: goto tr275;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr275;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr275;
	} else
		goto tr275;
	goto tr0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	if ( (*p) == 10u )
		goto st106;
	goto st107;
tr275:
#line 9 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st108;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
#line 5264 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr276;
		case 10u: goto tr277;
		case 13u: goto tr276;
		case 32u: goto tr276;
		case 33u: goto tr278;
		case 35u: goto tr279;
		case 39u: goto tr280;
		case 40u: goto tr281;
		case 43u: goto tr282;
		case 44u: goto tr278;
		case 45u: goto tr283;
		case 59u: goto tr278;
		case 63u: goto tr278;
		case 94u: goto tr285;
		case 95u: goto st108;
		case 123u: goto tr286;
		case 126u: goto st108;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st108;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st108;
	} else
		goto st108;
	goto tr0;
tr288:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st109;
tr276:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st109;
tr277:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st109;
tr291:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st109;
tr292:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st109;
tr303:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st109;
tr304:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st109;
tr312:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st109;
tr313:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st109;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
#line 5439 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st109;
		case 10u: goto tr288;
		case 13u: goto st109;
		case 32u: goto st109;
		case 33u: goto tr3;
		case 35u: goto st110;
		case 39u: goto st3;
		case 40u: goto st66;
		case 44u: goto tr3;
		case 48u: goto tr8;
		case 59u: goto tr3;
		case 63u: goto tr3;
		case 95u: goto tr12;
		case 123u: goto st21;
		case 126u: goto tr12;
	}
	if ( (*p) < 49u ) {
		if ( 43u <= (*p) && (*p) <= 45u )
			goto tr7;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto tr12;
		} else if ( (*p) >= 65u )
			goto tr12;
	} else
		goto tr9;
	goto tr0;
tr279:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st110;
tr294:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st110;
tr306:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st110;
tr315:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 21 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.ref = id;
    }
	goto st110;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
#line 5533 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( (*p) == 10u )
		goto st109;
	goto st110;
tr282:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st111;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
#line 5547 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr290;
		case 126u: goto tr290;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr290;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr290;
	} else
		goto tr290;
	goto tr0;
tr290:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st112;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
#line 5571 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr291;
		case 10u: goto tr292;
		case 13u: goto tr291;
		case 32u: goto tr291;
		case 33u: goto tr293;
		case 35u: goto tr294;
		case 39u: goto tr295;
		case 40u: goto tr296;
		case 44u: goto tr293;
		case 59u: goto tr293;
		case 63u: goto tr293;
		case 95u: goto st112;
		case 123u: goto tr298;
		case 126u: goto st112;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st112;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st112;
	} else
		goto st112;
	goto tr0;
tr283:
#line 9 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.UpperIndex(), span8c{pd,p} ));
    }
	goto st113;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
#line 5607 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr299;
		case 126u: goto tr299;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr299;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr299;
	} else
		goto tr299;
	goto tr0;
tr299:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st114;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
#line 5631 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 45u: goto tr300;
		case 95u: goto st114;
		case 126u: goto st114;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st114;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st114;
	} else
		goto st114;
	goto tr0;
tr300:
#line 12 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.LowerIndex(), span8c{pd,p} ));
    }
	goto st115;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
#line 5656 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr302;
		case 126u: goto tr302;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr302;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr302;
	} else
		goto tr302;
	goto tr0;
tr302:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st116;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
#line 5680 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr303;
		case 10u: goto tr304;
		case 13u: goto tr303;
		case 32u: goto tr303;
		case 33u: goto tr305;
		case 35u: goto tr306;
		case 39u: goto tr307;
		case 40u: goto tr308;
		case 44u: goto tr305;
		case 59u: goto tr305;
		case 63u: goto tr305;
		case 95u: goto st116;
		case 123u: goto tr310;
		case 126u: goto st116;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st116;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st116;
	} else
		goto st116;
	goto tr0;
tr285:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st117;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
#line 5716 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr311;
		case 126u: goto tr311;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr311;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr311;
	} else
		goto tr311;
	goto tr0;
tr311:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st118;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
#line 5740 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr312;
		case 10u: goto tr313;
		case 13u: goto tr312;
		case 32u: goto tr312;
		case 33u: goto tr314;
		case 35u: goto tr315;
		case 39u: goto tr316;
		case 40u: goto tr317;
		case 44u: goto tr314;
		case 59u: goto tr314;
		case 63u: goto tr314;
		case 95u: goto st118;
		case 123u: goto tr319;
		case 126u: goto st118;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st118;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st118;
	} else
		goto st118;
	goto tr0;
tr320:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st119;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
#line 5776 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st119;
		case 10u: goto tr320;
		case 13u: goto st119;
		case 32u: goto st119;
		case 35u: goto st120;
		case 95u: goto tr322;
		case 126u: goto tr322;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr322;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr322;
	} else
		goto tr322;
	goto tr0;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	if ( (*p) == 10u )
		goto st119;
	goto st120;
tr322:
#line 9 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st121;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
#line 5816 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr323;
		case 10u: goto tr324;
		case 13u: goto tr323;
		case 32u: goto tr323;
		case 33u: goto tr325;
		case 35u: goto tr326;
		case 39u: goto tr327;
		case 40u: goto tr328;
		case 43u: goto tr329;
		case 44u: goto tr325;
		case 45u: goto tr330;
		case 58u: goto tr332;
		case 59u: goto tr325;
		case 63u: goto tr325;
		case 94u: goto tr333;
		case 95u: goto st121;
		case 123u: goto tr334;
		case 126u: goto st121;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st121;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st121;
	} else
		goto st121;
	goto tr0;
tr336:
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st122;
tr323:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st122;
tr324:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st122;
tr339:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st122;
tr340:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st122;
tr352:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st122;
tr353:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st122;
tr362:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st122;
tr363:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
#line 67 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        // TODO error reporting
    }
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
#line 5992 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto st122;
		case 10u: goto tr336;
		case 13u: goto st122;
		case 32u: goto st122;
		case 33u: goto tr3;
		case 35u: goto st123;
		case 39u: goto st3;
		case 40u: goto st66;
		case 44u: goto tr3;
		case 48u: goto tr8;
		case 58u: goto st106;
		case 59u: goto tr3;
		case 63u: goto tr3;
		case 95u: goto tr12;
		case 123u: goto st21;
		case 126u: goto tr12;
	}
	if ( (*p) < 49u ) {
		if ( 43u <= (*p) && (*p) <= 45u )
			goto tr7;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto tr12;
		} else if ( (*p) >= 65u )
			goto tr12;
	} else
		goto tr9;
	goto tr0;
tr326:
#line 30 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st123;
tr342:
#line 22 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st123;
tr355:
#line 18 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st123;
tr365:
#line 26 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
#line 12 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        pc = p;
    }
#line 18 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        spec.id = id;
    }
	goto st123;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
#line 6087 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	if ( (*p) == 10u )
		goto st122;
	goto st123;
tr329:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st124;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
#line 6101 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr338;
		case 126u: goto tr338;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr338;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr338;
	} else
		goto tr338;
	goto tr0;
tr338:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st125;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
#line 6125 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr339;
		case 10u: goto tr340;
		case 13u: goto tr339;
		case 32u: goto tr339;
		case 33u: goto tr341;
		case 35u: goto tr342;
		case 39u: goto tr343;
		case 40u: goto tr344;
		case 44u: goto tr341;
		case 58u: goto tr346;
		case 59u: goto tr341;
		case 63u: goto tr341;
		case 95u: goto st125;
		case 123u: goto tr347;
		case 126u: goto st125;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st125;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st125;
	} else
		goto st125;
	goto tr0;
tr330:
#line 9 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.UpperIndex(), span8c{pd,p} ));
    }
	goto st126;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
#line 6162 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr348;
		case 126u: goto tr348;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr348;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr348;
	} else
		goto tr348;
	goto tr0;
tr348:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st127;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
#line 6186 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 45u: goto tr349;
		case 95u: goto st127;
		case 126u: goto st127;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st127;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st127;
	} else
		goto st127;
	goto tr0;
tr349:
#line 12 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.LowerIndex(), span8c{pd,p} ));
    }
	goto st128;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
#line 6211 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr351;
		case 126u: goto tr351;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr351;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr351;
	} else
		goto tr351;
	goto tr0;
tr351:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st129;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
#line 6235 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr352;
		case 10u: goto tr353;
		case 13u: goto tr352;
		case 32u: goto tr352;
		case 33u: goto tr354;
		case 35u: goto tr355;
		case 39u: goto tr356;
		case 40u: goto tr357;
		case 44u: goto tr354;
		case 58u: goto tr359;
		case 59u: goto tr354;
		case 63u: goto tr354;
		case 95u: goto st129;
		case 123u: goto tr360;
		case 126u: goto st129;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st129;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st129;
	} else
		goto st129;
	goto tr0;
tr333:
#line 15 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
	goto st130;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
#line 6272 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr361;
		case 126u: goto tr361;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr361;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr361;
	} else
		goto tr361;
	goto tr0;
tr361:
#line 6 "/home/gritzko/Projects/ron/ron/././uuid-grammar.rl"
	{
        pd = p;
    }
	goto st131;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
#line 6296 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	switch( (*p) ) {
		case 9u: goto tr362;
		case 10u: goto tr363;
		case 13u: goto tr362;
		case 32u: goto tr362;
		case 33u: goto tr364;
		case 35u: goto tr365;
		case 39u: goto tr366;
		case 40u: goto tr367;
		case 44u: goto tr364;
		case 58u: goto tr369;
		case 59u: goto tr364;
		case 63u: goto tr364;
		case 95u: goto st131;
		case 123u: goto tr370;
		case 126u: goto st131;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st131;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st131;
	} else
		goto st131;
	goto tr0;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 1: 
	case 2: 
	case 3: 
	case 4: 
	case 5: 
	case 6: 
	case 7: 
	case 8: 
	case 9: 
	case 10: 
	case 11: 
	case 12: 
	case 13: 
	case 14: 
	case 15: 
	case 16: 
	case 17: 
	case 18: 
	case 19: 
	case 20: 
	case 21: 
	case 22: 
	case 23: 
	case 24: 
	case 25: 
	case 26: 
	case 27: 
	case 28: 
	case 29: 
	case 30: 
	case 31: 
	case 32: 
	case 33: 
	case 34: 
	case 35: 
	case 36: 
	case 37: 
	case 38: 
	case 39: 
	case 40: 
	case 41: 
	case 42: 
	case 43: 
	case 44: 
	case 45: 
	case 46: 
	case 47: 
	case 48: 
	case 49: 
	case 50: 
	case 51: 
	case 52: 
	case 53: 
	case 54: 
	case 55: 
	case 56: 
	case 57: 
	case 58: 
	case 59: 
	case 60: 
	case 61: 
	case 62: 
	case 63: 
	case 64: 
	case 65: 
	case 66: 
	case 67: 
	case 68: 
	case 69: 
	case 70: 
	case 71: 
	case 72: 
	case 73: 
	case 74: 
	case 75: 
	case 76: 
	case 77: 
	case 78: 
	case 79: 
	case 80: 
	case 81: 
	case 82: 
	case 83: 
	case 84: 
	case 85: 
	case 86: 
	case 87: 
	case 88: 
	case 89: 
	case 90: 
	case 91: 
	case 92: 
	case 93: 
	case 94: 
	case 95: 
	case 96: 
	case 97: 
	case 98: 
	case 99: 
	case 100: 
	case 101: 
	case 102: 
	case 103: 
	case 104: 
	case 105: 
	case 106: 
	case 107: 
	case 108: 
	case 109: 
	case 110: 
	case 111: 
	case 112: 
	case 113: 
	case 114: 
	case 115: 
	case 116: 
	case 117: 
	case 118: 
	case 119: 
	case 120: 
	case 121: 
	case 122: 
	case 123: 
	case 124: 
	case 125: 
	case 126: 
	case 127: 
	case 128: 
	case 129: 
	case 130: 
	case 131: 
#line 103 "/home/gritzko/Projects/ron/ron/./ront-grammar.rl"
	{
        FAIL(BADSYNTAX);
    }
	break;
#line 6597 "/home/gritzko/Projects/ron/ron/ront-parser.cxx"
	}
	}

	_out: {}
	}

#line 57 "/home/gritzko/Projects/ron/ron/ront-parser.rl"

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

