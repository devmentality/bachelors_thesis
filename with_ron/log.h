#ifndef MAINPROJ_LOG_H
#define MAINPROJ_LOG_H

#include <vector>
#include <string>
#include "ron/const.hpp"

ron::PROC ReadLog(std::vector<ron::Op>& operations,  const std::string& file_name);
std::map<size_t, ron::Op>* BuildPrimaryKeyMapping(const std::vector<ron::Op> &operations);

#endif //MAINPROJ_LOG_H
