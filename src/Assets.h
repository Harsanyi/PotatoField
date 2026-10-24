#pragma once
#include <string>

namespace Assets
{
    void initialize(const char* executable);
    std::string file(const std::string& relativePath);
}
