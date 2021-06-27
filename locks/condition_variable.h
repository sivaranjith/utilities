#include "mutex.h"
#include <linux/futex.h>

namespace condition_variable {

class ConditionVariable {
  int var;
  int var2;

public:
  ConditionVariable() : var(0) {}

  void wait(mutex::Synchronization &mutex_var) {
    var2 = mutex_var.conditionVariable;
    syscall(SYS_futex, &var, FUTEX_WAIT, var);
    mutex_var.lock();
  }

  void wait(mutex::Synchronization &mutex_var, bool (*pred1)()) {
    while (!pred1()) {
      wait(mutex_var);
    }
  }

  void notifyAll() { syscall(SYS_futex, &var, FUTEX_REQUEUE, 1, NULL, &var2); }

  void notifyOne() { syscall(SYS_futex, &var, FUTEX_WAKE, 1); }
};

} // namespace condition_variable