String command;
//---------VARIABLES----------
//---Dimming---
const byte interrupt_pin = 2;
const byte trigger_pin = 7;
int intensity = 0;//value (0-1024) 0->OFF 1024->Max intensity
int dimming = 0;
int state = 0;

//---Air Q---
#define puerto_lectura_air_quality_analog A2
#define puerto_lectura_air_quality_digital 12

float sensorValue;
float RS_air;
float R0;

//---Temp/Humidity---

  // REQUIRES the following Arduino libraries:
  // - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
  // - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 11
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

//---LDR---
#define puerto_lectura_LDR A5

const float Vin_LDR = 5.0; //Si se alimenta con 5V, tambien puede ser 3V
const float analog_Resolution = 1023.0; //la resolucion que tiene el ADC (en arduino de 0 a 1023)


//---MOTOR---
int PWM_out_pin = 3;
int PWM_out_level = 0;

//---PIR---
const int pir_pin = 5;


//--------SETUP---------

//---Dimming---
void dimmingISR(){
  noInterrupts();
  OCR1A = 16000000/(8*dimming) -1;
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void DimmingSetup(){
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

//---Air Q---

void AirQSetup(){
  pinMode(puerto_lectura_air_quality_analog,INPUT);
  pinMode(puerto_lectura_air_quality_digital,INPUT);
  }

//---Temp/Humidity---

void THSetup(){
   // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

//---LDR---

void LDRSetup() {
  pinMode(puerto_lectura_LDR,INPUT);
}

//---MOTOR---

void MotorSetup(){
  pinMode(PWM_out_pin, OUTPUT);
  Serial.println("Introduce un numero entre 100 y 255 para encender el motor");
  Serial.println("Introduce 0 para apagarlo. En un inicio esta apagado");
}

//---Pir---

/*void pirISR(){
  Serial.println("movement detected!!!! \n AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH run");
}*/

void PirSetup(){
  pinMode(pir_pin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(pir_pin), pirISR, RISING);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DimmingSetup();
  AirQSetup();
  THSetup();
  LDRSetup();
  MotorSetup();
  PirSetup();
  Serial.println("SETUP DONE");
  
}

//------FUNCTIONS--------

//---Dimming---

void readDimming(){
  dimming = command.substring(8).toInt();
  }
  
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
  noInterrupts();
  digitalWrite(trigger_pin, HIGH);
  TIMSK1 &= (0 << OCIE1A);
  delayMicroseconds(100);
  digitalWrite(trigger_pin, LOW);
}

//---Air Q---

void readAirQ(){
 /* there is no analog read in here :)
  *  for(int x=0;x<100;x++)
    sensorValue=sensorValue+analogRead(puerto_lectura_air_quality_analog);
  sensorValue=sensorValue/100.0;
  float sensor_volt = (sensorValue/1024)*5.0; 
  RS_air = (5.0-sensor_volt)/sensor_volt; // Depend on RL on yor module 
  R0 = RS_air/9.9; // According to MQ9 datasheet table 

  Serial.print("analog: ");
  Serial.println(R0);
  */ 
  Serial.print("digital: ");
  Serial.println(digitalRead(puerto_lectura_air_quality_digital));
  //delay(1000);
}

//---Temp/Humidity---

void readTH(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}

//---LDR---

void readLDR(){
  long lectura=0;
  float Voltaje_R2=0;
  // Leemos el valor en el pin puerto_lectura_LDR
  lectura=analogRead(puerto_lectura_LDR);
  // Calculamos el voltaje en la resistencia desconocida
  Voltaje_R2=(Vin_LDR/analog_Resolution)*lectura;
  // Lo imprimimos en el Monitor Serie
  Serial.println(Voltaje_R2);
}

//---MOTOR---

void MotorLoop(){
  analogWrite( PWM_out_pin, PWM_out_level);
  delay(100);
}

void readMotor(){
  PWM_out_level = command.substring(6).toInt();
      if(PWM_out_level <= 70){
         Serial.println("Apagando");
        return;
      }
      analogWrite( PWM_out_pin, 220);
      delay(251);
      analogWrite( PWM_out_pin, PWM_out_level);
      Serial.println(PWM_out_level);
}

//---PIR---

void readPir(){
  Serial.println("istheresomeone?");
  Serial.println(digitalRead(pir_pin));
}

//---Control---
void displayHelp(){
  Serial.println("Feature not implemented yet :)");
  }

void readCommand(){
  if(Serial.available()){
    command = Serial.readString();
    if(command.startsWith("dimming"))readDimming();
    else if (command.startsWith("AirQ"))readAirQ();
    else  if (command.startsWith("TH"))readTH();
    else  if (command.startsWith("LDR"))readLDR();
    else  if (command.startsWith("Motor"))readMotor();
    else  if (command.startsWith("Pir"))readPir();
    else  if (command.startsWith("help"))displayHelp();
    else Serial.println("Unkown command, type help for information about available commands");
    }
  }

//-----------------------------  
void loop() {
  // put your main code here, to run repeatedly:
  readCommand();
  MotorLoop();
  //Serial.println("istheresomeone?");
  //Serial.println(digitalRead(pir_pin));
}
