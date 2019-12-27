const byte interrupt_pin = 2;
const byte trigger_pin = 7;
int intensity = 0;//value (0-1024) 0->OFF 1024->Max intensity

const byte pot = A4;

int dimming = 0;

int state = 0;

void dimmingISR() {
  //detachInterrupt(digitalPinToInterrupt(interrupt_pin));
  delayMicroseconds(9*1000);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigger_pin, LOW);
  //attachInterrupt(digitalPinToInterrupt(interrupt_pin), dimmingISR, FALLING);
}

void setup() {
  Serial.begin(9600);
  pinMode(interrupt_pin, INPUT);
  pinMode(trigger_pin, OUTPUT);
  pinMode(pot, INPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), dimmingISR, RISING);
}



int getDimmingValue() {
  //return (analogRead(pot)/1023)*10;
  return 1;
}

void loop() {
  //detachInterrupt(digitalPinToInterrupt(interrupt_pin));
  //dimming = getDimmingValue();
  //if(dimming != 0)attachInterrupt(digitalPinToInterrupt(interrupt_pin), dimmingISR, FALLING);
  /*Serial.println("H");
  digitalWrite(trigger_pin, HIGH);
  //Serial.println("H");
  delay(5);
  //delay(9);
  Serial.println("L");
  digitalWrite(trigger_pin, LOW);
  delay(5);*/
}
