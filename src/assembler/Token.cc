#include "Token.h"
#include "General.h"
#include <algorithm>
#include <regex>

using namespace chipedda::assembler;

Token::Token(const std::string& str)
{
    std::string strUpper=strToUpperCase(str);
    
    auto it=TOKEN_TYPE_MAP.find(strUpper);
    if(it!=TOKEN_TYPE_MAP.end()) // constant token
    {
        type=it->second;
    }
    else // variable token
    {
        std::regex v_rgx("V[0-9A-F]"); // Vx
        std::regex decimal_rgx("\\d+"); // decimal literal
        std::regex hex_rgx("0X[0-9A-F]+"); // hexadecimal literal

        if(std::regex_match(strUpper,v_rgx))
        {
            type=TokenType::V;
            data=std::stoi(std::string(1,strUpper[1]),0,16);
        }
        else if(std::regex_match(strUpper,decimal_rgx))
        {
            type=TokenType::LITERAL;
            data=std::stoi(strUpper);
        }
        else if(std::regex_match(strUpper,hex_rgx))
        {
            type=TokenType::LITERAL;
            data=std::stoi(strUpper,0,16);
        }

    }

}
