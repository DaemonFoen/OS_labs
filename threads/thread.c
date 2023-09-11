#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM_THREADS 5

int global = 5;

void *mythread(void *arg) {
    int local = 0;
    static int static_local = 0;
    const int const_local = 0;
    global++;
    local++;
    printf("%d thread: local %d, global %d\n",gettid(),local,global);
    printf("local: %p\n static_local: %p\n const_local: %p\n global: %p\n",&local,&static_local,&const_local, &global);
	printf("mythread [%d %d gettid: %d pthread_self: %lu]: Hello from mythread!\n", getpid(), getppid(), gettid(), pthread_self());
    printf("\n\n");
	return NULL;
}

int main() {
	pthread_t threads[NUM_THREADS];
	int err;

    for (int i = 0; i < NUM_THREADS; ++i) {
        err = pthread_create(&threads[i], NULL, mythread, threads);
//        pthread_join(threads[i],NULL);
        if (err) {
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("%d pthread_t: %lu\n",i,threads[i]);
    }

    sleep(1200);
    for (int i = 0; i < NUM_THREADS; ++i){
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            return -1;
        }
    }

	return 0;
}
