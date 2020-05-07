#include "elecanisms.h"
#include "ws2812b.h"

// RGB values for sequence: red, green, blue, cyan, magenta, yellow
uint8_t red[6] = { 255, 0, 0, 0, 255, 255 };
uint8_t green[6] = { 0, 255, 0, 255, 0, 255 };
uint8_t blue[6] = { 0, 0, 255, 255, 255, 0 };

// Parameter t is assumed to be in the range of 1 to 60
void display_time(uint16_t t) {
    uint16_t color_index, position, num_full, num_half, num_off, i;

    color_index = (t - 1) / 10; // quotient of division of (t - 1) by 10.
    position = (t - 1) % 10;    // remainder of division of (t - 1) by 10.

    // number of full-brightness LED(s) is position (integer) divided by 2.
    num_full = position >> 1;   // quotient of position divided by 2 (using right bit shift instead of divide).

    // if position is an even number, then there will be a half-brightness LED
    // after the last full-brightness LED(s) and before the off LED(s).
    num_half = position % 2;    // remainder of position divided by 2 (using bitwise and to extract least-significant bit instead of modulus).

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

int16_t main(void) {

    init_elecanisms();
    init_ws2812b();
    
    uint16_t t=0;
    
    T1CON = 0x0030;         // set Timer1 period to 1s
    PR1 = 2*0x7A11;

    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    T1CONbits.TON = 1;      // turn on Timer1
    
    while (1){
        if (IFS0bits.T1IF == 1) {
            IFS0bits.T1IF = 0;
            t++;

            display_time(t);
        }  
    }
}
