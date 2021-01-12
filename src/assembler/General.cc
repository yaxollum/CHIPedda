#include "General.h"
#include <algorithm>

namespace chipedda::assembler
{
    std::string strToUpperCase(std::string str)
    {
        std::transform(str.begin(),str.end(),str.begin(),
                [](char c) -> char {
                    if(c>='a' && c<='z')
                    {
                        return c-('a'-'A');
                    }
                    else
                    {
                        return c;
                    }
                });
        return str;
    }
}
