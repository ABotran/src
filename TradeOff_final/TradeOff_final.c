#include "elecanisms.h"
#include "ws2812b.h"

#define D0_OUT         D0        
#define D1_OUT         D1         
#define D2_OUT         D2        

typedef void (*STATE_HANDLER_T)(void);

void start(void);
void timer(void);
void lose(void);
void end(void);

STATE_HANDLER_T state, last_state;

uint16_t s;         //The lose state flash varible
uint16_t t;         //game timer
uint16_t color_index, position, num_full, num_half, num_off, i; //LED color indexing

// RGB values for sequence: red, green, blue, orange, pink, yellow
uint8_t red[6] = { 200, 0, 0, 250, 200, 200 };        
uint8_t green[6] = { 0, 200, 0, 50, 0, 160 };
uint8_t blue[6] = { 0, 0, 200, 0, 200, 0 };


void start(void) {
    if (state != last_state) {  // if we are entering the state, do initialization stuff
        last_state = state;
        disable_interrupts();
        ws2812b_write(0,60,0);
        ws2812b_write(0,60,0);
        ws2812b_write(0,60,0);
        ws2812b_write(0,60,0);
        ws2812b_write(0,60,0);
        enable_interrupts();
    }

    // Perform state tasks

    // Check for state transitions
    if (D0_OUT == 0) {
        state = timer;
    }

    if (state != last_state) {  // if we are leaving the state, do clean up stuff

    }
}

void timer(void) {
    if (state != last_state) {  // if we are entering the state, do intitialization stuff
        last_state = state;
        IFS0bits.T1IF = 0;
        TMR1 = 0;
        T1CONbits.TON = 1;
        t = 0;           //set game timer to 0
    }

    // Perform state tasks
    if (IFS0bits.T1IF == 1) {
        IFS0bits.T1IF = 0;
        t++;  //+1s to game timer

        color_index = (t - 1) / 10; // quotient of division of (t - 1) by 10.
        position = (t - 1) % 10;    // remainder of division of (t - 1) by 10.
        // number of full-brightness LED(s) is position (integer) divided by 2.
        num_full = (position + 1) >> 1;   // quotient of position divided by 2 (using right bit shift instead of divide).
        // if position is an even number, then there will be a half-brightness LED
        // after the last full-brightness LED(s) and before the off LED(s).
        num_half = 1 - (position % 2);    // remainder of position divided by 2 (using bitwise and to extract least-significant bit instead of modulus).
        num_off = 5 - num_full - num_half;

        disable_interrupts();
        // turn on full-brightness LED(s).
        for (i = 0; i < num_full; i++)
            ws2812b_write(red[color_index], green[color_index], blue[color_index]);

        // turn on half-brightness LED(s).
        if (num_half == 1)
            ws2812b_write(red[color_index] >> 3, green[color_index] >> 3, blue[color_index] >> 3);  // using right bit shift instead of divide by 2.

        // turn off the remaining LED(s).
        for (i = 0; i < num_off; i++)
            ws2812b_write(0, 0, 0);

        enable_interrupts();
    }

    // Check for state transitions
    if (D1_OUT == 1) {
        state = lose;
    } else if (t > 60) {
        state = lose;
    } else if (D2_OUT == 1) {
        state = end;
    } else if (D0_OUT == 1) {
        state = start;
    }

    if (state != last_state) {  // if we are leaving the state, do clean up stuff
        T1CONbits.TON = 0;
    }
}

void lose(void) {
    if (state != last_state) {  // if we are entering the state, do intitialization stuff
        last_state = state;
        PR1 = 6249.8;    // set Timer1 period to 0.1s
        IFS0bits.T1IF = 0;
        TMR1 = 0;
        T1CONbits.TON = 1;
        s = 1;
    }

    // Perform state tasks
    if (IFS0bits.T1IF == 1) {
        IFS0bits.T1IF = 0;
        if (s==1){
            s = 0;
            disable_interrupts();
            ws2812b_write(60,60,60);
            ws2812b_write(0,0,0);
            ws2812b_write(60,60,60);
            ws2812b_write(0,0,0);
            ws2812b_write(60,60,60);
            enable_interrupts();
            } else {
                s = 1;
                disable_interrupts();
                ws2812b_write(0,0,0);
                ws2812b_write(60,60,60);
                ws2812b_write(0,0,0);
                ws2812b_write(60,60,60);
                ws2812b_write(0,0,0);
                enable_interrupts();
            }
    }

    // Check for state transitions
    if (D0_OUT == 1) {
        state = start;
    }

    if (state != last_state) {  // if we are leaving the state, do clean up stuff
        T1CONbits.TON = 0;
        PR1 = 62498;         // set Timer1 period to 1s
    }
}

void end(void) {
    if (state != last_state) {  // if we are entering the state, do initialization stuff
        last_state = state;
        
        uint16_t color_index, position, num_full, num_half, num_off, i;

        color_index = (t - 1) / 10; // quotient of division of (t - 1) by 10.
        position = (t - 1) % 10;    // remainder of division of (t - 1) by 10.
        // number of full-brightness LED(s) is position (integer) divided by 2.
        num_full = (position + 1) >> 1;   // quotient of position divided by 2 (using right bit shift instead of divide).
        // if position is an even number, then there will be a half-brightness LED
        // after the last full-brightness LED(s) and before the off LED(s).
        num_half = 1 - (position % 2);    // remainder of position divided by 2 (using bitwise and to extract least-significant bit instead of modulus).
        num_off = 5 - num_full - num_half;
        
        disable_interrupts();
        // turn on full-brightness LED(s).
        for (i = 0; i < num_full; i++)
            ws2812b_write(red[color_index], green[color_index], blue[color_index]);

        // turn on half-brightness LED(s).
        if (num_half == 1)
            ws2812b_write(red[color_index] >> 3, green[color_index] >> 3, blue[color_index] >> 3);  // using right bit shift instead of divide by 2.

        // turn off the remaining LED(s).
        for (i = 0; i < num_off; i++)
            ws2812b_write(0, 0, 0);
        enable_interrupts();
    }

    // Perform state tasks (nothing)
        
    // Check for state transitions
    if (D0_OUT == 1) {
        state = start;
    }

    if (state != last_state) {  // if we are leaving the state, do clean up stuff
 
    }
}

int16_t main(void) {
    init_elecanisms();
    init_ws2812b();

    D0_PD = ON;
    D1_PD = ON;
    D2_PD = ON;

    T1CON = 0x0030;         // set Timer1 period to 1s
    PR1 = 62498;
    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag

    state = start;

    last_state = (STATE_HANDLER_T)NULL;

    while (1) {
        state();
    }
}