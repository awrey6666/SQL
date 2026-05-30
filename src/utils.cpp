#include "utils.h"
#include <cctype>

static std::string trim(std::string s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())))
        s.erase(0, 1);
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
        s.pop_back();
    return s;
}

std::string clean(std::string s) {
    s = trim(s);
    if (!s.empty() && s.back() == ';')
        s.pop_back();

    s = trim(s);
    if (!s.empty() && s.front() == '"')
        s.erase(0, 1);
    if (!s.empty() && s.back() == '"')
        s.pop_back();

    return trim(s);
}
