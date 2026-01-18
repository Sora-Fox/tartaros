# This file is part of the Tartaros project.
#
# Copyright (C) 2026 Sora Fox [github.com/Sora-Fox]
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

SRC =
SRC += src/bootstrap/utils.cpp src/bootstrap/start.cpp src/bootstrap/enter_virtual.cpp
SRC += src/early_init/mb_parser.cpp src/early_init/constructors.cpp src/early_init/gdt.cpp src/early_init/idt.cpp src/early_init/enter_kernel.cpp src/early_init/vga.cpp src/early_init/console.cpp src/early_init/panic.cpp src/early_init/asm_utils.cpp src/early_init/framebuffer.cpp
SRC += src/kernel/console.cpp src/kernel/vga_driver.cpp src/kernel/core_utils.cpp src/kernel/main.cpp
SRC += src/mm/new.cpp src/mm/alloc.cpp src/mm/paging.cpp src/mm/phys_alloc.cpp
SRC += src/libk/vsnprintf.cpp src/libk/cxxabi.cpp src/libk/ctype.cpp src/libk/string.cpp
FONT = font.psf

BUILD = build
TARGET = bin/kernel
ISO = bin/os.iso

FONT_OBJ = $(addprefix $(BUILD)/,$(FONT:.psf=.o))
OBJ = $(addprefix $(BUILD)/,$(SRC:.cpp=.o))
DEP = $(OBJ:.o=.d)

override CPPFLAGS += -MMD -MP -Isrc
override CXXFLAGS += -ffreestanding -fno-exceptions -fno-rtti
#TODO: fix warnings
override CXXFLAGS += -Wall -Wextra -Wpedantic #-Werror
override CXXFLAGS += -std=c++20
override CXXFLAGS += -g
override LDFLAGS += -T linker.ld -nostdlib -Wl,-no-pie

ifeq ($(CROSS_COMPILE),)
    ifeq ($(CC), clang)
        override TARGET_ARCH += --target=i686-unknown-elf
    else
        $(error Use clang of setup CROSS_COMPILE)
    endif
else
    override CXX := $(CROSS_COMPILE)$(CXX)
endif


override quiet =
ifeq ($(findstring s,$(firstword -$(MAKEFLAGS))),s)
    override quiet = 1
endif

override Q =
ifneq ($(VERBOSE),1)
    override Q = @
endif

COLOR_OFF = \e[0m
COLOR_CXX = \e[32m
COLOR_LD  = \e[1;32m

.DEFAULT_GOAL := all
.PHONY: all
all: $(BUILD)/$(ISO)

$(BUILD)/$(TARGET): $(OBJ) $(FONT_OBJ)
	$(if $(quiet),,@echo -e "[LD ] $(COLOR_LD)Linking $(subst $(BUILD)/,,$@) (executable)$(COLOR_OFF)")
	@mkdir -p $(@D)
	$(Q)$(CXX) $(TARGET_ARCH) $(LDLIBS) $(LDFLAGS) $^ -o $@
	$(Q)grub-file --is-x86-multiboot $@


$(OBJ): $(BUILD)/%.o: %.cpp
	$(if $(quiet),,@echo -e "[CXX] $(COLOR_CXX)Compiling $(subst $(BUILD)/,,$@)$(COLOR_OFF)")
	@mkdir -p $(@D)
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

$(FONT_OBJ): $(FONT)
	$(if $(quiet),,@echo -e "[PSF] Compiling $(subst $(BUILD)/,,$@)")
	@mkdir -p $(@D)
	$(Q)objcopy -O elf32-i386 -B i386 -I binary $(FONT) $(FONT_OBJ)

.PHONY: clean
clean:
	$(if $(quiet),,@echo -e "[RM ] Removing build artifacts")
	$(Q)$(RM) -r $(BUILD)

$(BUILD)/$(ISO): $(BUILD)/$(TARGET) grub.cfg
	$(if $(quiet),,@echo -e "[LD ] $(COLOR_LD)Building $(subst $(BUILD)/,,$@) (image)$(COLOR_OFF)")
	$(Q)mkdir -p isodir/boot/grub
	$(Q)cp $< isodir/boot
	$(Q)cp grub.cfg isodir/boot/grub/grub.cfg
	$(Q)sed -i "s/KERNEL_BIN/$(notdir $<)/" isodir/boot/grub/grub.cfg
	$(Q)grub-mkrescue -o $@ --grub-mkimage="grub-mkimage -O i386-pc" isodir &>/dev/null
	$(Q)$(RM) -r isodir

.PHONY: qemu 
qemu: $(BUILD)/$(ISO)
	qemu-system-x86_64 -cdrom $< -boot d -serial stdio -m 8G

.PHONY: gdb
gdb: $(BUILD)/$(ISO)
	qemu-system-x86_64 -cdrom $< -boot d -serial stdio -m 8G -s -S

-include $(DEP)

