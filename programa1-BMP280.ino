/*
  Capitulo 36 de Arduino desde cero en Español.
  Programa que muestra en monitor serie los valores de temperatura y presion
  atmosferica del sensor BMP280 conectado mediante bus I2C.

  Autor: bitwiseAr  

*/

#include <Wire.h>   // incluye libreria de bus I2C
#include <Adafruit_Sensor.h>  // incluye librerias para sensor BMP280
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;    // crea objeto con nombre bmp

float TEMPERATURA;    // variable para almacenar valor de temperatura
float PRESION;      // variable para almacenar valor de presion atmosferica
float ALTITUD;  //variable altitud

void setup() {
  Serial.begin(9600);       // inicializa comunicacion serie a 9600 bps
  Serial.println("Iniciando:");     // texto de inicio
  if ( !bmp.begin() ) {       // si falla la comunicacion con el sensor mostrar
    Serial.println("BMP280 no encontrado !"); // texto y detener flujo del programa
    while (1);          // mediante bucle infinito
  }
}

void loop() {
  TEMPERATURA = bmp.readTemperature();    // almacena en variable el valor de temperatura
  PRESION = bmp.readPressure()/100;   // almacena en variable el valor de presion divido
                                      // por 100 para covertirlo a hectopascales
  ALTITUD = bmp.readAltitude(1006);    //ajustamos la altitud indicando en el parámetro la presión a nivel del mar
  Serial.print("Temperatura: ");    // muestra texto
  Serial.print(TEMPERATURA);      // muestra valor de la variable
  Serial.print(" C ");        // muestra letra C indicando grados centigrados
    
  Serial.print("Presion: ");      // muestra texto
  Serial.print(PRESION);      // muestra valor de la variable
  Serial.print(" hPa   ");     // muestra texto hPa indicando hectopascales
  
  Serial.print("Altitud: ");      // muestra texto
  Serial.print(ALTITUD);      // muestra valor de la variable
  Serial.println(" m.s.n.m");     // metros sobre el nivel del mar

  delay(500);          // retardo de 1 segundo entre lecturas
}
