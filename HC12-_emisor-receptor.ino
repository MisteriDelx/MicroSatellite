 //EMISOR-RECEPTOR
#include <SoftwareSerial.h>

SoftwareSerial serial2(10, 11);  //configuramos el pin10 como RXD y el pin 11 como TXD, que irán conectados al TXD y RXD del modulo radiofrecuencia

void setup()
{
// BLUETOOTH
    Serial.begin(9600);
// HC-12   
    serial2.begin(9600);
 }

void loop() {
  if (Serial.available()) {  // aca envias por BT y lees tu teclado del monitor Serie.
      char val = Serial.read();
      serial2.print(val);
  }
  else{
    Serial.println("Monitor Serie no disponible");
  }
  if (serial2.available()) {  // aca recibes y muestras en pantalla del IDE
      char val1 = serial2.read();
      Serial.print(val1);            // lo recibido por el BT se verá en el Monitor Serie.
  }
  else {
    Serial.println(" Módulo de RF no disponible");
  }
  delay (20);
}
