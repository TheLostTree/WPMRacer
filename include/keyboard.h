#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <avr/interrupt.h>
#include <util/delay.h>
#include "serial.h"
#include "queue.h"
#include "scancodes.h"



Queue keyboard_buf;
uint8_t keyboard_data[16];

#define PS2_PIN_CLOCK 2
#define PS2_PIN_DATA 3
#define PS2_INPUT_PINS PINC
#define PS2_INPUT_PORT PORTC
#define PS2_INPUT_DDR DDRC

// keyboard input states: shift/noshift, keydown/keyup




void keyboard_init(){
    // // setup INT0 on falling edge
    // EICRA |= (1 << ISC01); // ISC01 = 1, ISC00 = 0 : falling edge
    // EIMSK |= (1 << INT0);  // enable INT0
    TCCR1B = (1 << CS11); // prescaler /8
    TCNT1 = 0;
    // last_time = 0; // Initialize last_time
    // init ddrb as input
    PS2_INPUT_DDR &= ~( (1 << PS2_PIN_CLOCK) | (1 << PS2_PIN_DATA) ); // inputs
    // enable pullups
    PS2_INPUT_PORT |= ( (1 << PS2_PIN_CLOCK) | (1 << PS2_PIN_DATA) ); // pullups

    // debug_bin(PS2_INPUT_DDR);
    // serial_println("");
    // debug_bin(PS2_INPUT_PORT);
    // serial_println("");

    // setup pin change interrupt on pc2 (pcint10) for clock line
    PCICR |= (1 << PCIE1); // enable pin change interrupt for port
    PCMSK1 |= (1 << PCINT10) | (1 << PCINT11); // enable pin change interrupt for pc2, pc3

    init_queue(&keyboard_buf, keyboard_data, sizeof(keyboard_data) / sizeof(keyboard_data[0]));
}
bool ps2_data_available(){
    return !(keyboard_buf.size == 0);
}
bool shift_mode = false;
bool control_mode = false;
bool alt_mode = false;
bool is_key_up = false;

// technically this doesn't fully decode the ps2 protocol;
// theres a few multibyte sequences it doesn't handle, but those aren't relevant inputs for the typing test
bool ps2_read_data(unsigned char* ch){
    if(!dequeue(&keyboard_buf, ch)) {
        return false; // no data
    };
    if (is_key_up == false) {
            if (*ch == 0xF0) {is_key_up = true; /* keyup */}
            else if (*ch == 0x12 || *ch == 0x59) {shift_mode = true; /* shift key*/}
            else if (*ch == 0x14) {control_mode = true; /* control key*/}
            else if (*ch == 0x11) {alt_mode = true; /* alt key*/}
            else {
                // normal keydown
                uint8_t mapped = shift_mode ? keycodes[*ch][1] : keycodes[*ch][0];
                *ch = mapped;
                return true;
            }
    } else{
        // keyup
        is_key_up = false;
        if(*ch == 0x12 || *ch == 0x59){
            shift_mode = false; // release shift
        }
        // ignore keyup codes for now
    }
    return false;
}

bool ps2_is_shifted(){
    return shift_mode;
}

bool ps2_is_control(){
    return control_mode;
}
bool ps2_is_alt(){
    return alt_mode;
}

// Data sent from the device to the host is read on the falling edge of the clock signal
// The clock frequency must be in the range 10 - 16.7 kHz

// ISR(INT0_vect){
//     serial_println("INT0");
//     char bit = PIND & (1 << 3) ? 1 : 0; // read data line (pd3)
//     return; // this is runnign even though nothing is on pin1...
// }


uint8_t bit_buffer[11];
// uint16_t time_buffer[11];
uint8_t bit_buffer_idx = 0;


uint16_t last_time = 0;

