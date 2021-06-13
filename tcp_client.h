#ifndef MAINPROJ_TCP_CLIENT_H
#define MAINPROJ_TCP_CLIENT_H

#include <string>
#include <map>
#include <vector>
#include "version_vector.h"
#include "ron/op.hpp"


int Connect(const std::string& server_ip, int server_port);
void SendCmd(int socket, const std::string& cmd);
int64_t FetchOndx(int socket, uint64_t replica_id);
void PushChanges(
        int socket,
        const std::map<uint64_t, Version>& version_vector,
        const std::vector<ron::Op>& patch
);

#endif //MAINPROJ_TCP_CLIENT_H
