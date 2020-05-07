#include "elecanisms.h"
#include "ws2812b.h"

void write_leds(uint8_t red,uint8_t green,uint8_t blue, uint16_t num){
    uint16_t i; //could it be the 8/16 bit num?

    disable_interrupts();

    for(i=0;i<num;i++)
        ws2812b_write(red,green,blue);

    enable_interrupts();
}

int16_t main(void) {

	uint16_t state = 0;

    init_elecanisms();
    init_ws2812b();

    T1CON = 0x0030;         // set Timer1 period to 0.5s
    PR1 = 0x7A11;

    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    T1CONbits.TON = 1;      // turn on Timer1

	write_leds(0,0,0,5);
    while (1) {
        if (IFS0bits.T1IF == 1) {
            IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
            if (state==1){
            	state = 0;
            	write_leds(0,0,100,5);
                LED1 = OFF;
                LED3 = ON;
            } else {
            	state = 1;
            	write_leds(100,0,0,5);
            	LED1 = ON;
            	LED3 = OFF;
            }
        }    
            
    }
}
