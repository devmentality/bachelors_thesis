#ifndef MAINPROJ_LWW_H
#define MAINPROJ_LWW_H

#include <vector>
#include "ron/op.hpp"

ron::Tuple GetKeyTuple(const ron::Op& op);
std::pair<ron::Tuple, ron::Tuple> SplitOperation(const ron::Op& op);
void GenerateResultingOperations(
        std::vector<DbOperation>& resulting_operations,
        const std::vector<ron::Op>& log,
        const std::vector<ron::Op>& new_ops);

#endif //MAINPROJ_LWW_H
