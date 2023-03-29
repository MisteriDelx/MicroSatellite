/*
  RECEPTOR O ESTACIÓN BASE
  Lectura de datos por el HC12
*/
#include <SoftwareSerial.h> // Librería para crear puertos virtuales
SoftwareSerial HC12 (10, 11); // pin 10 como (RX) y pin 11 como (TX)

void setup() {
  Serial.begin(9600);       // inicializa comunicacion serie a 9600 baudios
  while(!Serial){}        // mientras el Serial no tenga datos no hará ninguna acción
  HC12.begin (9600);  // inicializa la comunicación del puerto serie virtual
  pinMode (8, OUTPUT); // led de verificación de comunicaciones
}

void loop(){
    float temp, presion, altitud;  // creamos otras dos variables con otro nombre porque si no el no recibe correctamente los datos de temp y presion
    if (HC12.available()>=12) {  // Comprobamos si se ha recibido suficientes datos 12 bytes para tres números tipo float)
    HC12.readBytes((byte*)&temp, sizeof(temp));  // leemos el dato de 4 bytes y lo almacenamos en la dirección de memoria de la variable temp 
    HC12.readBytes((byte*)&presion, sizeof(presion));  // leemos los 4 bytes siguientes y los almacenamos en la dirección de memoria de la variable presion
    HC12.readBytes((byte*)&altitud, sizeof(altitud));
    Serial.print("Temperatura:");
    Serial.print(temp);  // mostramos la temperatura por el monitor serie
    Serial.print(" Presión:");
    Serial.print(presion);
    Serial.println("hPa");
    Serial.print(" Altitud:");
    Serial.print(altitud);
    Serial.println("m");
     }
}
void serialEvent(){ // esta función se ejecuta al acabar cada loop
while (HC12.available()) { // mientras haya datos en el buffer del puerto serie virtual
    // se toma un nuevo byte:
    char inChar = (char)HC12.read(); //leemos el caracter que entra por el puerto serie
    // lo añadimos al inputString:
    inputString += inChar; //inputString=inputString+inChar
    // si el siguiente carácter es un salto de línea, activa un aviso para que el programa 
    //en bucle pueda hacer algo al respecto
    if (inChar == '\n') {
      stringComplete = true;
        if (HC12.read=="led_On"){
        digitalWrite (8,HIGH); //led o zumbador de verificación de comunicaciones  
        }
      
    }
}

/*
 * el operador & se utiliza para obtener la dirección de memoria de la variable temp,
 * y se le pasa como argumento a la función readBytes. La función lee el número de bytes especificados 
 * por sizeof(temp) (que es el tamaño de la variable temp en bytes) de la comunicación serial y los almacena 
 * en la dirección de memoria que indica el puntero. La dirección de memoria se convierte a un puntero 
 * a byte utilizando un cast explícito a (byte*), para que sea compatible con el tipo de argumento que espera 
 * la función readBytes.
 * En C++, un puntero es una variable que contiene la dirección de memoria de otra variable.
 * Es decir, en lugar de almacenar el valor de una variable, un puntero almacena la dirección 
 * de memoria donde se encuentra esa variable en el espacio de memoria del programa. 
 * Un puntero se declara anteponiendo el operador * al nombre de la variable, 
 * y se inicializa asignándole la dirección de memoria de otra variable o utilizando el 
 * operador & para obtener la dirección de memoria de una variable existente.
 * 
 */
