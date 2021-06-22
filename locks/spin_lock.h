#include <atomic>

/**
 *  this implementation is done with the help of the following link
 *  https://rigtorp.se/spinlock/
 */
namespace spin_lock {

class SpinLock {
  std::atomic<bool> conditionVariable;

public:
  SpinLock() : conditionVariable(false) {}

  void lock() {
    while (true) {
      if (!conditionVariable.exchange(true, std::memory_order_acquire)) {
        break;
      }
      while (conditionVariable.load(std::memory_order_relaxed))
        ;
    }
  }

  void unlock() { conditionVariable.store(false); }
};

} // namespace spin_lock