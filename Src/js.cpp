//
// Created by halet on 1/5/2026.
//

#include "js.h"

std::unordered_map<VarType, std::string> VarTypeToString = {
    {CONSTANT, "const"},
    {LET, "let"},
    {VAR, "var"}
};

std::string js;

void *expectedNextInitialized = nullptr;
bool nextInitializedIsRequired = false;
