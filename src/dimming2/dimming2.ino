const byte interrupt_pin = 2;
const byte trigger_pin = 7;
int intensity = 0;//value (0-1024) 0->OFF 1024->Max intensity

int dimming = 0;

int state = 0;

int readDimming(){
  return 100;
}

void dimmingISR(){
  noInterrupts();
  OCR1A = 16000000/(8*dimming) -1;
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
  noInterrupts();
  digitalWrite(trigger_pin, HIGH);
  TIMSK1 &= (0 << OCIE1A);
  //delay(10);
  digitalWrite(trigger_pin, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(interrupt_pin, INPUT);
  pinMode(trigger_pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), dimmingISR, RISING);

  noInterrupts(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 100 Hz increments
  OCR1A = 19999; // = 16000000 / (8 * 100) - 1 (must be <65536)
  // OCR1A = 16000000/(8*dimming) -1;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts(); // allow interrupts
}

void loop() {
  // dimming = readDimming();

}

/* TIMER 1 for interrupt frequency 100 Hz:
cli(); // stop interrupts
TCCR1A = 0; // set entire TCCR1A register to 0
TCCR1B = 0; // same for TCCR1B
TCNT1  = 0; // initialize counter value to 0
// set compare match register for 100 Hz increments
OCR1A = 19999; // = 16000000 / (8 * 100) - 1 (must be <65536)
// turn on CTC mode
TCCR1B |= (1 << WGM12);
// Set CS12, CS11 and CS10 bits for 8 prescaler
TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
// enable timer compare interrupt
TIMSK1 |= (1 << OCIE1A);
sei(); // allow interrupts
*/

/* TIMER 1 for interrupt frequency 50 Hz:
cli(); // stop interrupts
TCCR1A = 0; // set entire TCCR1A register to 0
TCCR1B = 0; // same for TCCR1B
TCNT1  = 0; // initialize counter value to 0
// set compare match register for 50 Hz increments
OCR1A = 39999; // = 16000000 / (8 * 50) - 1 (must be <65536)
// turn on CTC mode
TCCR1B |= (1 << WGM12);
// Set CS12, CS11 and CS10 bits for 8 prescaler
TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
// enable timer compare interrupt
TIMSK1 |= (1 << OCIE1A);
sei(); // allow interrupts
 */
