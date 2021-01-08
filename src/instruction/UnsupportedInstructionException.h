#include <exception>
#include <stdint.h>
#include <string>

namespace chipedda::instruction
{
    class UnsupportedInstructionException : public std::exception
    {
    public:
        UnsupportedInstructionException(uint16_t instr);
    private:
        std::string message;
        const char *  what() const noexcept override;
    };
}
