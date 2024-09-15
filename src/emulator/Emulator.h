#include "Display.h"
#include "Keyboard.h"
#include "Registers.h"
#include "Timer.h"
#include <random>
#include <stack>

namespace chipedda::emulator {
class Emulator {
private:
  std::vector<uint8_t> memory;
  Display display;
  Registers registers;
  std::stack<uint8_t> call_stack;
  Keyboard keyboard;
  Timer soundTimer;
  Timer delayTimer;

  std::mt19937 mtEngine;

public:
  Emulator(const std::vector<uint8_t> &program);
  void nextInstruction();
};
} // namespace chipedda::emulator
