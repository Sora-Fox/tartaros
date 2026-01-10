#include <cassert>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>
#include <sys/time.h>
#include <ucontext.h>
#include "scheduler.hpp"

constexpr static double coef = 0.2;

void task1_5_100()
{
  for (int i = 0; i != 5; ++i) {
    int x = 0;
    // printf("enter x: ");
    // scanf("%d", &x);
    std::cout << "[" << x << "]\n" << std::flush;
    using mc = std::chrono::microseconds;
    std::this_thread::sleep_for(mc(size_t(coef * 100'000)));
  }
}

void task2_5_100()
{
  for (int i = 0; i != 5; ++i) {
    std::cout << "[2]\n" << std::flush;
    using mc = std::chrono::microseconds;
    std::this_thread::sleep_for(mc(size_t(coef * 100'000)));
  }
}

void task3_5_300()
{
  for (int i = 0; i != 5; ++i) {
    std::cout << "[3]\n" << std::flush;
    using mc = std::chrono::microseconds;
    std::this_thread::sleep_for(mc(size_t(coef * 200'000)));
  }
}

void task4_10_900()
{
  for (int i = 0; i != 10; ++i) {
    std::cout << "[4]\n" << std::flush;
    using mc = std::chrono::microseconds;
    std::this_thread::sleep_for(mc(size_t(coef * 300'000)));
  }
}

int main()
{
  try {
    create_task(task1_5_100);
    auto t2 = create_task(task2_5_100);
    auto t3 = create_task(task3_5_300);
    auto t4 = create_task(task4_10_900);
    request_priority(t2, 30);
    request_priority(t3, 50);
    request_priority(t4, 98);
    std::cout << "== Main ==\n";
    run_scheduler();
    std::cout << "== Main ==\n";
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}
