#include "mutex.h"
#include <limits.h>
#include <linux/futex.h>

namespace condition_variable {

class ConditionVariable {
  int var;
  std::atomic<mutex::Synchronization *> var2;

public:
  ConditionVariable() : var(0) {}

  int wait(mutex::Synchronization &mutex_var) {
    if (var2.load() == nullptr) {
      var2.compare_exchange_strong(nullptr, &mutex_var);
    }
    if (var2.load() != &mutex_var) {
      return -1;
    }

    mutex_var.unlock();
    syscall(SYS_futex, &var, FUTEX_WAIT, 0);
    mutex_var.lock();

    return 0;
  }

  void wait(mutex::Synchronization &mutex_var, bool (*pred1)()) {
    while (!pred1()) {
      wait(mutex_var);
    }
  }

  void notifyAll() {
    var = 1;
    syscall(SYS_futex, &var, FUTEX_REQUEUE, 1, INT_MAX, var2);
  }

  void notifyOne() {
    var = 1;
    syscall(SYS_futex, &var, FUTEX_WAKE, 1);
  }
};

} // namespace condition_variable