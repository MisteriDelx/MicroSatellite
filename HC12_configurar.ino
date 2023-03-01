/*  
 *  Abajo esta el autor original del sketch; yo le cambie algunos parámetros nomas 
 *  
 *  HC12 Send/Receive Example Program 2
    By Mark J. Hughes
    for AllAboutCircuits.com

    Conectar el pin RXD del HC12 al pin digital de Arduino 11
    Conectar el pin TXD del HC12 al pin digital de Arduino 10
    Conectar el pin SET del HC12 al pin digital de Arduino 9

    IMPORTANTE: El sketch original recomienda no alimentar al Arduino
    solo por el cable USB sino que conectarlo por USB pero ademas
    proveerle otra alimentación (como ser una pila de 9V).
    Respecto al capacitor de desacople recomendado en la imagen
    del conexionado, yo he trabajado sin el y no he tenido problemas.*/

#include <SoftwareSerial.h>

const byte HC12RxdPin = 11;                      // Pin de Arduino conectado al RXD del HC12
const byte HC12TxdPin = 10;                      // Pin de Arduino conectado al TXD del HC12
const byte HC12SetPin = 9;                      // Pin de Arduino conectado al SET del HC12

unsigned long timer = millis();                 // timmer para los delay
char SerialByteIn;                              // Variable temporal
char HC12ByteIn;                                // Variable temporal
String HC12ReadBuffer = "";                     // Escribir/leer el buffer 1 del HC12
String SerialReadBuffer = "";                   // Escribir/leer el buffer 2 del puerto serie
boolean SerialEnd = false;                      // Indicador de fin del string del puerto serie
boolean HC12End = false;                        // Indicador de fin del string del HC12
boolean commandMode = false;                    // Envío de comandos AT

// Creamos los pines de transmisión y recepción del "nuevo" puerto serie
SoftwareSerial HC12(HC12TxdPin, HC12RxdPin);

void setup() {

  HC12ReadBuffer.reserve(64);                   // Se reservan 64 bytes para el mensaje a la entrada del puerto serie
  SerialReadBuffer.reserve(64);                 // Se reservan 64 bytes para el mensaje a la entrada del HC12
  pinMode(HC12SetPin, OUTPUT);                  // Se configura el pin como salida (High for Transparent / Low for Command)
  digitalWrite(HC12SetPin, HIGH);               // Al ponerlo en alto, se activa el modo "Transparente" para setear parametros
  delay(80);                                    // 80 ms de delay recomendados en la hoja de datos
  Serial.begin(9600);                           // Abro el puerto serie de la computadora
  HC12.begin(9600);                             // Abro el puerto serie de Arduino que va al HC12
}

void loop() {

  while (HC12.available()) {                    // Mientras el puerto del HC12-Arduino tenga datos
    HC12ByteIn = HC12.read();                   // Se guarda cada caracter del buffer en byteIn
    HC12ReadBuffer += char(HC12ByteIn);         // Escribimos cada caracter del byteIn al buffer HC12ReadBuffer
    if (HC12ByteIn == '\n') {                   // Chequeamos si esta el caracter que indica el fin del string
      HC12End = true;                           // La variable HC12End (el indicador de fin del string del HC12) se pone en TRUE
    }
  }

  while (Serial.available()) {                  // Mientras en el puerto Arduino-computadora hay datos
    SerialByteIn = Serial.read();               // Se guarda cada caracter en byteIn
    SerialReadBuffer += char(SerialByteIn);     // Escribimos cada caracter del byteIn en el buffer SerialReadBuffer
    if (SerialByteIn == '\n') {                 // Chequeamos si esta el caracter que indica el fin del string
      SerialEnd = true;                         // Ponemos la variable SerialEND en TRUE
    }
  }

  if (SerialEnd) {                              // Lo siguiente ocurre cuando sea TRUE la variable SerialEnd

    if (SerialReadBuffer.startsWith("AT")) {    // Si el comando empieza con AT
      HC12.print(SerialReadBuffer);             // Se envía el comando
      delay(100);                               //
      digitalWrite(HC12SetPin, LOW);            // Sepone el pin de SET en estado bajo para entrar al modo de comandos
      delay(100);                               // Un delay recomendado antes de empezar a enviar comandos
      Serial.print(SerialReadBuffer);           // Vemos el "eco" del comando que enviamos en el monitor serie
      HC12.print(SerialReadBuffer);             // Enviamos el comando al HC12
      delay(500);                               // Esperamos 500 ms para la respuesta
      digitalWrite(HC12SetPin, HIGH);           // Salimos del modo de comandos y entramos al modo transparente
      delay(100);                               // Un delay antes de proceder
    } else {
      HC12.print(SerialReadBuffer);             // Transmit non-command messageAT
    }
    SerialReadBuffer = "";                      // Vaciamos el buffer SerialReadBuffer
    SerialEnd = false;                          // Reseteamos el indicador de fin del string
  }

  if (HC12End) {                                // Si el indicador HC12End esta en TRUE
    if (HC12ReadBuffer.startsWith("AT")) {      // Chequeamos si el comando AT fue recibido
      digitalWrite(HC12SetPin, LOW);            // Ponemos en bajo el pin de SET y entramos en el modo de comandos
      delay(100);                               // Un delay antes de enviar el comando
      Serial.print(SerialReadBuffer);           // Eco del comando para verlo en el monitor serie
      HC12.print(HC12ReadBuffer);               // Escribe el comando en el HC12
      delay(500);                               // Esperamos 500 ms para ver su respuesta
      digitalWrite(HC12SetPin, HIGH);           // Salimos del modo de comandos y volvemos al modo transparente
      delay(100);                               // Un delay antes de proceder
      HC12.println("Remote Command Executed");  // Una linea indicando si se ejecuto el comando
    } else {
      Serial.print(HC12ReadBuffer);             // Envio del mensaje a la pantalla
    }
    HC12ReadBuffer = "";                        // Vaciamos el buffer
    HC12End = false;                            // Reseteamos el indicador de fin del string
  }
}

