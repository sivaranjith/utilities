#include "spin_lock.h"
#include <iostream>
#include <thread>

void print_first_ten_numbers(spin_lock::SpinLock &lock) {
  lock.lock();
  for (int i = 0; i < 10; ++i) {
    std::cout << i << " : ";
  }

  std::cout << std::endl;
  lock.unlock();
}

void print_second_ten_numbers(spin_lock::SpinLock &lock) {
  lock.lock();
  for (int i = 10; i < 20; ++i) {
    std::cout << i << " : ";
  }

  std::cout << std::endl;
  lock.unlock();
}

/**
 *  compile this program with lpthread flag like follows
 *  g++ spin_lock_user.cpp -lpthread
 */
int main() {
  spin_lock::SpinLock lock;

  std::thread t1(print_first_ten_numbers, std::ref(lock));
  std::thread t2(print_second_ten_numbers, std::ref(lock));

  t1.join();
  t2.join();

  return 0;
}