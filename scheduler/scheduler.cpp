#include "scheduler.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <format>
#include <memory>
#include <optional>
#include <print>
#include <unordered_map>
#include <utility>
#include <vector>
#include <signal.h>
#include <sys/time.h>
#include <ucontext.h>
#include <unistd.h>

using clock_type = std::chrono::steady_clock;
using timestamp = clock_type::time_point;

struct task_t
{
  ucontext_t ctx{};
  std::unique_ptr<char[]> stack = nullptr;
  task_fn_t function{};
  priority_t priority = 0;
  timestamp last_run;
  enum class status_t { finished, run };
  status_t status = status_t::run;
};

struct Scheduler
{
  using task_container = std::unordered_map<task_id_t, task_t>;
  ucontext_t main_ctx{};
  task_container tasks{};
  task_id_t cur;
  constexpr static std::size_t frequency_hz = 500;
};

static Scheduler scheduler;

namespace {
  void schedule(int, siginfo_t*, void*);
  void do_request_priority(priority_t, task_t&);
  void task_entry_point(task_id_t);
  void refresh_tasks();
  void remove_finished();
  std::optional<task_id_t> get_next_task();
  void set_initial_task();
  void start_timer();
  void stop_timer();

  using sigaction_t = struct sigaction;
  using itimerval_t = struct itimerval;
  void safe_getcontext(ucontext_t*);
  void safe_swapcontext(ucontext_t* oucp, const ucontext_t* ucp);
  void safe_sigaction(int, const sigaction_t* act, sigaction_t* old);
  void safe_setitimer(int, const itimerval_t* new_val, itimerval_t* old);

  void out_debug_info();
}

task_id_t create_task(task_fn_t function)
{
  assert(function && "Null task function");
  constexpr static std::size_t stack_size = 64 * 1024;
  task_t task{
    .stack = std::make_unique<char[]>(stack_size),
    .function = function,
    .priority = 0,
    .last_run = clock_type::now(),
    .status = task_t::status_t::run,
  };
  static task_id_t task_counter = 0;
  safe_getcontext(&task.ctx);
  task.ctx.uc_stack.ss_sp = task.stack.get();
  task.ctx.uc_stack.ss_size = stack_size;
  task.ctx.uc_stack.ss_flags = 0;
  const static auto entry = reinterpret_cast<task_fn_t>(task_entry_point);
  const task_id_t task_id = task_counter++;
  makecontext(&task.ctx, entry, 1, task_id);
  scheduler.tasks[task_id] = std::move(task);
  return task_id;
}

void request_priority(task_id_t task_id, priority_t priority)
{
  auto& task = scheduler.tasks.at(task_id);
  do_request_priority(priority, task);
}

void run_scheduler()
{
  if (scheduler.tasks.empty()) {
    return;
  }
  set_initial_task();
  out_debug_info();
  start_timer();
  safe_swapcontext(&scheduler.main_ctx, &scheduler.tasks[scheduler.cur].ctx);
  stop_timer();
}

namespace {
  void schedule(int, siginfo_t*, void*)
  {
    refresh_tasks();
    auto* prev_ctx = &scheduler.tasks[scheduler.cur].ctx;
    auto next_task = get_next_task();
    auto* next_ctx = &scheduler.main_ctx;
    if (next_task) {
      scheduler.cur = *next_task;
      task_t& cur_task = scheduler.tasks[scheduler.cur];
      cur_task.last_run = clock_type::now();
      next_ctx = &cur_task.ctx;
      out_debug_info();
    }
    safe_swapcontext(prev_ctx, next_ctx);
  }

  void do_request_priority(const priority_t priority, task_t& task)
  {
    constexpr static priority_t max = 100;
    task.priority = std::min(priority, max);
  }

  void task_entry_point(task_id_t task_id)
  {
    auto& task = scheduler.tasks[task_id];
    try {
      task.function();
    } catch (...) {
    }
    task.status = task_t::status_t::finished;
    while (true) {
      pause();
    }
  }

  void increase_priority(task_t& task)
  {
    using ms = std::chrono::milliseconds;
    auto now = clock_type::now();
    auto age = std::chrono::duration_cast<ms>(now - task.last_run);
    auto new_priority = task.priority + age.count() / 20;
    do_request_priority(new_priority, task);
  }

