#include <vector>
#include <string>
#include <stdint.h>

namespace chipedda::disassembler
{
    class Disassembler
    {
    private:
        const std::vector<uint8_t>& program;
        std::string disassembleInstruction(uint8_t byte1,uint8_t byte2);
        std::string intToHexString(int num,int padding);
        std::string getRegisterName(uint8_t x);
    public:
        Disassembler(const std::vector<uint8_t>& prog);
        std::string disassemble();
    };
}
