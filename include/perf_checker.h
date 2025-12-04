#ifndef PERF_CHECKER_H
#define PERF_CHECKER_H


#include <avr/io.h>
#include "serial.h"


void perf_checker_init(){
    // setup timer1 for cycle counting
    TCCR1B = (1 << CS11); // prescaler /8
    TCNT1 = 0;
}


unsigned int cycle_checkpoints[10];
unsigned int cycle_checkpoints_info[10];
unsigned char cycle_checkpoint_index = 0;
unsigned int cycle_prescale = 8;


void cycle_log_checkpoint(int info = 0){
    if(cycle_checkpoint_index < 10){
        cycle_checkpoints[cycle_checkpoint_index] = TCNT1;
        cycle_checkpoints_info[cycle_checkpoint_index] = info;
        cycle_checkpoint_index++;
    }
}

void clear_checkpoints(){
    cycle_checkpoint_index = 0;
    for(unsigned char i = 0; i < 10; i++){
        cycle_checkpoints[i] = 0;
        cycle_checkpoints_info[i] = 0;
    }
}

void log_cycle_report(){
    serial_println("Cycle Report:");
    for(unsigned char i = 0; i < cycle_checkpoint_index; i++){
        unsigned int diff = cycle_checkpoints[i] - cycle_checkpoints[i - 1 >= 0 ? i - 1 : 0];
        serial_print("checkpoint ");
        serial_print((long)i, 10, false);
        serial_print(": ");
        serial_print((long)cycle_checkpoints[i], 10, true);
        serial_print("  Cycles since last: ");
        serial_println((long)diff, 10);

        serial_print("  Info: ");
        serial_println((long)cycle_checkpoints_info[i], 10);
        
    }
}

uint16_t get_current_cycle_count(){
    return TCNT1;
}

#endif // PERF_CHECKER_H