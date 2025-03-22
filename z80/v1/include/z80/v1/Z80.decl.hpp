#pragma once

#ifndef SPECBOLT_MODULES
#include "z80/v1/Instruction.decl.hpp"
#include "z80/common/Z80Base.hpp"
#include "peripherals/Memory.hpp"
#include <cstdint>
#include <array>
#endif

namespace specbolt::v1 {

SPECBOLT_EXPORT class Z80 : public Z80Base {
public:
  explicit Z80(Memory &memory) : Z80Base(memory) {}

  // TODO "Cycles" class?
  std::size_t execute_one();

  void push16(std::uint16_t value);
  [[nodiscard]] std::uint16_t pop16();
  void push8(std::uint8_t value);
  [[nodiscard]] std::uint8_t pop8();

  void interrupt();
  void retn();

  void branch(std::int8_t offset);

  [[nodiscard]] std::uint8_t read8(std::uint16_t address) const;
  [[nodiscard]] std::uint16_t read16(std::uint16_t address) const;
  void write8(std::uint16_t address, std::uint8_t value);
  void write16(std::uint16_t address, std::uint16_t value);


  [[nodiscard]] std::uint16_t read(Instruction::Operand operand, std::int8_t index_offset);
  void write(Instruction::Operand operand, std::int8_t index_offset, std::uint16_t value);

private:
  void execute_one_inner(const std::array<std::uint8_t, 4> opcodes, std::uint16_t initial_pc);
  template<auto prefix, std::size_t next_byte = 0>
  requires(next_byte <= 0xFF)  // this can be removed, it's just a failsafe to prevent infinite recursion
  bool execute_one_inner_static_prefix(const std::array<std::uint8_t, 4> opcode, std::uint16_t initial_pc);
  template<auto... prefixes>
  void execute_one_inner_static_prefixes(const std::array<std::uint8_t, 4> opcode, std::uint16_t initial_pc);
  void execute_one_inner_dynamic(const std::array<std::uint8_t, 4> opcode, std::uint16_t initial_pc);

  void execute(const Instruction &instr);
};

}
