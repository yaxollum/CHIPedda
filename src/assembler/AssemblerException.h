#pragma once

#include <exception>
#include <string>

namespace chipedda::assembler
{
    class AssemblerException : public std::exception
    {
    public:
        AssemblerException(const std::string& line);
    private:
        std::string message;
        const char *  what() const noexcept override;
    };
}