  void refresh_tasks()
  {
    assert(!scheduler.tasks.empty() && "no tasks");
    remove_finished();
    constexpr static priority_t fine = 2;
    auto& tasks = scheduler.tasks;
    auto& current = scheduler.tasks[scheduler.cur];
    current.priority -= std::min(current.priority, fine);
    std::for_each(tasks.begin(), tasks.end(),
        [](auto& task) { increase_priority(task.second); });
  }

  void remove_finished()
  {
    std::vector<task_id_t> to_erase;
    for (const auto& [id, task] : scheduler.tasks) {
      if (task.status == task_t::status_t::finished && id != scheduler.cur) {
        to_erase.push_back(id);
      }
    }
    for (const auto& id : to_erase) {
      scheduler.tasks.erase(id);
    }
  }

  std::optional<task_id_t> get_next_task()
  {
    assert(!scheduler.tasks.empty() && "no tasks");
    task_id_t best_task = 0;
    bool is_found = false;
    for (const auto& [id, task] : scheduler.tasks) {
      if (task.status == task_t::status_t::finished) {
        continue;
      }
      if (!is_found || task.priority > scheduler.tasks[best_task].priority) {
        is_found = true;
        best_task = id;
      }
    }
    return is_found ? std::optional(best_task) : std::nullopt;
  }

  void set_initial_task()
  {
    assert(!scheduler.tasks.empty() && "no tasks");
    scheduler.cur = 0;
    for (const auto& [id, task] : scheduler.tasks) {
      auto& current = scheduler.tasks[scheduler.cur];
      if (task.priority > current.priority) {
        scheduler.cur = id;
      }
    }
  }

  std::size_t hz_to_ms(std::size_t hz) noexcept
  {
    return 1000 / hz;
  }

  void start_timer()
  {
    const std::size_t ms = hz_to_ms(scheduler.frequency_hz);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = schedule;
    sa.sa_flags |= SA_RESTART;
    sa.sa_flags |= SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    safe_sigaction(SIGALRM, &sa, nullptr);
    itimerval itv;
    itv.it_interval.tv_sec = ms / 1000;
    itv.it_interval.tv_usec = (ms % 1000) * 1000;
    itv.it_value = itv.it_interval;
    safe_setitimer(ITIMER_REAL, &itv, nullptr);
  }

  void stop_timer()
  {
    itimerval itv{};
    safe_setitimer(ITIMER_REAL, &itv, nullptr);
  }

  void safe_getcontext(ucontext_t* ucp)
  {
    if (getcontext(ucp) != 0) {
      throw std::logic_error("getcontext failed");
    }
  }

  void safe_swapcontext(ucontext_t* oucp, const ucontext_t* ucp)
  {
    if (swapcontext(oucp, ucp) == -1) {
      throw std::logic_error("swapcontext failed");
    }
  }

  void safe_sigaction(int signum, const sigaction_t* act, sigaction_t* oldact)
  {
    if (sigaction(signum, act, oldact) != 0) {
      throw std::logic_error("sigaction failed");
    }
  }

  void
  safe_setitimer(int which, const itimerval_t* new_val, itimerval_t* old_val)
  {
    if (setitimer(which, new_val, old_val) != 0) {
      throw std::logic_error("setitimer failed");
    }
  }

  void out_debug_info()
  {
#ifndef NDEBUG
    std::string ids = "\t<";
    std::print("\t<");
    for (const auto& [id, task] : scheduler.tasks) {
      ids += std::format("{:<3} ", id);
      const auto* cur = &scheduler.tasks[scheduler.cur];
      if (&task == cur && task.status == task_t::status_t::finished) {
        std::print("{:<3}", "[f]");
      } else if (&task == cur) {
        std::print("{:<3}", std::format("[{}]", task.priority));
      } else if (task.status == task_t::status_t::finished) {
        std::print("{:<3}", " f");
      } else {
        std::print(" {:<3}", task.priority);
      }
    }
    std::println(">");
//    std::println(ids + ">");
#endif
  }
}
