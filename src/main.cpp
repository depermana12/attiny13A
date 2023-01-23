#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
boolean buttonState = 0;         // current state of the button
boolean lastButtonState = 0;     // previous state of the button
void setup() {
   DDRB &= ~(1 << PB4);          // Set the pin PB4 as input
   PORTB |= (1 << PB4);          //activate pull-up resistor for PB4
   DDRB |= (1<<PB0);             // set PB1 as output(LED)
}
void sleep() {
    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT4);                   // Use PB4 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT4);                  // Turn off PB4 as interrupt pin
    sleep_disable();                        // Clear SE bit
    //ADCSRA |= _BV(ADEN);                  // ADC on
    sei();                                  // Enable interrupts
    } // sleep
ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
    }
void loop() {
   static unsigned long timer = 0;
   unsigned long interval = 100;                   // check switch 20 times per second
   if (millis() - timer >= interval) {
      timer = millis();
      buttonState = PINB & (1 << PB4);
      if (buttonState != lastButtonState) {
         if (buttonState == LOW) {
            digitalWrite(PB0, !digitalRead(PB0)); // toggle the output
        }
  }
      lastButtonState = buttonState;
   }
   sleep();
}