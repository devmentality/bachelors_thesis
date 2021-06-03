
#line 1 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
#include "utf8.hpp"

namespace ron {


#line 6 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"

#line 11 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
static const int UTF8_start = 7;
static const int UTF8_first_final = 7;
static const int UTF8_error = 0;

static const int UTF8_en_main = 7;


#line 7 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"

PROC ParseUtf8(string32& into, span8c utf8) {
    auto p = utf8.begin();
    auto pe = utf8.end();
    auto eof = pe;
    Codepoint cp = 0;
    int cs = 0;
    
#line 28 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	{
	cs = UTF8_start;
	}

#line 15 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
    
#line 35 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	{
	if ( p == pe )
		goto _test_eof;
	goto _resume;

_again:
	switch ( cs ) {
		case 7: goto st7;
		case 8: goto st8;
		case 0: goto st0;
		case 1: goto st1;
		case 2: goto st2;
		case 3: goto st3;
		case 9: goto st9;
		case 4: goto st4;
		case 5: goto st5;
		case 6: goto st6;
		case 10: goto st10;
	default: break;
	}

	if ( ++p == pe )
		goto _test_eof;
_resume:
	switch ( cs )
	{
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr8;
		} else if ( (*p) >= 128u )
			goto st0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto st0;
		} else if ( (*p) >= 240u )
			goto tr10;
	} else
		goto tr9;
	goto tr7;
tr0:
#line 7 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st8;
tr7:
#line 8 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p); }
	goto st8;
tr11:
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 8 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p); }
	goto st8;
tr15:
	cs = 8;
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 8 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p); }
	goto _again;
tr19:
	cs = 8;
#line 12 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 8 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p); }
	goto _again;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 134 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr12;
		} else if ( (*p) >= 128u )
			goto st0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto st0;
		} else if ( (*p) >= 240u )
			goto tr14;
	} else
		goto tr13;
	goto tr11;
st0:
cs = 0;
	goto _out;
tr8:
#line 9 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st1;
tr12:
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 9 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto st1;
tr16:
	cs = 1;
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 9 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto _again;
tr20:
	cs = 1;
#line 12 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 9 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&0x1f; }
	goto _again;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 202 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr0;
	goto st0;
tr9:
#line 10 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st2;
tr13:
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 10 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto st2;
tr17:
	cs = 2;
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 10 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto _again;
tr21:
	cs = 2;
#line 12 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 10 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&0xf; }
	goto _again;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 255 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr2;
	goto st0;
tr2:
#line 7 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 267 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr3;
	goto st0;
tr3:
#line 7 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 279 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr16;
		} else if ( (*p) >= 128u )
			goto st0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto st0;
		} else if ( (*p) >= 240u )
			goto tr18;
	} else
		goto tr17;
	goto tr15;
tr10:
#line 11 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st4;
tr14:
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 11 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto st4;
tr18:
	cs = 4;
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 11 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto _again;
tr22:
	cs = 4;
#line 12 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
#line 11 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (*p)&7; }
	goto _again;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 344 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr4;
	goto st0;
tr4:
#line 7 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 356 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr5;
	goto st0;
tr5:
#line 7 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 368 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto tr6;
	goto st0;
tr6:
#line 7 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{ cp = (cp<<6) | ((*p)&0x3f); }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 380 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr20;
		} else if ( (*p) >= 128u )
			goto st0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto st0;
		} else if ( (*p) >= 240u )
			goto tr22;
	} else
		goto tr21;
	goto tr19;
	}
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 8: 
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
	break;
	case 9: 
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; cs = 0; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
	break;
	case 10: 
#line 12 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>MAX_UNICODE_CODEPOINT) {
        cs = (0);
        {p++; cs = 0; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/./utf8-grammar.rl"
	{
    if (cp>=0xD800 && cp<0xE000) {
        cs = (0);
        {p++; cs = 0; goto _out;}
    }
}
#line 18 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"
	{ into.push_back(cp); }
	break;
#line 445 "/home/gritzko/Projects/ron/ron/utf8-parser.cxx"
	}
	}

	_out: {}
	}

#line 24 "/home/gritzko/Projects/ron/ron/utf8-parser.rl"

    NEED(cs>=UTF8_first_final, BADUNICODE);
    DONE;
}

}
