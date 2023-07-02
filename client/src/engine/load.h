#pragma once

#include <string>
#include <fstream>
#include <sstream>

namespace util {

    std::string read_to_string(const std::string &path) {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        return stream.str();
    }

}
//std::vector<>