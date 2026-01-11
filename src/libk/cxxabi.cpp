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

#include <stddef.h>
#include <stdint.h>

namespace {
  constexpr uint8_t INITIALIZED = 1;
  constexpr uint8_t IN_PROGRESS = 2;
}

extern "C" {
int __cxa_guard_acquire(uint32_t* guard)
{
  if (*guard & INITIALIZED) {
    return 0;
  }
  *guard |= IN_PROGRESS;
  return 1;
}

void __cxa_guard_release(uint32_t* guard)
{
  *guard &= ~IN_PROGRESS;
  *guard |= INITIALIZED;
}

void __cxa_guard_abort(uint32_t* guard)
{
  *guard &= ~IN_PROGRESS;
}

struct atexit_func
{
  void (*func)(void*);
  void* obj;
};

constexpr size_t MAX_ATEXIT = 32;
static atexit_func atexit_funcs[MAX_ATEXIT];
static size_t atexit_count = 0;

int __cxa_atexit(void (*func)(void*), void* arg, void* dso_handle)
{
  (void)dso_handle;
  if (atexit_count >= MAX_ATEXIT) {
    return -1;
  }
  atexit_funcs[atexit_count++] = { func, arg };
  return 0;
}

void __cxa_finalize(void* dso_handle)
{
  (void)dso_handle;
  for (size_t i = 0; i < atexit_count; ++i) {
    atexit_funcs[i].func(atexit_funcs[i].obj);
  }
  atexit_count = 0;
}

void __cxa_pure_virtual()
{
  asm volatile("cli");
  while (true) {
    asm volatile("hlt");
  }
}

void __cxa_throw(void* obj, void* type, void (*destructor)(void*))
{
  (void)obj;
  (void)type;
  (void)destructor;
  asm volatile("cli");
  while (true) {
    asm volatile("hlt");
  }
}

void* __cxa_allocate_exception(size_t size)
{
  (void)size;
  return nullptr;
}

void __cxa_free_exception(void* obj)
{
  (void)obj;
}
}
