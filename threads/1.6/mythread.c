#define _GNU_SOURCE

#ifndef THREAD_LIBS
#define THREAD_LIBS

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#endif

#include "mythread.h"
#include "mymutex.h"


void mythread_end_routine(mythread_t *thread) {
    printf("Thread %d is ready to join\n", thread->mythread_id);
    thread->is_closed = 1;
    while (1) {
        if (thread->is_closed_main == 1) {
            *(thread->join_state_main) = 1;
            wake_futex_blocking(thread->join_state_main, 1);
            wait_on_futex_value(thread->join_state, 1);
            break;
        } else {
            wait_on_futex_value(thread->join_state, 1);
        }
    }
    return;
}

int mythread_startup(void *arg) {
    mythread_t *thread = (mythread_t *) arg;
    *(thread->ready_state) = 1;
    wake_futex_blocking(thread->ready_state, 1);

    //^ work
    thread->retval = thread->mythread_func(thread->mythread_arg);

    mythread_end_routine(thread);
    return 0;
}

void *create_stack(size_t stack_size) {
    void *stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    memset(stack, 0, STACK_SIZE);
    if (stack == MAP_FAILED) {
        return NULL;
    }
    return stack;
}

int mythread_create(mythread_t **thread, void *(start_routine), void *arg) {
    void *child_stack = create_stack(STACK_SIZE);
    if (child_stack == NULL) {
        perror("Child stack");
        return -1;
    }

    *thread = (mythread_t *) malloc(sizeof(mythread_t) * 1);
    static int newthread_id = 0;
    newthread_id++;
    (*thread)->stack_ptr = child_stack;
    (*thread)->mythread_id = newthread_id;

    (*thread)->is_closed = 0;
    (*thread)->is_closed_main = 0;

    (*thread)->retval = NULL;
    (*thread)->mythread_func = start_routine;
    (*thread)->mythread_arg = arg;

    (*thread)->join_state = (int *) malloc(sizeof(int) * 1);
    (*thread)->join_state_main = (int *) malloc(sizeof(int) * 1);
    (*thread)->ready_state = (int *) malloc(sizeof(int) * 1);
    *((*thread)->ready_state) = 0;
    *((*thread)->join_state) = 0;
    *((*thread)->join_state_main) = 0;

    (*thread)->systhread_id = clone(mythread_startup, child_stack + STACK_SIZE,
                                    CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD, (void *) *thread);
    if ((*thread)->systhread_id == -1) {
        perror("Clone");
        if (munmap(child_stack, STACK_SIZE) == -1){
            printf("munmap error\n");
            abort();
        }
        return 0;
    }

    printf("Thread %d created\n", (*thread)->mythread_id);
    wait_on_futex_value((*thread)->ready_state, 1);
    printf("Thread %d started\n", (*thread)->mythread_id);

    return 0;
}

int mythread_join(mythread_t *thread, void **retval_) {
    if (thread == NULL) {
        return -1;
    }
    int tid;
    thread->is_closed_main = 1;
    while (1) {
        if (thread->is_closed == 1) {
            tid = thread->mythread_id;
            *retval_ = thread->retval;
            *(thread->join_state) = 1;
            wake_futex_blocking(thread->join_state, 1);
            free(thread->ready_state);
            free(thread->join_state_main);
            free(thread->join_state);
            free(thread);
            break;
        } else {
            wait_on_futex_value(thread->join_state_main, 1);
        }
    }
    printf("Thread %d has been joined\n", tid);
    return 0;
}
