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

ISR(INT0_vect,ISR_NOBLOCK) {
    PORTB |= BV(PORTB3);
    _delay_ms(FLASH_LENGTH);
    PORTB &= ~BV(PORTB3);
}

int main() {

    // Set output pins
    //    B0 (indicator)
    //    B3 (power led driver)
    DDRB |= BV(DDB0) | BV(DDB3);

    // Enable external interrupt on falling edge.
    GIMSK |= BV(INT0);
    MCUCR |= BV(ISC01);
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
