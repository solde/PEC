#define puerto_lectura_LDR A5

  const float Vin_LDR = 5.0; //Si se alimenta con 5V, tambien puede ser 3V
  const float analog_Resolution = 1023.0; //la resolucion que tiene el ADC (en arduino de 0 a 1023)

void setup() {

 
  Serial.begin(9600);
  pinMode(puerto_lectura_LDR,INPUT);
}
void loop()
{
  long lectura=0;
  float Voltaje_R2=0;
  // Leemos el valor en el pin puerto_lectura_LDR
  lectura=analogRead(puerto_lectura_LDR);
  // Calculamos el voltaje en la resistencia desconocida
  Voltaje_R2=(Vin_LDR/analog_Resolution)*lectura;
  // Lo imprimimos en el Monitor Serie
  Serial.println(Voltaje_R2,2);
}

