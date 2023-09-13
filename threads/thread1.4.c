#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

int i = 0;

void cleanup(void *arg) {
    free(arg);
    printf("cleanup\n");
}

void *mythread(void *arg) {
    while (1) {
        i++;
        pthread_testcancel();
    }
}

void *mythread_str(void *arg) {
    char *str = (char *) malloc(12);
    if (!str) {
        perror("malloc allocation error");
        pthread_exit(NULL);
    }
    pthread_cleanup_push(cleanup, str) ;
            strcpy(str, "hello world");
            while (1) {
                printf("%s\n", str);
                sleep(1);
            }

    pthread_cleanup_pop(1);
}

int main() {
    pthread_t thread_str;
    int err_str = pthread_create(&thread_str, NULL, mythread_str, NULL);
    if (err_str) {
        printf("main: pthread_create() failed: %s\n", strerror(err_str));
        return -1;
    }
    pthread_cancel(thread_str);
    pthread_join(thread_str, NULL);


//    pthread_t thread;
//    int err = pthread_create(&thread, NULL, mythread, NULL);
//    if (err) {
//        printf("main: pthread_create() failed: %s\n", strerror(err));
//        return -1;
//    }
//    pthread_cancel(thread);
//    while (1) {
//        printf("counter = %d \n", i);
//        sleep(2);
//    }
//    return 0;
}