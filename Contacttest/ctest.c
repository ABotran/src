#include "elecanisms.h"

#define D0_OUT         D0
#define D0_DIR         D0_DIR

int16_t main(void) {

    init_elecanisms();
    D0_PD = ON;

    while (1) {
        if (D0_OUT == 1){
            LED2 = ON;
        } else {
            LED2 = OFF;
        }

        LED1 = (SW2 == 0) ? ON : OFF; 
        LED3 = (SW3 == 0) ? ON : OFF;
    }
}