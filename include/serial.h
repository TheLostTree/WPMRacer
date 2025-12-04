#ifndef SerialAtmega
#define SerialAtmega

#include <avr/io.h>
#include <avr/interrupt.h>
// #include <stdarg.h>
// #include <stdio.h>

/*
Modified serial at mega to add more logging options :D
*/



void serial_init (unsigned long baud ) {
    UBRR0 = (((16000000/(baud*16UL)))-1) ; // Set baud rate

    // enable transmitter & reciever 
    UCSR0B |= (1 << TXEN0 ) | (1 << RXEN0 ); 
  
    // ? why does this enable then disable the interrupt? instead of just disabling it alltogether
    UCSR0B |= (1 << RXCIE0 ); // 0b10000000 //0b01111111
    UCSR0B &= ~(1 << RXCIE0 );

    // set frame format: 8 data, no stop bit? idk
    UCSR0C = (3 << UCSZ00 ) | (1<<USBS0); 
    // UBRR0
}



bool serial_peek(){
  return (UCSR0A & 1 << RXC0); // rxc0 == 1 indicates there is something
}

// used for error cond
// void serial_flush(void)
// {
//   unsigned char dummy;
//   while (UCSR0A & (1<<RXC0)) dummy = UDR0;
// }

unsigned char serial_read_char(){
  while (!(UCSR0A & (1<<RXC0)));
  return UDR0;
}


//sends a char
void serial_char(char ch )
{
    // busy wait until UDRE0 1
    while (( UCSR0A & (1 << UDRE0 )) == 0);
    UDR0 = ch ;
}



//sends a string


void serial_print(const char *str){
    for (int i; str[i] != '\0'; i++){
        serial_char(str[i]);
    }
}
void serial_print(const char *str, uint16_t len){
    for (int i; i < len; i++){
        serial_char(str[i]);
    }
}

void serial_println(const char *str){
    serial_print(str);
    serial_char('\n');
}

/*
void serial_printf(const char *format, ...){
    // very basic printf implementation
    va_list args;
    va_start(args, format);
    char buffer[128];
    int ret = snprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    serial_print(buffer);
}
*/

void serial_println(){
    serial_char('\n');
}

//sends an long. can be used with integers
void serial_println(long num, int base = 10){
  char arr[sizeof(long)*8 + 1]; //array with size of largest possible number of digits for long
  char *str = &arr[sizeof(arr) - 1]; //point to last val in buff
  *str = '\0'; //set last val in buff to null terminator

  if(num < 0){ //if negative, print '-' and turn n to positive
    serial_char('-');
    num = -num;
  }

  if(num == 0){// if 0, print 0
    serial_char(48);
  }else{//else, fill up arr starting from the last number
    while(num) {
        char temp = num % base;//get digit
        num /= base;//shift to next digit
        str--;//go back a spot in arr
        *str = temp < 10 ? temp + '0' : temp + 'A' - 10; // "+ A - 10" for A-F hex vals
    }
  }

  serial_println(str);//print from str to end of arr
}

//sends an long. can be used with integers
void serial_print(long num, int base = 10, bool newline = true){
  char arr[sizeof(long)*8 + 1]; //array with size of largest possible number of digits for long
  char *str = &arr[sizeof(arr) - 1]; //point to last val in buff
  *str = '\0'; //set last val in buff to null terminator

  if(num < 0){ //if negative, print '-' and turn n to positive
    serial_char('-');
    num = -num;
  }

  if(num == 0){// if 0, print 0
    serial_char(48);
  }else{//else, fill up arr starting from the last number
    while(num) {
        char temp = num % base;//get digit
        num /= base;//shift to next digit
        str--;//go back a spot in arr
        *str = temp < 10 ? temp + '0' : temp + 'A' - 10; // "+ A - 10" for A-F hex vals
    }
  }

  if(newline) {
    serial_println(str);//print from str to end of arr
  }else{
    serial_print(str);
  } 
}


void debug_bin(unsigned char byte){
    for(int i = 7; i >= 0; i--){
        serial_char( ( (byte >> i) & 0x01 ) ? '1' : '0' );
    }
}


// comment the following line if one decides to need println with doubles
/*
char buffer[50];
// not a very sophisticated or safe method to print doubles but this is very much for debugging
void serial_println(double num, int decimals = 3){
    long long integer_part = (long long)num;
    double fractional_part = num - integer_part;
    if(decimals > 20) decimals = 20;

    // Convert integer part to string
    int i = 0;
    if (integer_part == 0) {
        buffer[i++] = '0';
    } else {
        long long temp = integer_part;
        int start = i;
        while (temp > 0) {
            buffer[i++] = (temp % 10) + '0';
            temp /= 10;
        }
        // Reverse the integer part
        int j = start;
        int k = i - 1;
        while (j < k) {
            char t = buffer[j];
            buffer[j] = buffer[k];
            buffer[k] = t;
            j++;
            k--;
        }
    }

    buffer[i++] = '.'; // Add decimal point

    if(i + decimals > 48) decimals = 48 - i; // just to be safe 

    // Convert fractional part to string (simplified for a few decimal places)
    for (int k = 0; k < decimals; k++) { // Print decimals amt of decimal places
        fractional_part *= 10;
        int digit = (int)fractional_part;
        buffer[i++] = digit + '0';
        fractional_part -= digit;
    }
    buffer[i] = '\0'; // Null-terminate the string
    serial_println(buffer);
}

// */
#endif