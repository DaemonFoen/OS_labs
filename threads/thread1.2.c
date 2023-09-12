#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



void* mythread(void *arg) {
    return (void*)42;
}

void* mythread_string(void *arg){
    return "hello world";
}

void* mythread_tid(void *arg){
    printf("thread id = %d\n",gettid());
//    pthread_detach(pthread_self());
}

int main() {
    pthread_t thread_int;
    pthread_t thread_str;
    int err_int = pthread_create(&thread_int, NULL, mythread, NULL);
    int err_str = pthread_create(&thread_str, NULL, mythread_string, NULL);
    if (err_int) {
        printf("main: pthread_create() failed: %s\n", strerror(err_int));
        return -1;
    }
    if (err_str) {
        printf("main: pthread_create() failed: %s\n", strerror(err_str));
        return -1;
    }
    void* raw_int = NULL;
    void* raw_str = NULL;
    pthread_join(thread_int,&raw_int);
    pthread_join(thread_str,&raw_str);
    printf("int = %d\n",(int)raw_int);
    printf("str = %s\n",raw_str);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t thread_endless;
    while (1){
        int err = pthread_create(&thread_endless, &attr,mythread_tid,NULL);
        printf("error code = %d\n",err);
    }
    return 0;
}
