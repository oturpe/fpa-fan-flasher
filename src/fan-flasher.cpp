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
        //PORTB |= BV(PORTB0);
    }
    else {
        PORTB &= ~BV(PORTB0);
    }
}

uint16_t flashCounter = 0;
uint16_t flashDelay = FLASH_DELAY;
uint16_t rotationCounter;

ISR(INT0_vect,ISR_NOBLOCK) {
    flashCounter = FLASH_LENGTH;
    flashDelay = FLASH_DELAY;
}

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK) {
    if((flashCounter == FLASH_LENGTH) && (flashDelay > 0)) {
        flashDelay--;
    }
    else if (flashCounter > 0) {
        PORTB |= BV(PORTB3);
        flashCounter--;
    } else {
        PORTB &= ~BV(PORTB3);
    }
}

int main() {

    // Set output pins
    //    B0 (indicator)
    //    B3 (power led driver)
    DDRB |= BV(DDB0) | BV(DDB3);

    // Enable external interrupt on falling edge.
    Attiny45::setExternalInterrupt(Attiny45::EIM_FALLING);
    sei();

    // Initialize timer interrupt
    TCCR0A |= BV(WGM01);
    TIMSK |= BV(OCIE0A);
    Attiny45::setTimer0Prescaler(FLASH_LENGTH_PRESCALER);
    OCR0A = FLASH_UNIT;

    bool indicatorLit = false;
    uint64_t counter = 0;
    bool flashOn = false;

    while(true) {
        counter += 1;
        _delay_ms(LOOP_DELAY);

        if(counter % INDICATOR_HALF_PERIOD == 0) {
            indicatorLit = !indicatorLit;
            setIndicator(indicatorLit);
        }
    }
}
