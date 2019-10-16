#ifndef _TELEBAR_UTILS_SERIALIZABLEUTILS_
#define _TELEBAR_UTILS_SERIALIZABLEUTILS_

#include <vector>
#include <string>
#include <tuple>

namespace utils{
    std::vector<std::tuple<std::string, std::string>> getTouplesFromStream(std::string stream);
};

#endif