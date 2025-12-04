#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H
#include <stdint.h>
#include "timer_isr.h"


#define MAX_TASK_CNT 6 // conservative limit for now

typedef void (*TickFn)(void * );
typedef struct {
    int32_t period;
    int32_t elapsed_time;
    TickFn tick_fn;
    int8_t state;
} Task;

static Task tasks[MAX_TASK_CNT];
static uint8_t num_tasks = 0;
static long GCD_PERIOD = 10; // in ms



void addTask(long period, long elapsed_time, TickFn tick_fn){
    if(num_tasks < MAX_TASK_CNT){
        tasks[num_tasks].period = period;
        tasks[num_tasks].elapsed_time = elapsed_time;
        tasks[num_tasks].tick_fn = tick_fn;
        num_tasks++;
    }
}


void TimerISR(){
    // disable interrupts again.. im paranoid
    cli();
    for(uint8_t i = num_tasks; i ; i--){ // reverse order bc its faster to cmp 0 
        uint8_t task_idx = i - 1;
        tasks[task_idx].elapsed_time += GCD_PERIOD; // TODO: figure out if it is more correct to increment after ticking or before, if so move line after if
        if(tasks[task_idx].elapsed_time >= tasks[task_idx].period){
            //log tick_fn addr:
            // serial_print("Tick function address: 0x");
            // serial_print((long)tasks[task_idx].tick_fn, 16, true);

            // uint16_t start_time = get_current_cycle_count();
            // clear_checkpoints();


            if(tasks[task_idx].tick_fn) tasks[task_idx].tick_fn(&tasks[task_idx]);


            // uint16_t elapsed_cycles = get_current_cycle_count() - start_time;
            // .1 ms
            // if(elapsed_cycles > 1600/8){
            //     serial_print("Task ");
            //     serial_print((long)task_idx, 10, false);
            //     serial_println(" took x/8 cycles:");
            //     // float elapsed_nanoseconds = elapsed_cycles;
            //     serial_print((long)elapsed_cycles, 10);
            //     serial_print(" and generated # checkpoints: ");
            //     serial_print((long)cycle_checkpoint_index, 10, true);
            //     log_cycle_report();
            // }
            tasks[task_idx].elapsed_time = 0;
        }
            
    }

    sei();
}

void init_task_scheduler(long gcd_period){
    GCD_PERIOD = gcd_period;
    num_tasks = 0;
}
void start_task_scheduler(){
    TimerSet(GCD_PERIOD);
    TimerOn();
}


#endif // TASK_SCHEDULER_H