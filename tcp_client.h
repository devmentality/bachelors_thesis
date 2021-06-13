#ifndef MAINPROJ_TCP_CLIENT_H
#define MAINPROJ_TCP_CLIENT_H

#include <string>
#include <map>
#include <vector>
#include "version_vector.h"
#include "ron/op.hpp"


void RunPush(
        const std::string& server_ip,
        int server_port,
        const std::map<uint64_t, Version>& version_vector,
        const std::vector<ron::Op>& patch
);

#endif //MAINPROJ_TCP_CLIENT_H
