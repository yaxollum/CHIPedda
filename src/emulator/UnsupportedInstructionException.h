#include <exception>
#include <stdint.h>
#include <string>

namespace chipedda::emulator
{
    class UnsupportedInstructionException : public std::exception
    {
    public:
        UnsupportedInstructionException(uint16_t instr);
    private:
        uint16_t instruction;
        std::string message;
        const char *  what() const noexcept override;
    };
}
