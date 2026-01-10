/* Licensed under the GPLv3 license */

#include <chrono>
#include <iostream>
#include <thread>
#include "scheduler.hpp"

void task1()
{
  using mc = std::chrono::microseconds;
  std::this_thread::sleep_for(mc(100'000));
  for (int i = 0; i != 5; ++i) {
    int x = 1;
    std::cout << "enter x: ";
    if (!(std::cin >> x)) {
      std::cerr << "Failed to input x\n";
      return;
    }
    std::cout << "x = " << x << '\n';
  }
  exit_task();
}

void task2()
{
  std::cout << "task2 started\n";
  using sec = std::chrono::seconds;
  std::this_thread::sleep_for(sec(7));
  std::cout << "task2 finished\n";
  exit_task();
}

int main()
{
  try {
    [[maybe_unused]] auto t1 = create_task(task1);
    [[maybe_unused]] auto t2 = create_task(task2);
    request_priority(t1, 100);
    std::cout << "== Main ==\n";
    run_scheduler();
    std::cout << "== Main ==\n";
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}
