#ifndef MAINPROJ_SOCKET_IO_H
#define MAINPROJ_SOCKET_IO_H

#include <iostream>
#include <map>
#include <vector>
#include "version_vector.h"
#include "ron/op.hpp"

void ReadVersionVector(std::map<uint64_t, Version>& remote_version_vector, int client_socket);
void ReadRonPatch(std::vector<ron::Op>& patch, int client_socket);
void SendVersionVector(int client_socket, const std::map<uint64_t, Version>& version_vector);
void SendPatch(int client_socket, const std::vector<ron::Op>& patch);

#endif //MAINPROJ_SOCKET_IO_H
