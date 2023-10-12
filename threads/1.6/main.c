#include "mythread.h"
#include "mymutex.h"

void* default_thread_func(void* arg) {
//    while (TRUE) {
//        sleep(1);
//        printf("Thread %d func with arg %s is working\n", arg.mythread_id, (char*) arg.mythread_arg);
//    }
    return "some string1";
}

void* test(void* arg) {
//    for (int i = 0; i < 5; i++) {
//        sleep(1);
//        printf("hello\n");
////        printf("Thread %d func with arg %s is working\n", arg.mythread_id, (char*) arg.mythread_arg);
//    }
    return "some string2";
}

int main() {
    void* retval = NULL;
    mythread_t* t = NULL;
    if (mythread_create(&t, default_thread_func, "hello1") == -1) {
        printf("Cannot create thread\n");
        return -1;
    }
    mythread_t* t1 = NULL;
    if (mythread_create(&t1, test, "hello2") == -1) {
        printf("Cannot create thread\n");
        return -1;
    }
    sleep(1);
    if (mythread_join(t1, &retval) == -1) {
        printf("This thread cannot be joined\n");
    }
    sleep(1);

    if (retval != NULL) {
        printf("Thread returned value : %s\n", (char*) retval);
    }
    mythread_join(t, &retval);
    return 0;
}
