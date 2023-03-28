/*
  *Programa completo del CanSat
  *LATA SATÉLITE-EMISOR DE DATOS
*/
///////////////////////////////////////////
//LIBRERÍAS PARA LOS SENSORES
//////////////////////////////////////////
#include <DHT.h> //librería para el sensor de humedad
#include <DHT_U.h>
#include <SoftwareSerial.h> //para añadir otro puerto serie por pines distintos al 0 y 1
#include <Wire.h>   // incluye libreria de bus I2C
#include <Adafruit_Sensor.h>  // incluye librerias para sensor BMP280
#include <Adafruit_BMP280.h>
#include <SPI.h>    // incluye librería interfaz SPI necesario para el lector de microSD
#include <SD.h>     // incluye librería para tarjetas SD
#include <DHT_U.h>    // incluye libreria Adafruit Unified Sensor

//////////////////////////////////////////////////////////////////////////////
//VARIABLES PARA SENSOR HUMEDAD DHT11 Y SENSOR PRESIÓN Y TEMPERATURA BMP280
/////////////////////////////////////////////////////////////////////////////
int Pin_DHT= 2;  // pin DATOS de DHT11 o DHT22 a pin digital 2
float TEMPERATURA;    // variable para almacenar valor de temperatura
float PRESION, P0;    // variables para almacenar valor de presion atmosferica
                      // y presion actual como referencia para altitud

unsigned long previousMillis=0; //variable para almacenar el tiempo del último envío
const long intervalo_envio=1100; //intervalo de tiempo entre cada envío de datos por el HC12

int Presion_min = 898;     // presión atmosférica a 1000 m de altitud
int MillisInicioBajada = 0;
boolean InicioBajada= false;
int intervalo_servo  = 10000;    // 10 segundos
#define SSpin 10    // Slave Select en pin digital 10 necesario para el lector de tarjetas microSD

////// VARIABLES PARA EL SENSOR ULTRAVIOLETA  CJMCU-GUVA-S12SD/CJMCU-S12D/////////
////// Sensor ULTRAVIOLETA:  SIGNAL->A0; Vcc=5V

long sum = 0;
int Vout = 0;
int sensorValue = 0;
int uv;

///////CREACIÓN DE OBJETOS PARA CADA SENSOR/////////
DHT dht(Pin_DHT, DHT11);   // objeto sensor de humedad. Cambiar segundo parámetro por DHT22 si se utiliza en lugar del DHT11
Adafruit_BMP280 bmp;        //objeto sensor de temperatura y presión
SoftwareSerial HC12 (10,11); //objeto para crear un puerto serie virtual para el emisor de radiofrecuencia
File archivo;     // objeto archivo del tipo File para lector de tarjetas microSD

 void setup(){
  if (!SD.begin(SSpin)) {     // inicializacion de tarjeta SD
    Serial.println("Fallo inicializando tarjeta SD!");// si falla se muestra texto correspondiente y
    return;         // se sale del setup() para finalizar el programa
  }
  Serial.println("Tarjeta SD iniciada correctamente");  // texto de inicializacion correcta
  archivo = SD.open("datos.txt", FILE_WRITE); // apertura para lectura/escritura de archivo datos.txt
  Serial.begin(9600);   // inicializacion de comunicación serie a 9600 baudios
  HC12.begin(9600); 
  dht.begin();          // inicializacion de sensor de humedad
  bmp.begin();
  Serial.println("Iniciando bmp280...");     // texto de inicio
  if ( !bmp.begin(0x76) ) {       // si falla la comunicacion con el sensor mostrar
    Serial.println("BMP280 no encontrado. Programa detenido !"); // texto y detener flujo del programa
    while (1);          // mediante bucle infinito
  }
  P0 = bmp.readPressure()/100;      // almacena en P0 el valor actual de presion
          // en hectopascales para calculo de la altura o altitud respecto de la base de lanzamiento
    }

