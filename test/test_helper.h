#pragma once
#include <gtest/gtest.h>
#include <string>
#include <cctype>

#define STRINGIFY(code)  #code

inline static std::string wo_ws(std::string a)
{
    std::string b(a);
    for (auto it = b.begin(); it != b.end(); )
        if (std::isspace(*it))
            it = b.erase(it);
        else
            it++;
    return b;
}

inline static std::string trim(std::string a)
{
    while (!a.empty() && std::isspace(a[0]))
        a = a.substr(1);
    
    while (!a.empty() && std::isspace(a[a.size()-1]))
        a = a.substr(0, a.size()-1);    
    for (auto it = a.begin(); it != a.end(); ++it)
        if (std::iscntrl(*it))
            *it = ' ';
    return a;
}