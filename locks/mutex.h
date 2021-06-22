#include <atomic>
#include <iostream>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

/**
 *  implemented with the help of the following paper and blocks
 *  Futexes Are Tricky -> https://akkadia.org/drepper/futex.pdf
 *  man page -> https://man7.org/linux/man-pages/man2/futex.2.html
 *  https://eli.thegreenplace.net/2018/basics-of-futexes/
 */
namespace mutex {

class Synchronization {
private:
  std::atomic<int> conditionVariable;

  enum LockState {
    UNLOCKED,
    LOCKED_WITHOUT_CONTENTION,
    LOCKED_WITH_CONTENTION
  };

public:
  Synchronization() : conditionVariable(LockState::UNLOCKED) {}

  void lock() {
    int val = LockState::UNLOCKED;
    if (!conditionVariable.compare_exchange_strong(
            val, LockState::LOCKED_WITHOUT_CONTENTION,
            std::memory_order_acquire)) {
      do {
        val = LockState::LOCKED_WITHOUT_CONTENTION;
        if (conditionVariable == LockState::LOCKED_WITH_CONTENTION ||
            conditionVariable.compare_exchange_strong(
                val, LockState::LOCKED_WITH_CONTENTION,
                std::memory_order_acquire)) {
          syscall(SYS_futex, &conditionVariable, FUTEX_WAIT,
                  LockState::LOCKED_WITH_CONTENTION);
        }
        val = LockState::UNLOCKED;
      } while (!conditionVariable.compare_exchange_strong(
          val, LockState::LOCKED_WITH_CONTENTION, std::memory_order_acquire));
    }
  }
  void unlock() {
    if (--conditionVariable != LockState::UNLOCKED) {
      conditionVariable.store(LockState::UNLOCKED);
      syscall(SYS_futex, &conditionVariable, FUTEX_WAKE, 1);
    }
  }
};

} // namespace mutex