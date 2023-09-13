#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

void *thread_one(void *arg) {
    sigset_t sigset;
    sigfillset(&sigset);
    if (pthread_sigmask(SIG_BLOCK, &sigset, NULL) != 0) {
        perror("pthread_sigmask");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (i != 3) {
        pthread_kill(pthread_self(), SIGSEGV);
        printf("Thread 1 blocked all signals\n");
        i++;
    }
    return NULL;
}


void sigint_handler(int signo) {
    printf("Thread 2 received SIGINT\n");
}

void *thread_two(void *arg) {
    signal(SIGINT, sigint_handler);
    int i = 0;
    while (i != 3) {
        pthread_kill(pthread_self(), SIGINT);
        i++;
    }
    return NULL;
}

void *thread_three(void *arg) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    int i = 0;
    while (i!=3){
        int sig;
        sigwait(&set, &sig);
        i++;
        printf("Thread 3 received SIGQUIT\n");
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2, thread3;
    int err1, err2, err3;

    err1 = pthread_create(&thread1, NULL, thread_one, NULL);
    if (err1 != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    err2 = pthread_create(&thread2, NULL, thread_two, NULL);
    if (err2 != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    err3 = pthread_create(&thread3, NULL, thread_three, NULL);
    if (err3 != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    sleep(1);
    pthread_kill(thread3, SIGQUIT);
    sleep(1);
    pthread_kill(thread3, SIGQUIT);
    sleep(1);
    pthread_kill(thread3, SIGQUIT);
    pthread_join(thread3, NULL);
    return 0;
}
