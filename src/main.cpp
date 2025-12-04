#include <avr/io.h>
#include <avr/eeprom.h>
#include "serial.h"
#include "lcd.h"
#include "task_scheduler.h"
#include "wordlist.h"
#include "keyboard.h"
#include "queue.h"


Queue serial_input_buf;
uint8_t data[16]; // surely this is enough

xorshift32 rng;

// EEPROM storage for high score (32-bit float)
float EEMEM eeprom_high_score = 0.0f;
float high_score = 0.0f;

#pragma region DisplayTask
volatile unsigned char ascii_char = '\0';
unsigned char line = 0;
unsigned char pos = 0;
int inited = 0;

bool display_needs_cleared = false;

// this doesnt quite work right.

void lcd_next_line(){
    line = line + 1;
    pos = 0;
    if(line >= 2){
        line = 0;
        //todo: figure something better than a flag tbh...
        display_needs_cleared = true;
    }

    
    _delay_us (50); // 50 ish us here
    lcd_goto_xy(line, pos); // 60 ish us here

}
// todo: handle backspace (8)
void lcd_write_and_advance(char ch, int reset_line = 0){

    // ignore
    if(ch == '\r'){
        return;
    }

    if(ch == '\n'){
        lcd_next_line();
        return;
    }

    if(ch == 8){ // backspace
        if(pos > 0){
            pos--;
            lcd_goto_xy(line, pos);
            lcd_write_character(' ');
            _delay_us(50);
            lcd_goto_xy(line, pos);
        }
        return;
    }



    // writing to pos, line
    // serial_print("Writing char '");
    // serial_char(isprint(ch) ? ch : '?');
    // serial_print("' at line ");
    // serial_print((long)line, 10, false);
    // serial_print(", pos ");
    // serial_print((long)pos, 10, true);

    // cycle_log_checkpoint(14);
    if(display_needs_cleared){
        lcd_write_into_hidden("",0,0);
        lcd_write_into_hidden("",1,0);
        line = reset_line;
        pos = 0;
        display_needs_cleared = false;
    }

    // cycle_log_checkpoint(15);
    lcd_write_character(ch);
    // cycle_log_checkpoint(16);
    pos++;

    if(pos >= 16){
        lcd_next_line();
    }
    // cycle_log_checkpoint(17);
}

void DisplayTaskTick(void * this_task){
    
}

// particular characters:
// \ => yen for some reason
// ~ => -> 
// del => <-
#pragma endregion


#pragma region SerialInputTask
void SerialInputTaskTick(void * this_task){
    if(!serial_peek()) return;
    char ch = serial_read_char();
    if(ch) {
        enqueue(&serial_input_buf, ch);
    }
}
#pragma endregion


uint16_t levenshtein_diff(const char *a, const uint16_t len, const char *b, const uint16_t b_len) {
  static uint16_t cache[256]; // max length 256, since our test_str is 255 chars
  uint16_t a_idx = 0;
  uint16_t b_idx = 0;
  uint16_t a_dist;
  uint16_t b_dist;
  uint16_t result;
  char code;

  // initialize the vector.
  while (a_idx < len) {
    cache[a_idx] = a_idx + 1;
    a_idx++;
  }

  
  while (b_idx < b_len) {
    code = b[b_idx];
    result = a_dist = b_idx++;
    a_idx = SIZE_MAX;

    while (++a_idx < len) {
      b_dist = code == a[a_idx] ? a_dist : a_dist + 1;
      a_dist = cache[a_idx];

      cache[a_idx] = result = a_dist > result
        ? b_dist > result
          ? result + 1
          : b_dist
        : b_dist > a_dist
          ? a_dist + 1
          : b_dist;
    }
  }
  return result;
}



float get_accuracy(const char* test_str, const char* player_str, uint16_t player_idx){
    // levenshtein distance between test_str and player_str up to player_idx
    // return 1.0 - (distance / max_len) :D 
    // or (max_len - distance) / max_len
    // when i started this project i did not realize i signed myself up for a dsa problem but oh well
    float distance = (float) levenshtein_diff(test_str, player_idx, player_str, player_idx);

    return 1.0 - (distance / (float)player_idx); // TODO
}


void generate_test_string(char * test_str, size_t max_len, xorshift32* rng){
    // generate a long string of words from the wordlist
    size_t current_len = 0;
    test_str[0] = '\0';
    while(current_len < max_len - 10){ // leave some buffer
        get_word(next_rand32(rng) & 0xFF, test_str + current_len, max_len - current_len);
        size_t word_len = strlen(test_str + current_len);
        current_len += word_len;
        word_len && ((test_str[current_len] = ' '), current_len++);
    }
    if(current_len > 0){
        test_str[current_len - 1] = '\0'; // replace last space with null
    } else {
        test_str[max_len - 1] = '\0';
        // serial_println("Warning: test string generation failed, no words added.");
    }
}

