#include "mythread.h"
#include "myfutex.h"

void *default_thread_func(void *arg) {
    return "first thread";
}

void *test(void *arg) {
    return "second thread";
}

int main() {
    void *retval = NULL;
    mythread_t *t = NULL;
    if (mythread_create(&t, default_thread_func, NULL) == -1) {
        printf("Cannot create thread\n");
        return -1;
    }
    mythread_t *t1 = NULL;
    if (mythread_create(&t1, test, NULL) == -1) {
        printf("Cannot create thread\n");
        return -1;
    }
    sleep(1);
    if (mythread_join(t1, &retval) == -1) {
        printf("This thread cannot be joined\n");
    }
    sleep(1);
    printf("Thread returned value : %s\n", (char *) retval);

    mythread_join(t, &retval);
    printf("Thread returned value : %s\n", (char *) retval);
    return 0;
}
