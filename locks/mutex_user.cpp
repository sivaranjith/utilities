#include "mutex.h"
#include <iostream>
#include <thread>

void print_first_ten_numbers(mutex::Synchronization &lock) {
  lock.lock();
  for (int i = 0; i < 10; ++i) {
    std::cout << i << " : ";
  }

  std::cout << std::endl;
  lock.unlock();
}

void print_second_ten_numbers(mutex::Synchronization &lock) {
  lock.lock();
  for (int i = 10; i < 20; ++i) {
    std::cout << i << " : ";
  }

  std::cout << std::endl;
  lock.unlock();
}

/**
 *  compile this program with lpthread flag like follows
 *  g++ mutex_user.cpp -lpthread
 */
int main() {
  mutex::Synchronization lock;

  std::thread t1(print_first_ten_numbers, std::ref(lock));
  std::thread t2(print_second_ten_numbers, std::ref(lock));

  t1.join();
  t2.join();

  return 0;
}