uint16_t get_wrd_cnt(const char* str, size_t max_len){
    uint16_t word_cnt = 0;
    bool in_word = false;
    for(size_t i = 0; i < max_len; i++){
        if(str[i] != ' ' && !in_word){
            in_word = true;
            (word_cnt)++;
        } else if(str[i] == ' '){
            in_word = false;
        }
    }
    return word_cnt;
}


#define test_str_len 255
char test_str[255] = "port hand differ four and play not with run put that your let large to with close sea what put over run draw part answer cross make do little as call look will been press eye house are real than their was each go when here hand real is at look find year";


uint16_t test_idx = 0;
uint16_t test_word_start_idx = 0;
uint16_t test_word_len = 0;
const uint16_t test_len = 255;

char player_str[255];

// char player_buf[32];
uint16_t player_idx = 0;
const uint16_t player_len = 32;



#pragma region WordGameTask

enum WordGameState {
    GAME_INIT,
    WAITING_TO_START,
    PLAYING,
    PRE_GAME_OVER, // times up! screen
    GAME_OVER // score screen
};
WordGameState word_game_state = GAME_INIT;
uint16_t game_time = 0;
#define GAME_DURATION_TICKS (6000) // 10ms per tick 
// #define GAME_DURATION_TICKS (600) // 10ms per tick 


enum PlayingStates{
    PlayingInit,
    PlayingIdle,
    PlayingOnKeyDown,
};

volatile PlayingStates playing_state = PlayingInit;
PlayingStates PlayingFSMTick(PlayingStates state){
    switch(state){
        case PlayingInit:
            lcd_clear();

            lcd_write_into_hidden(test_str, 0);
            test_idx = 0;
            lcd_goto_xy(1,0);
            break;
        case PlayingIdle:
             break;
        case PlayingOnKeyDown:
            break;
    }


    // serial_print(game_time, 10, false);
    // serial_print(" ");
    // serial_print(state, 10, false);
    // serial_println(" playingfsmtick");



    PlayingStates nextstate = state;
    uint8_t ch;
    switch(state){
        case PlayingInit:
            nextstate = PlayingIdle;
            break;
        case PlayingIdle:
            if(dequeue(&serial_input_buf, &ch)){
                // lcd_shift_left();
                // write test words
                lcd_write_into_hidden(test_str + test_idx, 0, test_len - test_idx);
                // test_str + test_idx is the start of whatever is being displayed
                test_idx++;

                serial_char(ch);
                // serial_print((long)test_str + test_idx, 16, true);
                // serial_print((long)test_str, 16, true);
                line = 1;
                lcd_goto_xy(line, pos);

                if(ch == '\b'){
                    // backspace
                    if(player_idx > 0){
                        player_idx--;
                        player_str[player_idx] = '\0'; 
                    }
                }

                lcd_write_and_advance(ch, 1);
                player_str[player_idx++] = ch; 

                if(ch == ' '){
                    // clear pos so we start next word fresh, playeridx stays same so we can cmp later
                    pos = 0;
                    lcd_write_line("                ", 1, 16);
                    lcd_goto_xy(1,pos);
                }
            }
            break;
        case PlayingOnKeyDown:
            break;
        default:
            nextstate = PlayingIdle;
    }

    if(player_idx == test_len){
        // finished all words
        word_game_state = PRE_GAME_OVER;
        // serial_println("\nGame Over!");
        lcd_clear();
        lcd_write_line("All words done!", 0, 15);
        lcd_write_line("Enter for Score", 1, 15);
    }
    if(game_time >= GAME_DURATION_TICKS){
        word_game_state = PRE_GAME_OVER;
        // serial_println("\nGame Over!");
        lcd_clear();
        lcd_write_line("Time's up!", 0, 10);
        lcd_write_line("Enter for Score", 1, 15);
        //todo: score calculation:
    }
    game_time++;
    return nextstate;
    
}


// really only meant for rendring numbers from 0 to 1 as percentages

void render_float(float percent, char* buf, size_t buf_size, bool percent_sign = false){
    // percent is 0.0 to 1.0
    // ideal: "100.00%"
    if(buf_size < 5) return; // need at least 5 chars
    int int_percent = (int)(percent * 10000.0f); // 0 to 10000
    int whole = int_percent / 100;
    int frac = int_percent % 100;
    // no snprintf on this platform :(
    buf[0] = (whole / 100) % 10 + '0';
    buf[1] = (whole / 10) % 10 + '0';
    buf[2] = (whole % 10) + '0';
    buf[3] = '.';
    buf[4] = (frac / 10) % 10 + '0';
    buf[5] = (frac % 10) + '0';
    if(percent_sign){
        buf[6] = '%';
        buf[7] = '\0';
    } else{
        buf[6] = '\0';
    }
    // 7 chars!
}

