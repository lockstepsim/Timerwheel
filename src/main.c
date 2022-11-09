/*****************************************************************************
main.c
Maanyavar
23-10-2021
Start a win32 app
Deps:- 
******************************************************************************/

#ifndef UNICODE
#define UNICODE
#endif

#include <stdio.h>
#include <windows.h>
#include "timerwheel.c"

#define MAX_TIMERS  10

#define TASK_NEW   0 /* Schedule/arm a new timer. */
#define TASK_STOP  1 /* Disarm an existing timer. */
#define TASK_ACTIVATE  2 /* Instantly execute an existing timer. */
#define TASK_COUNT 3

#define MAX_INTERVAL  100



int cur_tick = 0;   
int timer_info[MAX_TIMERS]; // associative with 'timer'; 0: unused; >1:scheduled expected expiration; <0: stopped.
struct timerID timer[MAX_TIMERS];



void on_expire(int idx)
{
    if(timer_info[idx] != cur_tick)
    {
        printf("\n\nBad timer; incorrect schedule. timer:- %d, cur_tick:- %d \n", timer_info[idx], cur_tick);
        timer_info[idx] = -4;
    }
    else
    {
        printf("\n Perfecto. timer:- %d, cur_tick:- %d \n\n", timer_info[idx], cur_tick);
        timer_info[idx] = -2;
    }


    //stop_timer(pTimer, 0);  
    //timer_info[idx] = -1;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    srand(GetTickCount());

    for(;;)
    {
        if(rand() % 100 < 10)
        {
            switch(rand() % TASK_COUNT)
            {
                case TASK_NEW:
                {
                    for(int i = 0; i < MAX_TIMERS; i++)
                    {
                        
                        if(timer_info[i] == 0)
                        {
                            int period = 1 + (rand() % MAX_INTERVAL);
                            timer_info[i] = cur_tick + period;
                            //printf("\n \nfcurtick = %d,ccurtick = %d, interval = %d", cur_tick+period, cur_tick, period);
                            start_timer(&timer[i], period, &on_expire, i);
                            break;
                        }
                    }
                } break;

                case TASK_ACTIVATE:
                {
                    for(int i = 0; i < MAX_TIMERS; i++)
                    {
                        if(timer_info[i] > 0)
                        {
                            timer_info[i] = cur_tick;
                            stop_timer(&timer[i], 0);
                            timer_info[i] = -1;
                            break;
                        }
                    }
                } break;

                case TASK_STOP:
                {
                    for(int i = 0; i < MAX_TIMERS; i++)
                    {
                        if(timer_info[i] > 0)
                        {
                            stop_timer(&timer[i], 1);
                            timer_info[i] = -1;
                            break;
                        }
                    }
                }
            }
        }

        int countr = 0;
        for(int i = 0; i < 10; i++)
        {
            if(timer_info[i] < 0)
                countr = countr + 1;
        }
        if(countr == 10)
        {
            return 0;
            break;
        }

        printf("\n cur_tick:- %d", cur_tick);

        cur_tick++;
        per_tick_bookeeping(1);
    }
}
    