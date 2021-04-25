#ifndef RON_TEST_HPP
#define RON_TEST_HPP
#include <ftw.h>
#define GTEST_DONT_DEFINE_FAIL 1
#include <gtest/gtest.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

#include "../uuid.hpp"

#define DEBUG 1
#define QUITE_A_FEW 100000

#define ASSERT_OK(x) \
    { ASSERT_EQ(Word2String(x._s), "0"); }

#define ASSERT_SAME(x,y) \
    { ASSERT_EQ(x.TrimExt().String(), y.TrimExt().String()); }

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag,
              struct FTW *ftwbuf) {
    int rv = remove(fpath);

    if (rv) perror(fpath);

    return rv;
}

int rmrf(char *path) { return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS); }

class TmpDir {
    char *name_;
    char *cwd_;

   public:
    TmpDir(const std::string &short_name) {
        std::string nameext{};
        char* tmp = getenv("TMP");
        std::string tmpdir{tmp ? tmp : "/tmp"};
        nameext.append(tmpdir);
        nameext.append("/");
        nameext.append(short_name);
        nameext.append("XXXXXX");
        name_ = strdup(nameext.c_str());
        cwd_ = (char *)malloc(1024);
        cwd_ = getcwd(cwd_, 1024);
        name_ = mkdtemp(name_);
        auto ok = chdir(name_);
        assert(ok == 0);
    }
    std::string Path() const { return std::string{name_}; }
    TmpDir() : TmpDir{"TMP"} {}
    ~TmpDir() {
        rmrf(name_);
        chdir(cwd_);
        free((void *)name_);
        free(cwd_);
    }
};

void Tick2DoubleQuote(std::string& str) {
    replace(str.begin(), str.end(), '`', '"');
}

#endif