// Load high score from EEPROM
void load_high_score(){

    eeprom_high_score = 0;
    high_score = eeprom_read_float(&eeprom_high_score);
}

// Save high score to EEPROM
void save_high_score(){
    eeprom_write_float(&eeprom_high_score, high_score);
}

// Calculate final score: wpm * accuracy
float calculate_final_score(float wpm, float accuracy){
    return wpm * accuracy;
}

void write_buf(char* buf, const char* str, size_t len){
    for(size_t i = 0; i < len; i++){
        buf[i] = str[i];
    }
}
void WordGameTaskTick(void * this_task){

    // state actions
    switch (word_game_state)
    {
        case GAME_INIT:
        {
            char buf[17] = {0};
            write_buf(buf, "HS: ", 4);
            render_float(high_score/100.0f, buf + 4, sizeof(buf) - 4);
            lcd_write_line(buf, 0);
            lcd_write_line("> Press Enter   ", 1);
            lcd_goto_xy(1,1);
            break;
        }
        case WAITING_TO_START:
            // nothing yet
            break;
        case PLAYING:
            playing_state = PlayingFSMTick(playing_state);
            break;
        case GAME_OVER:
            break;
    }
    uint8_t ch;


    //transitions
    switch (word_game_state)
    {
        case GAME_INIT:
            word_game_state = WAITING_TO_START;
            break;
        case WAITING_TO_START:
            while(dequeue(&serial_input_buf, &ch)){
                if(ch == '\n' || ch == '\r'){
                    word_game_state = PLAYING;
                    playing_state = PlayingInit;
                    game_time = 0;  
                    // serial_println("Game Started!");
                    generate_test_string(test_str, sizeof(test_str), &rng);

                    break;
                }
            }
            break;
        case PLAYING:
            
            break;
        case PRE_GAME_OVER:
            while(dequeue(&serial_input_buf, &ch)){
                if(ch == '\n' || ch == '\r'){
                    word_game_state = GAME_OVER;
                    // calculate score
                    float accuracy = get_accuracy(test_str, player_str, player_idx);
                    // 5 words in 1 min = 5 wpm
                    // bc GAME_DURATION_TICKS is 1200*10ms = 12s and 60s/12s = 5n
                    uint16_t word_cnt = get_wrd_cnt(player_str, player_idx);
                    float wpm = (float)word_cnt / ((float)GAME_DURATION_TICKS / 6000.0f); // 6000 ticks in 1 min
                    float final_score = calculate_final_score(wpm, accuracy);
                    serial_print("\nFinal Score: ");
                    serial_print((double)final_score);
                    serial_print((double) high_score);
                    
                    // Check if new high score
                    if(final_score > high_score){
                        high_score = final_score;
                        serial_println(" New High Score!");
                        //display new high score message

                        
                        lcd_clear();
                        lcd_write_line("New High Score!", 0, 16);
                        lcd_write_line("Congrats!", 1, 9);
                        _delay_ms(2000); // pause to show message (hack... but oh well)
                        // Save new high score to EEPROM
                        save_high_score();
                    } else{
                        serial_println("");

                    }
                    
                    // serial_print("word cnt: ");
                    // serial_print(word_cnt, 10, true);
                    // serial_print((long)(wpm * 100.0f), 10, false);
                    // serial_println(" / 100");
                    // serial_print("Accuracy: ");
                    // serial_print((long)(accuracy * 10000.0f), 10, false);
                    // serial_println(" / 10000");
                    lcd_clear();
                    char buf[17] = {0}; // does this correctly zero initialize? 
                    write_buf(buf, "WPM: ", 5);
                    render_float(wpm / 100.0f, buf + 5, sizeof(buf) - 5);
                    lcd_write_line(buf, 0, 16);

                    lcd_goto_xy(0,15);
                    lcd_write_character(wpm >= 30.0f ? 0 : 1); // smiley or frowny based on wpm

                    write_buf(buf, "Acc: ", 5);
                    render_float(accuracy, buf + 5, sizeof(buf) - 5, true);
                    lcd_write_line(buf, 1, 16);
                    lcd_goto_xy(1,15);
                    lcd_write_character(accuracy >= 0.7f ? 0 : 1); // smiley or frowny based on accuracy
                    break;
                }
            }
            break;
        case GAME_OVER:
            // wait for reset
            uint8_t ch;
            while(dequeue(&serial_input_buf, &ch)){
                if(ch == '\n' || ch == '\r'){
                    word_game_state = GAME_INIT;
                    break;
                }
            }
            break;
    }
}
#pragma endregion


