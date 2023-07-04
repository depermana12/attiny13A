#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

volatile uint8_t buttonState = 0;         // current state of the button
volatile uint8_t lastButtonState = 0;     // previous state of the button
volatile uint8_t ledState = 0;            // current state of the LEDs

void setup() {
   DDRB &= ~(1 << PB4);          // Set the pin PB4 as input
   PORTB |= (1 << PB4);          // Activate pull-up resistor for PB4
   DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);  // Set PB0, PB1, PB2 as output (LEDs)

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

void toggleLedChaser500ms() {
    static uint32_t timer = 0;
    uint32_t interval = 500;  // Blink interval of 500ms

    if ((millis() - timer) >= interval) {
        timer = millis();
        ledState = (ledState + 1) % 8;

        PORTB = (PORTB & 0xF8) | ledState;
    }
}

void toggleLedChaser1s() {
    static uint32_t timer = 0;
    uint32_t interval = 1000;  // Blink interval of 1s

    if ((millis() - timer) >= interval) {
        timer = millis();
        ledState = (ledState + 1) % 8;

        PORTB = (PORTB & 0xF8) | ledState;
    }
}

ISR(PCINT0_vect) {
    buttonState = PINB & (1 << PB4);

    if (buttonState != lastButtonState) {
        if (buttonState == LOW) {
            ledState = 0;
        }
    }
   
    lastButtonState = buttonState;
}

int main() {
    setup();

    while (1) {
        if (ledState != 0) {
            toggleLedChaser500ms();
        } else {
            toggleLedChaser1s();
        }

        sleep();
    }

    return 0;
}
