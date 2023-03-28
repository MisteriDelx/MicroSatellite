/*
 * SENSOR ULTRAVIOLETA UV GUVA-S12SD. Este sensor se puede alimentar a 5V
 * calculamos el índice UV dividiendo la tensión de entrada del sensor por 0,1
 */
float sensorVoltage;
float sensorValue;
float intensidad_UV;

void setup() {
  Serial.begin(9600);
  pinMode(A1, OUTPUT); //leemos el valor del sensor UV por la entrada analógica 1
}

void loop() {

  
  sensorValue = analogRead(A1);
  sensorVoltage = sensorValue / 1024 * 5.0; //para convertir el valor del sensor que va de 0 a 1024 a voltios de 0 a 5V
  Serial.print("leyendo sensor UV = ");
  Serial.print(sensorValue);
  Serial.print(" voltaje del sensor UV = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");
  float intensidad_UV = mapfloat(sensorVoltage, 0, 5, 0.0, 15.0); // Conversión entre voltaje y radiación UV
  delay(1000); // deberíamos sustituir este delay por una función millis()
}
