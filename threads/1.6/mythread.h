#pragma once

#define _GNU_SOURCE

#ifndef MAIN_LIBS
#define MAIN_LIBS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#endif

#define STACK_SIZE 10240 // 10 Kb

typedef void* (*func_ptr_t)(void* arg);

typedef struct mythread {
    func_ptr_t mythread_func;
    void* mythread_arg;

    int mythread_id;
    pid_t systhread_id;
    volatile int is_closed;
    volatile int is_closed_main;
    void* retval;

    void* stack_ptr;
    int* ready_state;
    int* join_state;
    int* join_state_main;
} mythread_t;

int mythread_create(mythread_t** thread, void *(start_routine), void *arg);

int mythread_join(mythread_t* thread, void** retval);
