
#line 1 "/home/gritzko/Projects/ron/ron/uuid-parser.rl"
#include "uuid.hpp"

namespace ron {


#line 6 "/home/gritzko/Projects/ron/ron/uuid-parser.rl"

#line 11 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
static const int RONID_start = 1;
static const int RONID_first_final = 7;
static const int RONID_error = 0;

static const int RONID_en_main = 1;


#line 7 "/home/gritzko/Projects/ron/ron/uuid-parser.rl"

PROC Uuid::ParseUuid(Uuid& id, span8c data) {
    auto p = data.begin(); // parsed
    auto pb = p; // token start
    auto pe = data.end(); // end of data
    auto eof = pe; // end of file
    int cs = 0;

    
#line 29 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	{
	cs = RONID_start;
	}

#line 34 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 95u: goto tr0;
		case 126u: goto tr0;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr0;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr0;
	} else
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 6 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        pb = p;
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 67 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 43u: goto tr8;
		case 45u: goto tr9;
		case 94u: goto tr11;
		case 95u: goto st7;
		case 126u: goto st7;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st7;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st7;
	} else
		goto st7;
	goto st0;
tr8:
#line 15 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pb,p} ));
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 94 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr2;
		case 126u: goto tr2;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr2;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr2;
	} else
		goto tr2;
	goto st0;
tr2:
#line 6 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        pb = p;
    }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 118 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 95u: goto st8;
		case 126u: goto st8;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st8;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st8;
	} else
		goto st8;
	goto st0;
tr9:
#line 9 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.UpperIndex(), span8c{pb,p} ));
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 142 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr3;
		case 126u: goto tr3;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr3;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr3;
	} else
		goto tr3;
	goto st0;
tr3:
#line 6 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        pb = p;
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 166 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 45u: goto tr4;
		case 95u: goto st4;
		case 126u: goto st4;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st4;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st4;
	} else
		goto st4;
	goto st0;
tr4:
#line 12 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        CALL(Uuid::ParseIndex( id.LowerIndex(), span8c{pb,p} ));
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 191 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr6;
		case 126u: goto tr6;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr6;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr6;
	} else
		goto tr6;
	goto st0;
tr6:
#line 6 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        pb = p;
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 215 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 95u: goto st9;
		case 126u: goto st9;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st9;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st9;
	} else
		goto st9;
	goto st0;
tr11:
#line 15 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.first, span8c{pb,p} ));
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 239 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 95u: goto tr7;
		case 126u: goto tr7;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto tr7;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr7;
	} else
		goto tr7;
	goto st0;
tr7:
#line 6 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        pb = p;
    }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 263 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	switch( (*p) ) {
		case 95u: goto st10;
		case 126u: goto st10;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st10;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st10;
	} else
		goto st10;
	goto st0;
	}
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 9: 
#line 18 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pb,p} ));
        id.second |= YOLO_FLAGS;
    }
	break;
	case 8: 
#line 22 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pb,p} ));
        id.second |= LAMPORT_FLAGS;
    }
	break;
	case 10: 
#line 26 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        CALL(Uuid::ParseWord( id.second, span8c{pb,p} ));
        id.second |= TAG_FLAGS;
    }
	break;
	case 7: 
#line 30 "/home/gritzko/Projects/ron/ron/uuid-grammar.rl"
	{
        CALL(Uuid::ParseDouble( id, span8c{pb,p} ));
        id.second |= NAME_FLAGS;
    }
	break;
#line 320 "/home/gritzko/Projects/ron/ron/uuid-parser.cxx"
	}
	}

	_out: {}
	}

#line 20 "/home/gritzko/Projects/ron/ron/uuid-parser.rl"

    NEED (cs>=RONID_first_final, BADSYNTAX.Ext(BASE_U));
    DONE;
}

}

