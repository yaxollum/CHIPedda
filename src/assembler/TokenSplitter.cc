#include "TokenSplitter.h"
#include "AssemblerException.h"
#include "General.h"
#include <regex>

using namespace chipedda::assembler;

TokenSplitter::TokenSplitter(const std::string &ln) : line(ln) {
  std::vector<std::string> split = splitLine(line);
  if (split.empty()) {
    instruction = Symbol::NOT_AN_INSTRUCTION;
  } else {
    instruction = getInstructionFromName(split[0]);
    if (instruction != Symbol::NOT_AN_INSTRUCTION) {
      tokens.assign(split.begin() + 1, split.end());
    }
  }
}

Symbol
TokenSplitter::getInstructionFromName(const std::string &instructionName) {
  if (instructionName.length() == 0 || instructionName[0] == ';') {
    return Symbol::NOT_AN_INSTRUCTION;
  } else {
    std::string instructionUpper = strToUpperCase(instructionName);
    auto it = SYMBOL_MAP.find(instructionUpper);
    if (it != SYMBOL_MAP.end()) {
      return it->second;
    } else {
      throw AssemblerException(instructionUpper);
    }
  }
}

std::vector<std::string> TokenSplitter::splitLine(const std::string &line) {
  std::regex delim("[\\s,]+");
  auto semicolon_it = std::find(line.begin(), line.end(), ';');
  std::sregex_token_iterator token_it(line.begin(), semicolon_it, delim, -1);
  std::vector<std::string> split;
  std::copy_if(token_it, std::sregex_token_iterator(), back_inserter(split),
               [](std::string s) -> bool {
                 return !s.empty(); // only copy non-empty strings
               });
  return split;
}

bool TokenSplitter::isInstruction() const {
  return instruction != Symbol::NOT_AN_INSTRUCTION;
}

Symbol TokenSplitter::getInstruction() const { return instruction; }

const std::vector<Token> &TokenSplitter::getTokens() const { return tokens; }

const std::string &TokenSplitter::getLine() const { return line; }
