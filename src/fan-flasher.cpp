// Fog mover tester

#include "Attiny45Utils.h"

#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// \brief
///    Turns the indicator led on or off
///
/// \param lit
///    If led is turned on. Otherwise it is turned off.
inline void setIndicator(bool lit) {
    if (lit) {
        PORTB |= BV(PORTB0);
    }
    else {
        PORTB &= ~BV(PORTB0);
    }
}


bool magnetFound = false;

ISR(INT0_vect,ISR_NOBLOCK) {
    magnetFound = !magnetFound;

    if (magnetFound) {
        // External interrupt on rising edge.
        MCUCR |= BV(ISC01) | BV(ISC00);
        //DmxSimple.write(1,0x00);
        setIndicator(true);
    }
    else {
        // External interrupt on falling edge.
        MCUCR |= BV(ISC01);
        MCUCR &= ~BV(ISC00);
        //DmxSimple.write(1,0xff);
        setIndicator(false);
    }
}

int main() {

    // Set output pins
    //    B0 (power led driver)
    //    B1 (indicator)
    DDRB |= BV(DDB0);

    // Enable external interrupt
    GIMSK |= BV(INT0);
    sei();

    bool indicatorLit = false;
    uint64_t counter = 0;

    while(true) {
        counter += 1;
        _delay_ms(LOOP_DELAY);

        if(counter % INDICATOR_HALF_PERIOD == 0) {
            indicatorLit = !indicatorLit;
            setIndicator(indicatorLit);
        }
    }
}
