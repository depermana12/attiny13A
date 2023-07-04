#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

volatile uint8_t buttonState = 0;         // current state of the button
volatile uint8_t lastButtonState = 0;     // previous state of the button
volatile uint8_t ledState = LOW;          // current state of the LED

void setup() {
   DDRB &= ~(1 << PB4);          // Set the pin PB4 as input
   PORTB |= (1 << PB4);          // Activate pull-up resistor for PB4
   DDRB |= (1 << PB0);           // Set PB0 as output (LED)

   // Enable Pin Change Interrupt on PCINT4 (PB4)
   PCICR |= (1 << PCIE0);
   PCMSK0 |= (1 << PCINT4);

   sei();  // Enable global interrupts
}

void sleep() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_cpu();
    sleep_disable();
}

ISR(PCINT0_vect) {
    buttonState = PINB & (1 << PB4);

    if (buttonState != lastButtonState) {
        if (buttonState == LOW) {
            ledState = !ledState;
            PORTB ^= (1 << PB0); // Toggle the output
        }
    }

    lastButtonState = buttonState;
}

int main() {
    setup();

    while (1) {
        sleep();
    }

    return 0;
}
