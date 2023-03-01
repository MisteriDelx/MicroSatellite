#include <Servo.h>    //Biblioteca con instrucciones de control de servos
Servo servo1;

int PINSERVO = 2;
int PULSOMIN = 550;
int PULSOMAX = 2550;

void setup(){
  servo1.attach (PINSERVO,PULSOMIN,PULSOMAX);
}

void loop(){
  servo1.write(0);   //lo llevamos a ángulo 0
  delay(2000);
  servo1.write(180);  //lo llevamos a ángulo 180
  delay(2000);
}