#pragma region EchoTask
void EchoTaskTick(void * this_task){
    uint8_t ch;
    while(dequeue(&serial_input_buf, &ch)){
        if(ch == '\r'){
            ch = '\n';
        }
        serial_char(ch);
    }
}
#pragma endregion



#pragma region ISRDebuggingCursed
/*
#define log_isr(name) ISR(name){ serial_println(#name); }
#define isr_vector(num) log_isr(_VECTOR(num))

isr_vector(1);
isr_vector(2);
isr_vector(3);
// isr_vector(4);
isr_vector(5);
isr_vector(6);
// isr_vector(7);
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
// this is really dumb lmfao

ISR(BADISR_vect){
    serial_println("BADISR_vect");
}
*/
#pragma endregion


// 0 to 7, 8 to 14, 16 to 23 for PCINT0
void test_init(){
    // input on pc2
    DDRC &= ~(1 << PC2);
    PORTC |= (1 << PC2); // pullup


    // setup pc2 change interrupt
    // pc2 = PCINT10
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10); 
}


#pragma region SlowTestTask
void SlowTestTaskTick(void * this_task){
    //beeeeg number
    static char buf[50];
    static xorshift32 shift = {0b10001010010110101001100101011011};
    uint8_t num = next_rand32(&shift) & 0xFF; // 0-255  
    get_word(num, buf, 50);
    serial_print(buf);
    serial_print(" ");
}
#pragma endregion


#pragma region PS2KeyboardInputTask
void PS2KeyboardInputTaskTick(void * this_task){
    // check for ps2 data
    if(ps2_data_available()){
        uint8_t ch;
        if(ps2_read_data(&ch)){
            serial_print("PS2 Data: 0x");
            serial_print((long)ch, 16, true);
            serial_print("\n");
            enqueue(&serial_input_buf, ch); // this be getting passed around bro
        }
        // serial_print("PS2 Data: 0x");
        // serial_print((long)ch, 16, true);
        // serial_print("\n");
    }
}
#pragma endregion


uint8_t customChar[2][8] = {
 {
    // smiley
    0b00000,
    0b01010,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000
 },
 {  // frowny
    0b00000,
    0b01010,
    0b01010,
    0b01010,
    0b00000,
    0b01110,
    0b10001,
    0b00000
 }
};



void init_everything(){
    serial_init(57600);
    lcd_init();

    // Load high score from EEPROM
    load_high_score();

    //todo: custom chars; need to wireup the rs 

    lcd_define_custom_char(0, customChar[0]); // smiley
    lcd_define_custom_char(1, customChar[1]); // frowny

    _delay_ms(100);
    lcd_clear();
    _delay_ms(100);

    keyboard_init();
    // test_init();

    init_queue(&serial_input_buf, data, sizeof(data));
    // perf_checker_init();
    init_task_scheduler(10); // 10 ms gcd tick
}



int main(){

    DDRC = 0x00; PORTC = 0xFF; // all inputs on c
    DDRB = 0x00; PORTB = 0xFF; // all inputs on b
    DDRD = 0xFF; PORTD = 0x00;
    // eeprom_high_score = 0.0f; // initialize eeprom location
    // save_high_score();

    init_everything();

    //using the bits of our highscore float as rng seed!
    uint32_t num = (*(uint32_t*)&high_score);

    serial_println("Initialized.");


    

    rng = { num + 0b10101011101011101011100010101101}; // seed rng with high score bits + some noise
    // serial_println("Generated test string:");
    serial_println(test_str);


    // log_all();
    // __vector_4(); // test 
    // serial_println("TESTING EN RS");
    // test_en_rs();
    // serial_println("TEST COMPLETE.");


    // tasks will run from last added to first added
    addTask(50, 0xFF, DisplayTaskTick); 
    addTask(10, 0xFF, SerialInputTaskTick);
    // addTask(10, 0xFF, EchoTaskTick);

    addTask(10, 0xFF, WordGameTaskTick);

    // addTask(500, 0xFF, SlowTestTaskTick);
    addTask(1, 0xFF, PS2KeyboardInputTaskTick);

    start_task_scheduler();




    // todo: see if i can sleep and just have isrs wake up the cpu
    // https://gist.github.com/JChristensen/5616922 perhaps?
    while (1) {}
    return 0;
}
/*
1602 display is on pins pd4-pd7 (data) and pd3,pd2 (ctl)
todo: move ctl to portb, so we can use pd2 and pd3 for ps2 clock and data (INT0)


*/


//todo : use  ATOMIC_BLOCK(ATOMIC_FORCEON) {} where appropriate to avoid isr interference (mostly where variables are multibyte)

// todo: if u read PINx right after writing to DDRx, add a noop or small delay, to let the hardware catch up.