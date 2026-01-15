#include "kernel/main.hpp"
#include "early_init/asm_utils.hpp" // TODO: CODE ORG

void kernel::main()
{
  while (true) {
    assembly::hlt();
  }
}
