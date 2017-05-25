

#include <ucontext.h>


#define DEFAULT_STACK_SZIE (1024*128)
#define MAX_UTHREAD_SIZE   1024

enum CoroutineState{FREE,RUNNABLE,RUNNING,SUSPEND};

struct schedule_t;

typedef void (*Fun)(void *arg);

typedef struct coroutine_t
{
    ucontext_t ctx;
    Fun func;
    void *arg;
    enum CoroutineState state;
    char stack[DEFAULT_STACK_SZIE];
}coroutine_t;

typedef struct schedule_t
{
    ucontext_t main;
    int running_routine;
    coroutine_t *routines;
    int current_index;

}schedule_t;


static void coroutine_body(schedule_t *ps);
void init(schedule_t *schedule);
int  coroutine_create(schedule_t *schedule,Fun func,void *arg);
void coroutine_yield(schedule_t *schedule);
void coroutine_resume(schedule_t *schedule,int id);
int  schedule_finished(const schedule_t *schedule);


