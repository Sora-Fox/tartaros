/*
 * This file is part of the Tartaros project.
 *
 * Copyright (C) 2026 Sora Fox [github.com/Sora-Fox]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "early_init/idt.hpp"
#include <stddef.h>
#include <stdint.h>
#include "early_init/asm_utils.hpp"
#include "early_init/console.hpp"
#include "early_init/gdt.hpp"
#include "early_init/panic.hpp"

namespace {

  struct [[gnu::packed]] idt_entry
  {
    uint16_t isr_low;
    uint16_t cs_selector;
    uint8_t reserved1 = 0;
    uint8_t gate_type : 4;
    uint8_t reserved2 : 1 = 0;
    uint8_t dpl : 2;
    uint8_t present : 1;
    uint16_t isr_high;
  };

  struct [[gnu::packed]] idtr_register
  {
    uint16_t size;
    uintptr_t addr;
  };

  template <uint8_t = 0, uint8_t = 32>
  void init_isrs(idt_entry*);

  void disable_pic();
}

void early::init_idt()
{
  constexpr static size_t idt_size = 256;
  static idt_entry idt[idt_size]{};
  init_isrs(idt);
  const static idtr_register idtr{
    .size = sizeof(idt) - 1,
    .addr = reinterpret_cast<uintptr_t>(idt),
  };
  /*
   * TODO:
   * if (apic_present()) {
   */
  disable_pic();
  /*   init_apic();
   * } else {
   *   init_pic();
   * }
   */
  asm volatile("lidt %[idtr]; sti" : : [idtr] "m"(idtr));
}

namespace {
  template <uint8_t I>
  [[gnu::naked]]
  void handle_isr_with_error();
  template <uint8_t I>
  [[gnu::naked]]
  void handle_isr_without_error();

  struct [[gnu::packed]] interrupt_frame
  {
    uint32_t interrupt_number;
    uint32_t error_code;
    uint32_t eip;
    uint16_t cs;
    uint32_t eflags;
  };

  constexpr uint8_t interrupt_gate_type = 0xE;
  constexpr size_t ring0 = 0;

  template <uint8_t I, bool = false>
  idt_entry make_idt_entry();
  idt_entry make_idt_entry(uintptr_t, uint16_t = early::k_cs, uint8_t = ring0,
      uint8_t = interrupt_gate_type);

  template <uint8_t I>
  constexpr bool has_error_code()
  {
    return I == 8 || I == 10 || I == 11 || I == 12 || I == 13 || I == 14 || I == 17;
  }

  template <uint8_t I, uint8_t End>
  void init_isrs(idt_entry* idt)
  {
    if constexpr (I < End) {
      idt[I] = make_idt_entry<I, has_error_code<I>()>();
      init_isrs<I + 1, End>(idt);
    }
  }

  template <uint8_t I, bool HasErrorCode>
  idt_entry make_idt_entry()
  {
    if constexpr (HasErrorCode) {
      return make_idt_entry(reinterpret_cast<uintptr_t>(handle_isr_with_error<I>));
    }
    return make_idt_entry(reinterpret_cast<uintptr_t>(handle_isr_without_error<I>));
  }

  idt_entry make_idt_entry(const uintptr_t isr, const uint16_t cs_selector,
      const uint8_t dpl, const uint8_t gate_type)
  {
    return idt_entry{
      .isr_low = static_cast<uint16_t>(isr & 0xFFFF),
      .cs_selector = cs_selector,
      .gate_type = gate_type,
      .dpl = dpl,
      .present = 1,
      .isr_high = static_cast<uint16_t>(isr >> 16),
    };
  }

  const char* exceptions[32]{ "Division by zero", "Debugger", "NMI", "Breakpoint",
    "Overflow", "Bounds", "Invalid Opcode", "Coprocessor not available", "Double fault",
    "Coprocessor segment overrun", "Invalid Task State Segment", "Segment not present",
    "Stack Fault", "General protection fault", "Page Fault", "Reserved", "Math Fault",
    "Alignment Check", "Machine Check", "SIMD Floating Point Exception", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved" };

  void handle_exception(interrupt_frame ctx)
  {
    early::printf("%s\n", exceptions[ctx.interrupt_number]);
    early::printf("number = %zu\n", ctx.interrupt_number);
    early::printf("error code = %zu\n", ctx.error_code);
    early::printf("cs = %zu\n", ctx.cs);
    early::printf("eip = 0x%X\n", ctx.eip);
    early::printf("eflags = %zu\n", ctx.eflags);
    early::printf("cr2 = 0x%X\n", assembly::get_cr2());
    early::printf("cr3 = 0x%X\n", assembly::get_cr3());
    panic("System halted due to exception");
  }

  template <uint8_t I>
  void handle_isr_with_error()
  {
    asm volatile("cli\n"
                 "pushl %[int_num]\n"
                 "call %P[handler]\n"
                 "addl $8, %%esp\n"
                 "sti\n"
                 "iret"
        :
        : [int_num] "i"(I), [handler] "i"(handle_exception)
        : "esp");
  }

  template <uint8_t I>
  void handle_isr_without_error()
  {
    asm volatile("cli\n"
                 "pushl $0\n"
                 "pushl %[int_num]\n"
                 "call %P[handler]\n"
                 "addl $8, %%esp\n"
                 "sti\n"
                 "iret"
        :
        : [int_num] "i"(I), [handler] "i"(handle_exception)
        : "esp");
  }

  void disable_pic()
  {
    assembly::outb(0x20, 0x11);
    assembly::outb(0xA0, 0x11);
    assembly::outb(0x21, 0x20);
    assembly::outb(0xA1, 0x28);
    assembly::outb(0x21, 0x04);
    assembly::outb(0xA1, 0x02);
    assembly::outb(0x21, 0x01);
    assembly::outb(0xA1, 0x01);
    assembly::outb(0x21, 0xFF);
    assembly::outb(0xA1, 0xFF);
  }
}