void OnClockLow(){
    // we can't trust bit_buffer_idx here being in a valid state, since interrupts can happen at any time
    // but we can assume keystrokes are spaced out enough that we can just reset if too much time has passed
    uint16_t current_time = TCNT1;
    uint16_t time_diff = current_time - last_time;
    
    // On first call or timeout, initialize/reset
    if(bit_buffer_idx == 0){
        last_time = current_time;
        time_diff = 0;
    } else if(time_diff > 2000){ // 2000 tcnt1 ticks = 2000 * 8 / 16MHz = 0.001s = 1ms
        bit_buffer_idx = 0; // reset
        last_time = current_time;
        time_diff = 0;
        // serial_println("\t PS2 Clock Timeout, resetting bit buffer index");
    }
    
    char bit = PS2_INPUT_PINS & (1 << PS2_PIN_DATA) ? 1 : 0; // read data line (pc3)

    bit_buffer[bit_buffer_idx++] = bit;

    if(bit_buffer_idx >= 11){
        // full packet received
        bit_buffer_idx = 0;
        last_time = TCNT1; // reset time for next byte

        // parse bits
        // start bit (should be 0)
        // data bits
        uint8_t data_byte = 0;
        uint8_t parity = 1;
        for(uint8_t i = 0; i < 8; i++){
            data_byte |= (bit_buffer[i + 1] << i);
            parity ^= bit_buffer[i + 1];
        }

        // parity bit
        uint8_t parity_bit = bit_buffer[9];
        // end bit (should be 1)
        
        // enqueue data byte
        enqueue(&keyboard_buf, data_byte);

        return; // too much debug output; todo: if this is actually required, perhaps ask for resend

        // if(bit_buffer[0] != 0){
        //     serial_println("PS2 Error: invalid start bit");
        //     serial_println("Received bits: ");
        //     for(uint8_t i = 0; i < 11; i++){
        //         serial_char(bit_buffer[i] ? '1' : '0');
        //     }
        //     // 0 0110 1000 1 0
        //     // 1 1000 1101 1 
        //     serial_println("");
        //     return;
        // }
        // if(bit_buffer[10] != 1){
        //     serial_println("PS2 Error: invalid stop bit");
        //     serial_println("Received bits: ");
        //     for(uint8_t i = 0; i < 11; i++){
        //         serial_char(bit_buffer[i] ? '1' : '0');
        //     }
        //     serial_println("");
        //     return;
        // }
        // if(parity != parity_bit){
        //     serial_println("PS2 Error: invalid parity bit");
        //     serial_println("Received bits: ");
        //     for(uint8_t i = 0; i < 11; i++){
        //         serial_char(bit_buffer[i] ? '1' : '0');
        //     }
        //     serial_println("");
        //     return; // 0 00001111 1 1
        // }

    }
    // serial_print("timediff: ");
    // serial_print((long)(time2 - time), 10, true);


}

uint16_t num = 0;
ISR(PCINT1_vect){
    static volatile uint8_t last = 0b00111111; // default high because pull-up
	uint8_t changed;
    uint8_t current = PS2_INPUT_PINS;

	changed = current ^ last;
	last = current;

    // serial_println("PCINT1 ISR");
    // serial_print("Changed: 0b");
    // debug_bin(changed);
    // serial_println("");

    // serial_print("PINC: 0b");
    // debug_bin(current);
    // serial_println("\n");
    


	if (changed & (1 << PS2_PIN_CLOCK)) { /* CLOCK changed */
		if ((PS2_INPUT_PINS & (1 << PS2_PIN_CLOCK)) == 0){
            _delay_us(10); // debounce
			OnClockLow();
        } /* CLOCK LOW */
	}
}

// states
// idle 
// device to host sending data (can either go back to idle, or get interrupted by host to device)
// host to device sending data (goes back to idle)
// host waiting for ack
// host waiting for cmd response (timeout? )
// error state 

// maybe use loop_until_bit_is_set? 
#endif /* KEYBOARD_H_ */

/*
blue wire is 3 ( vcc)
orange is 4 (ground)
green is 1 (data)
white is 5 (clock)
)

*/

/*

new one:

yellow is data
white is clk

red is vcc, black is gnd
*/