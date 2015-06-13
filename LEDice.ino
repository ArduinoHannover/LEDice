#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void showLEDs(int d) {
  digitalWrite(4,d%2);
  digitalWrite(3,d==6);
  digitalWrite(2,d>1);
  digitalWrite(1,d>3);
}

ISR(PCINT0_vect) {}

//Animation
void shake() {
  cli(); //Wecker ausschalten, wir sind ja schon wach.
  char number = (char)(millis()%6); //Wuerfelaugen berechnen
  char i;
  for (i = 0; i < 20+number; i++) {
    showLEDs(i%6+1);
    delayMicroseconds(80000); //Schnelle Animation
  }
  for (char c = 0; c<9; c++) {
    showLEDs(i%6+1);
    //delay(c*10);
    for (char w = c; w > -1; w--)
      delayMicroseconds(80000);
    i++;
  }
  for (char c = 0; c<10; c++) {
    showLEDs(i%6+1);
    delayMicroseconds(1000000L);
    showLEDs(0);
    delayMicroseconds(1000000L);
  }
  sei();          //Wecker wieder aktivieren.
  system_sleep(); //Und einschlafen.
}

void setup() {
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(0, HIGH); //Pull-Up aktivieren
  
  sbi(GIMSK,PCIE);   //Externen Interrupt (Wecker) aktivieren
  sbi(PCMSK,PCINT0); //Wecker ist an Pin 0 (Schuettelsensor)
  sei();             //Wecker aktivieren
  system_sleep();    //Gute Nacht
  /* loop() wird vorerst nicht ausgefuehrt, da der ATtiny am Schlafen ist.
   * Wird er durch das Schuetteln aufgeweckt, dann wird der loop() erst aufgerufen.
   * Im loop() legt er sich allerdings nach der Animation wieder schlafen,
   * er wird erst wiederholt, wenn der Controller wieder aufgeweckt wurde.
  */
}

void system_sleep() {
  cbi(ADCSRA,ADEN);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
}

void loop() {
  shake();
}
