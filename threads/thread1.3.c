#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

struct Data {
    int num;
    char* str;
};

void* mythread(void *arg){
    struct Data *data = (struct Data *) arg;
    printf("thread data.num = %d, data.str = %s",data->num,data->str);
    fflush(stdout);
    free(data);
    return NULL;
}

int main() {
    struct Data *data = (struct Data *) malloc(sizeof(struct Data));
    if (!data) {
        perror("malloc allocation error");
        return -1;
    }
    data->num = 42;
    data->str = "Hello world";
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int err = pthread_create(&thread, &attr, mythread,(void*)data);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    sleep(5);
    return 0;
}