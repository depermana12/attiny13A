#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

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

void toggleLedOn() {
    ledState = HIGH;
    PORTB |= (1 << PB0);
}

void toggleLedBlink() {
    static uint32_t timer = 0;
    uint32_t interval = 5000;  // Blink interval of 5 seconds

    if ((millis() - timer) >= interval) {
        timer = millis();
        ledState = !ledState;

        if (ledState == HIGH) {
            PORTB |= (1 << PB0);
        } else {
            PORTB &= ~(1 << PB0);
        }
    }
}

void toggleLedOff() {
    ledState = LOW;
    PORTB &= ~(1 << PB0);
}

ISR(PCINT0_vect) {
    buttonState = PINB & (1 << PB4);

    if (buttonState != lastButtonState) {
        if (buttonState == LOW) {
            toggleLedOn();
        }
    }
   
    lastButtonState = buttonState;
}

int main() {
    setup();

    while (1) {
        if (ledState == HIGH) {
            toggleLedBlink();
        }

        sleep();
    }

    return 0;
}