void loop(){
  enviar_datos();
  medir_UV();
  SoltarSemillas();
  Cerrar_archivo_SD(); // Cerrará el archivo SD cuando haya pasado un tiempo determinado
}


void enviar_datos(){
    unsigned long actual=millis();  //Obtenemos el tiempo actual
  if (actual-previousMillis>=intervalo_envio){  // Comprobamos si ha pasado el intervalo de tiempo desde el último envío
  previousMillis=actual; // actualizamos el tiempo del último envío
  float temp = bmp.readTemperature();    // almacena en variable el valor de temperatura
  float presion = bmp.readPressure()/100;   // almacena en variable el valor de presion divido
  float altitud=bmp.readAltitude(P0);
            // por 100, en formato float, para covertirlo a hectopascales
  float humedad=dht.readHumidity();
  Serial.print("Temperatura: ");    // muestra texto
  Serial.print (temp);     // muestra el valor por el monitor serie
  Serial.print(" ºC Presión atmosférica:");        // muestra letra C indicando grados centigrados
  Serial.print(presion);       // muestra valor de la variable
  Serial.print("hPa ");     // muestra texto hPa indicando hectopascales
  Serial.print("Altitud");
  Serial.print(altitud);
  Serial.println("m ");
  Serial.print("Humedad:");
  Serial.print(humedad);
  Serial.println("%");
  HC12.write ((byte*)&temp, sizeof(temp));  //Enviamos el puntero de la variable temperatura y el tamaño de la variable
                                            //a través del HC-12. En este  caso el tamaño será 2 bytes ya que es de tipo float
  HC12.write ((byte*)&presion, sizeof(presion));  //Enviamos la presion a través del HC-12
  HC12.write ((byte*)&altitud, sizeof(altitud));
  HC12.write ((byte*)&humedad, sizeof(humedad)); 
}

}
void Cerrar_archivo_SD(){
    unsigned long actual=millis();  //Obtenemos el tiempo actual
    if (actual>=900000){  // Comprobamos si ha pasado 15 min para cierre del archivo SD
      archivo.close();        // cierre de archivo
      Serial.println("Archivo de la tarjeta SD completado"); // texto de escritura correcta en monitor serie
    }
}

void medir_UV() {
  sensorValue = 0;
  sum = 0;
  for (int i = 0 ; i < 1024 ; i++ ) {
    sensorValue = analogRead(A0);
    sum = sensorValue + sum;
    delay(2);
  }
  Vout = sum >> 10;
  Vout = Vout * 4980.0 / 1024;
  Serial.print("The Photocurrent value:");
  Serial.print(Vout);
  Serial.println("mV");
  if (Vout < 50) {
    uv = 0;
  }
  else if (Vout < 227) {
    uv = 1;
  }
  else if (Vout < 318) {
    uv = 2;
  }
  else if (Vout < 408) {
    uv = 3;
  }
  else if (Vout < 503) {
    uv = 4;
  }
  else if (Vout < 606) {
    uv = 5;
  }
  else if (Vout < 696) {
    uv = 6;
  }
  else if (Vout < 795) {
    uv = 7;
  }
  else if (Vout < 881) {
    uv = 8;
  }
  else if (Vout < 976) {
    uv = 9;
  }
  else if (Vout < 1079) {
    uv = 10;
  }
  else {
    uv = 11;
  }
  delay(20);
  Serial.print("Índice UV= ");
  Serial.println(uv);
}

void SoltarSemillas(){
  /*
   * SoltarSemillas()
        Medir presiÃ³nActual
        Si presiÃ³nActual < PresiÃ³nMÃ­nima    -- estamos subiendo
            PresiÃ³nMÃ­nima = presiÃ³nActual
        en caso contrario                   -- estamos bajando
            si InicioBajada == false
                MillisInicioBajada = millis()
                InicioBajada = true
            
            si millis() 
                >= 
               MillisInicioBajada + TiempoDeBajada
            
                ActivarServo
*/

}
