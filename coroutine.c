
#include <stdlib.h>
#include "coroutine.h"
#include <stdio.h>


void  init(schedule_t *schedule){
    schedule->running_routine = -1;
    schedule->current_index = 0;
    schedule->routines =(coroutine_t*)malloc(sizeof(struct coroutine_t) * (MAX_UTHREAD_SIZE));
    if (!schedule->routines) {
        printf("malloc error");
        exit(-1);
    }
    for (int i = 0; i < MAX_UTHREAD_SIZE; i++) {
        schedule->routines[i].state = FREE;
        }
}
void coroutine_resume(schedule_t *schedule , int id)
{
    if(id < 0 || id >= schedule->current_index){
        return;
    }

    coroutine_t *t = &(schedule->routines[id]);

    if (t->state == SUSPEND) {
        swapcontext(&(schedule->main),&(t->ctx));
    }
}

void coroutine_yield(schedule_t *schedule)
{
    if(schedule->running_routine != -1 ){
        coroutine_t *t = &(schedule->routines[schedule->running_routine]);
        t->state = SUSPEND;
        schedule->running_routine = -1;

        swapcontext(&(t->ctx),&(schedule->main));
    }
}

void coroutine_body(schedule_t *ps)
{
    int id = ps->running_routine;
    if(id != -1){
        coroutine_t *t = &(ps->routines[id]);
        t->func(t->arg);
        t->state = FREE;
        ps->running_routine = -1;
    }
}

int coroutine_create(schedule_t *schedule,Fun func,void *arg)
{
    int id = 0;
    for(id = 0; id < schedule->current_index; ++id ){
        if(schedule->routines[id].state == FREE){
            break;
        }
    }
    
    if (id == schedule->current_index) {
        schedule->current_index++;
    }

    coroutine_t *t = &(schedule->routines[id]);
    t->state = RUNNABLE;
    t->func = func;
    t->arg = arg;
    getcontext(&(t->ctx));
    
    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = DEFAULT_STACK_SZIE;
    t->ctx.uc_stack.ss_flags = 0;
    t->ctx.uc_link = &(schedule->main);
    schedule->running_routine = id;
    
    makecontext(&(t->ctx),(void (*)(void))(coroutine_body),2,schedule);
    swapcontext(&(schedule->main), &(t->ctx));
    
    return id;
}

int schedule_finished(const schedule_t *schedule)
{
    if (schedule->running_routine != -1){
        return 0;
    }else{
        for(int i = 0; i < schedule->current_index; ++i){
            if(schedule->routines[i].state != FREE){
                return 0;
            }
        }
    }

    return 1;
}


