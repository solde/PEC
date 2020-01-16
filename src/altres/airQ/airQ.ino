#define puerto_lectura_air_quality_analog A2
#define puerto_lectura_air_quality_digital 12

float sensorValue;
float RS_air;
float R0;

void setup() {
  Serial.begin(9600);
  pinMode(puerto_lectura_air_quality_analog,INPUT);
  pinMode(puerto_lectura_air_quality_digital,INPUT);
}


void loop() {
  for(int x=0;x<100;x++){
    sensorValue=sensorValue+analogRead(puerto_lectura_air_quality_analog);
      
  }
  sensorValue=sensorValue/100.0;
  float sensor_volt = (sensorValue/1024)*5.0; 
  RS_air = (5.0-sensor_volt)/sensor_volt; // Depend on RL on yor module 
  R0 = RS_air/9.9; // According to MQ9 datasheet table 


  Serial.print("analog");
  Serial.println(R0);
  Serial.print("digital");
  Serial.println(digitalRead(puerto_lectura_air_quality_digital));
  delay(1000);
}
