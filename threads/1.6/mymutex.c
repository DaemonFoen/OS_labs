#include "mymutex.h"

int futex(int* uaddr, int futex_op, int val, const struct timespec* timeout,
          int* uaddr2, int val3) {
  return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

void wait_on_futex_value(int* futex_addr, int val) {
  while (1) {
    int futex_rc = futex(futex_addr, FUTEX_WAIT_PRIVATE, val, NULL, NULL, 0);
    if (futex_rc == -1) {
      if (errno != EAGAIN) {
        perror("futex");
        exit(1);
      }
    } else if (futex_rc == 0) {
      if (*futex_addr == val) {
        // This is a real wakeup.
        return;
      }
    } else {
      abort();
    }
  }
}

void wake_futex_blocking(int* futex_addr, int val) {
  while (1) {
      int futex_rc = futex(futex_addr, FUTEX_WAKE_PRIVATE, val, NULL, NULL, 0);
      if (futex_rc == -1) {
          perror("futex wake");
          exit(1);
      } else if (futex_rc > 0) {
          return;
      }
  }
}