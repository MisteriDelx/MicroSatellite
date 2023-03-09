/*
  RECEPTOR O ESTACIÓN BASE
  Lectura de datos por el HC12
*/

#include <Wire.h>   // incluye libreria de bus I2C
#include <Adafruit_Sensor.h>  // incluye librerias para sensor BMP280
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;    // crea objeto con nombre bmp

String mensaje= "La temperatura es...";
//float Temp;    // variable para almacenar valor de temperatura

void setup() {
  Serial.begin(9600);       // inicializa comunicacion serie a 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);
}
  
void loop() {
  //Serial.print(mensaje);
  if (Serial.available() > 0) {
    float Temp = Serial.parseFloat();
    Serial.println(Temp);
   }
}   
