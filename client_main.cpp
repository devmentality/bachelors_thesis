#include <iostream>
#include <map>
#include <vector>
#include "tcp_client.h"
#include "version_vector.h"
#include "log.h"

using namespace std;
using namespace ron;

int main(int argc, char *argv[])
{
    map<uint64_t, Version> version_vector {
        {1, Version(3, 6)},
    };
    vector<Op> patch;
    ReadLog(patch, "patch.txt");

    RunPush("127.0.0.1", 12345, version_vector, patch);
    return 0;
}