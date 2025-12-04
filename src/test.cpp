/* avr_ps2 example - turn on led on PC2 with <enter>, off with <space>
 * see configuration in conf.h */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "serial.h"

#define PS2_PORT PORTC
#define PS2_PIN PINC
#define PS2_DDR DDRC
#define PS2_PCIE PCIE1
#define PS2_PCINT PCINT8
#define PS2_PCINT_vect PCINT2_vect
#define PS2_PIN_CLOCK 0
#define PS2_PIN_DATA 1

#define PS2_SHIFT_DEFAULT 0 /* if set, activate shift mode by default */
#define PS2_DEBUG 0         /* if set, store scan-code in buffer instead of ascii code */

#define PS2_BUF_LEN 128

static void _decode(unsigned char);

unsigned char _buf[PS2_BUF_LEN]; /* only used within ATOMIC_BLOCK */
uint8_t _bufbeg = 0;             /* only used within ATOMIC_BLOCK */
uint8_t _bufend = 0;             /* only used within ATOMIC_BLOCK */

extern unsigned char _unshifted[67][2];
extern unsigned char _shifted[67][2];

void ps2_init(void) {
    PS2_DDR &= ~(1 << PS2_PIN_CLOCK); /* set pin CLOCK as input */
    PS2_DDR &= ~(1 << PS2_PIN_DATA);  /* set pin DATA as input */
    PS2_PORT |= (1 << PS2_PIN_CLOCK); /* enable pin CLOCK pull-up */
    PS2_PORT |= (1 << PS2_PIN_DATA);  /* enable pin DATA pull-up */
    PCICR |= (1 << PS2_PCIE);         /* set PCIE1 to enable PCMSK1 scan */
    PCMSK1 |= (1 << PS2_PCINT);       /* set PCINT8 to trigger interrupts */
    // PCMSK1 = 0b00111111;       /* set all PCINT1 pins to trigger interrupts */
}

/* ps2_read - called from interrupt vector */
void ps2_read(void) {
    static unsigned char data;
    static unsigned char bitcount = 11;

    /* bit 3 to 10 is data, start, stop & parity bis are ignored */
    if ((bitcount < 11) && (bitcount > 2)) {
        /* data bit */
        data = (data >> 1);
        if ((PINC & (1 << PS2_PIN_DATA))) /* read DATA pin */
            data = data | 0x80;           /* store '1' */
        else
            data = data & 0x7f; /* store '0' */
    }
    if (--bitcount == 0) {
        /* all bits received */
        _decode(data);
        bitcount = 11;
    }
}

/* ps2_buf_push - append a char to the pressed keys buffer
 * typically called in PCINT interrupt */
void ps2_buf_push(unsigned char c) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        _buf[_bufend] = c;
        if (_bufend + 1 == PS2_BUF_LEN)
            _bufend = 0;
        else
            _bufend++;
        if (_bufend == _bufbeg) { /* buffer full, remove a char */
            if (_bufbeg + 1 == PS2_BUF_LEN)
                _bufbeg = 0;
            else
                _bufbeg++;
        }
    }
}

/* ps2_buf_pull - fetch a char from the pressed keys buffer
 * typically called outside of interrupts */
unsigned char
ps2_buf_pull(void) {
    unsigned char c;

    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        if (_bufbeg == _bufend)
            return 0;
        c = _buf[_bufbeg];
        if (_bufbeg + 1 == PS2_BUF_LEN)
            _bufbeg = 0;
        else
            _bufbeg++;
    }
    return c;
}

/* ps2_buf_empty - empty pressed keys buffer */
void ps2_buf_empty(void) {
    _bufbeg = 0;
    _bufend = 0;
}

/* decode PS2 scan code */
static void _decode(unsigned char sc) {
    static unsigned char is_up = 0;
    static unsigned char shift = PS2_SHIFT_DEFAULT;
    unsigned char i;

    if (!is_up) {
        switch (sc) {
            case 0xF0: /* up-key */
                is_up = 1;
                break;
            case 0x12: /* left shift */
                shift = 1;
                break;
            case 0x59: /* right shift */
                shift = 1;
                break;
            default:
#if PS2_DEBUG
                ps2_buf_push(sc);
#else
                if (!shift) {
                    for (i = 0; _unshifted[i][0] != sc && _unshifted[i][0]; i++);
                    if (_unshifted[i][0] == sc) {
                        ps2_buf_push(_unshifted[i][1]);
                    }
                } else {
                    for (i = 0; _shifted[i][0] != sc && _shifted[i][0]; i++);
                    if (_shifted[i][0] == sc) {
                        ps2_buf_push(_shifted[i][1]);
                    }
                }
#endif /* PS2_DEBUG */
                break;
        }
    } else {
        is_up = 0; /* two 0xF0 in a row not allowed */
        switch (sc) {
            case 0x12: /* left shift */
                shift = 0;
                break;
            case 0x59: /* right shift */
                shift = 0;
                break;
        }
    }
}

