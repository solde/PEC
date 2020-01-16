int PWM_out_pin = 3;
int PWM_out_level = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PWM_out_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Introduce un numero entre 100 y 255 para encender el motor");
  Serial.println("Introduce 0 para apagarlo. En un inicio esta apagado")
}
/**
 * Para utilizar el PWM hay que llamar a analogWrite(pinPWM, nivelPWM);
 * Donde pinPWM es un pin del Arduino UNO (D6,D5,D3,D11,...).
 *  
 * El duty cicle es el equivalente al nivelPWM, se tabula como sigue:
 * !!!!! Se necesita una fuente de alimentación (5V) para el motor, no conectar a los 5V del arduino
 * !!!!! Los gnd de la fuente y del arduino deben ser comunes.
 * 0 --> 0% D.CYCLE
 * 127 ->50% D.CYCLE
 * 255 -> 100% D.CYCLE
 * WEB EJEMPLO: https://electronicshobbyists.com/arduino-pwm-tutorial/
 */
void loop() {
  // put your main code here, to run repeatedly
  analogWrite( PWM_out_pin, PWM_out_level);
  delay(100);
  
}


void serialEvent(){
  while(Serial.available()){
      String str = Serial.readStringUntil('\n');
      PWM_out_level = str.toInt();
      if(PWM_out_level <= 70){
         Serial.println("Apagando");
        break;
      }
      analogWrite( PWM_out_pin, 220);
      delay(251);
      analogWrite( PWM_out_pin, PWM_out_level);
      Serial.println(PWM_out_level);
  }
}

