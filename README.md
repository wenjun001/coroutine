coroutine
=======

Simple coroutine writeed by C


* use init initiatize the env 
* use`coroutine_create` create coroutine
* use`coroutine_resume` run or resume yield coroutine
* use`coroutine_yield` yield a coroutine
* use`schedule_finished` check whether all coroutines are done

Following are test code:

```
#include "coroutine.h"
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include<pthread.h>
schedule_t s;

int f1back = 0;
int f2back = 0;

void func1(void * arg)
{
    int *value = arg;
    printf("func1-- start with val :%d>>>>>\n",*value);

    coroutine_yield(&s);
    printf("func1-- end >>>>>\n");
}

void func2(void *arg)
{
    printf("func2-- start >>>>>\n");
    coroutine_yield(&s);
    printf("func2-- end >>>>>\n");

}


void updateValue(){
    sleep(2);
    f1back = 1;
    f2back = 1;
}


void (*fun1P)();
void (*fun2P)();
int main()
{
    printf("main start\n");
    init(&s);

    fun1P=&func1;
    fun2P=&func2;
    int x = 1;

    int id1 = coroutine_create(&s,fun1P,(void *)&x);

    //wait
    int id2 = coroutine_create(&s,fun2P,(void *)NULL);



    pthread_t tid;
    if (pthread_create(&tid, NULL, (void *)updateValue, NULL) != 0) {
        fprintf(stderr, "thread create failed\n");
        return -1;
    }




    while(f2back==0 && f1back==0){
         printf("main is running\n");
         sleep(1);
        if(f1back==1){
            coroutine_resume(&s,id1);
        }
        if(f2back==1){
            coroutine_resume(&s,id2);
        }

    }


    printf("main end\n");
    return 0;
}
}
```

run and out put 
```
main start
func1-- start with val :1>>>>>
func2-- start >>>>>
main is running
main is running
func1-- end >>>>>
func2-- end >>>>>
main end
```

Let me explain it
Firstly main create coroutine f1 and f2 then run it with pass value. 
```
main start
func1-- start with val :1>>>>>
func2-- start >>>>>
```
Secondely f1 and f2 coroutine need wait such as read data from DB or IO request... so back to main to run main
```
main start
func1-- start with val :1>>>>>
func2-- start >>>>>
main is running
main is running
```
Thiredly  data or IO are ready(In here we create thread to simulate this case )

Finally we get notification and data is ready then go back run f1 and f2  then finish main. 
```
func1-- end >>>>>
func2-- end >>>>>
main end
```

There is init version and been use in prototype project. If use it need more time to handle all cases. 

enjoy it!










