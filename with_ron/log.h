#ifndef MAINPROJ_LOG_H
#define MAINPROJ_LOG_H

#include <vector>
#include <string>
#include "ron/const.hpp"

ron::PROC ReadLog(std::vector<ron::Op>& operations,  const std::string& file_name);
std::map<size_t, ron::Op>* BuildPrimaryKeyMapping(const std::vector<ron::Op> &operations);
void MergeLogs(std::vector<ron::Op>& applied_ops_out, const std::vector<ron::Op> &log, const std::vector<ron::Op>& patch);

#endif //MAINPROJ_LOG_H