unsigned char _unshifted[67][2] = {
    {0x0e, '`'}, {0x15, 'q'}, {0x16, '1'}, {0x1a, 'z'}, {0x1b, 's'}, {0x1c, 'a'}, {0x1d, 'w'}, {0x1e, '2'}, {0x21, 'c'}, {0x22, 'x'}, {0x23, 'd'}, {0x24, 'e'}, {0x25, '4'}, {0x26, '3'}, {0x29, ' '}, {0x2a, 'v'}, {0x2b, 'f'}, {0x2c, 't'}, {0x2d, 'r'}, {0x2e, '5'}, {0x31, 'n'}, {0x32, 'b'}, {0x33, 'h'}, {0x34, 'g'}, {0x35, 'y'}, {0x36, '6'}, {0x39, ','}, {0x3a, 'm'}, {0x3b, 'j'}, {0x3c, 'u'}, {0x3d, '7'}, {0x3e, '8'}, {0x41, ','}, {0x42, 'k'}, {0x43, 'i'}, {0x44, 'o'}, {0x45, '0'}, {0x46, '9'}, {0x49, '.'}, {0x4a, '/'}, {0x4b, 'l'}, {0x4c, ';'}, {0x4d, 'p'}, {0x4e, '-'}, {0x52, '`'}, {0x54, '['}, {0x55, '='}, {0x5a, 13}, {0x5b, ']'}, {0x5d, '/'}, {0x61, '<'}, {0x66, 8}, {0x69, '1'}, {0x6b, '4'}, {0x6c, '7'}, {0x70, '0'}, {0x71, ','}, {0x72, '2'}, {0x73, '5'}, {0x74, '6'}, {0x75, '8'}, {0x79, '+'}, {0x7a, '3'}, {0x7b, '-'}, {0x7c, '*'}, {0x7d, '9'}, {0, 0}};

unsigned char _shifted[67][2] = {
    {0x0e, '`'}, {0x15, 'Q'}, {0x16, '!'}, {0x1a, 'Z'}, {0x1b, 'S'}, {0x1c, 'A'}, {0x1d, 'W'}, {0x1e, '@'}, {0x21, 'C'}, {0x22, 'X'}, {0x23, 'D'}, {0x24, 'E'}, {0x25, '$'}, {0x26, '#'}, {0x29, ' '}, {0x2a, 'V'}, {0x2b, 'F'}, {0x2c, 'T'}, {0x2d, 'R'}, {0x2e, '%'}, {0x31, 'N'}, {0x32, 'B'}, {0x33, 'H'}, {0x34, 'G'}, {0x35, 'Y'}, {0x36, '^'}, {0x39, 'L'}, {0x3a, 'M'}, {0x3b, 'J'}, {0x3c, 'U'}, {0x3d, '&'}, {0x3e, '*'}, {0x41, '<'}, {0x42, 'K'}, {0x43, 'I'}, {0x44, 'O'}, {0x45, ')'}, {0x46, '('}, {0x49, '>'}, {0x4a, '?'}, {0x4b, 'L'}, {0x4c, ':'}, {0x4d, 'P'}, {0x4e, '_'}, {0x52, '"'}, {0x54, '{'}, {0x55, '+'}, {0x5a, 13}, {0x5b, '}'}, {0x5d, '|'}, {0x61, '>'}, {0x66, 8}, {0x69, '1'}, {0x6b, '4'}, {0x6c, '7'}, {0x70, '0'}, {0x71, ','}, {0x72, '2'}, {0x73, '5'}, {0x74, '6'}, {0x75, '8'}, {0x79, '+'}, {0x7a, '3'}, {0x7b, '-'}, {0x7c, '*'}, {0x7d, '9'}, {0, 0}};

int main(void) {
    unsigned char c;

    serial_init(57600);

    ps2_init();
    DDRC |= (1 << PC2); /* set PC2 as output, for LED */
    sei();

    serial_println("PS2 Keyboard Test Program");

    while (1) {
        c = ps2_buf_pull();
        if (c != 0) {
            serial_char(c);

            if (c == '\r')
                PORTC |= (1 << PC2); /* LED on */
            else if (c == ' ')
                PORTC &= ~(1 << PC2); /* LED off */
        }
        _delay_ms(100);
        serial_println("still running");

    }
}

#define log_isr(name) ISR(name){ serial_println(#name); }
#define isr_vector(num) log_isr(_VECTOR(num))

isr_vector(1);
isr_vector(2);
isr_vector(3);
isr_vector(4);
// isr_vector(5);
isr_vector(6);
isr_vector(7);
isr_vector(8);
isr_vector(9);
isr_vector(10);
isr_vector(11);
isr_vector(12);
isr_vector(13);
isr_vector(14);
isr_vector(15);
isr_vector(16);
isr_vector(17);
isr_vector(18);
isr_vector(19);
isr_vector(20);
isr_vector(21);
isr_vector(22);
isr_vector(23);
isr_vector(24);
isr_vector(25);

ISR(PS2_PCINT_vect) {       
    serial_println("PCINT1 ISR");
                 /* PS2 interrupt */
    static volatile uint8_t last = 0xFF; /* default high because pull-up */
    uint8_t changed;


    changed = PS2_PIN ^ last;
    last = PS2_PIN;

    if (changed & (1 << PS2_PIN_CLOCK)) {          /* CLOCK changed */
        if ((PS2_PIN & (1 << PS2_PIN_CLOCK)) == 0) /* CLOCK LOW */
            ps2_read();
    }
}
