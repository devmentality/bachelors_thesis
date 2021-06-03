#include "../utf8.hpp"

#include <iostream>

#include "../uuid.hpp"
#include "test.hpp"

using namespace ron;
using namespace std;

TEST(Codepoint, UTF8) {
    std::string latin{"ABC"};
    string32 latin_cp;
    ASSERT_EQ(ParseUtf8(latin_cp, latin), OK);
    ASSERT_EQ(latin[0], latin_cp[0]);
    ASSERT_EQ(latin[1], latin_cp[1]);
    ASSERT_EQ(latin[2], latin_cp[2]);

    std::string raw{"Юникод\t萬國碼"};
    string32 rawcp;
    ASSERT_EQ(ParseUtf8(rawcp, raw), OK);
    ASSERT_EQ(rawcp.size(), 10);
    ASSERT_EQ(rawcp[5], 0x0434);
    std::string copy;
    ASSERT_EQ(SaveUtf8(copy, rawcp), OK);
    ASSERT_EQ(copy, raw);

    std::string u{"\u0008\u0009\u000a"}, uEsc, uRaw;
    u.push_back(0);
    ASSERT_EQ(u.size(), 4);
    string32 cpu;
    ASSERT_EQ(ParseUtf8(cpu, u), OK);
    ASSERT_EQ(SaveUtf8(uRaw, cpu), OK);
    ASSERT_EQ(SaveUtf8Esc(uEsc, cpu), OK);
    ASSERT_EQ(uEsc, "\\b\\t\\n\\0");
    ASSERT_EQ(uRaw, u);
}

TEST(utf8string, Basic) {
    char abc[]{"ABC"};
    string ascii{abc};
    int k = 0;
    for (auto cp : ascii) {
        ASSERT_TRUE(cp == abc[k++]);
    }
    string multi{"Юникод\t萬國碼"};
    string32 utf32{};
    ASSERT_OK(ParseUtf8(utf32, as_span8c(multi)));
    auto i = utf32.begin();
    ASSERT_TRUE(*i == 0x42E);
    ++i;
    ASSERT_TRUE(*i == 0x43D);
    ++i;
    ASSERT_TRUE(*i == 0x438);
    ++i;
    ASSERT_TRUE(*i == 0x43A);
    ++i;
    ASSERT_TRUE(*i == 0x43E);
    ++i;
    ASSERT_TRUE(*i == 0x434);
    ++i;
    ASSERT_EQ((int)*i, ASCII::TAB);
    ++i;
    ASSERT_EQ((int)*i, 0x842c);
    ++i;
    ASSERT_EQ((int)*i, 0x570b);
    ++i;
    ASSERT_EQ((int)*i, 0x78bc);

    //    ASSERT_EQ(multi.back(), 0x78bc);
    //    multi.pop_back();
    //    ASSERT_EQ(multi.back(), 0x570b);
    //    multi.pop_back();
    //    ASSERT_EQ(multi.back(), 0x842c);
    //    multi.pop_back();
    //    ASSERT_EQ(multi.back(), ASCII::TAB);
    //    multi.pop_back();
    //    ASSERT_EQ(multi.back(), 0x434);
}
