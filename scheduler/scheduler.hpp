#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <cstddef>

using task_fn_t = void (*)(void);
using task_id_t = std::size_t;
using priority_t = std::size_t;

task_id_t create_task(task_fn_t);
void request_priority(task_id_t, priority_t);
void run_scheduler();

#endif
