#include "constructors.hpp"

using init_func = void (*)();

extern "C" init_func _init_array_begin[];
extern "C" init_func _init_array_end[];
extern "C" init_func _fini_array_begin[];
extern "C" init_func _fini_array_end[];

void early::call_constructors()
{
  for (auto i = _init_array_begin; i != _init_array_end; ++i) {
    (*i)();
  }
}

void early::call_destructors()
{
  if (_fini_array_end <= +_fini_array_begin) {
    return;
  }
  for (auto i = _fini_array_end - 1; i != _fini_array_begin; --i) {
    (*i)();
  }
  (*_fini_array_begin)();
}
