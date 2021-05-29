/*

  Adaptación de control de silla de ruedas.

  Movimientos de motores a través de joystick y a través del móvil usando bluetooh.



  Creado en 2020.
  Por David Flores Román.
  Modificado el 29 de Septiembre de 2020.

*/


//librerias para bluetooth
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

//libreria para control de motores
#include "BTS79602M.h"

#include <analogWrite.h>

 //unsigned long time1=0;
  //unsigned long time2=0;
//#define BUZZER_PIN 27
//#define BUZZER_CHANNEL 1

int control = 12;

//Definicion de variables para los motores
/////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t EN1 = 19;
const uint8_t L_PWM1 = 18;
const uint8_t R_PWM1 = 5;

const uint8_t EN2 = 4;
const uint8_t L_PWM2 = 16;
const uint8_t R_PWM2 = 17;

BTS79602M motorController(EN1, L_PWM1, R_PWM1, EN2, L_PWM2, R_PWM2);


//variables para mostrar indicador de batería a través de LEDs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int r = 0; //variable que almacena el valor leído por el pin 34
float R = 0; //variable que almacena el valor de voltaje de la batería.
int au = 0; //variable auxiliar para almacenar el valor de r


//defición de pines para LEDs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int ledPIN1 = 23;//led verde
const int ledPIN2 = 22;//led amarillo
const int ledPIN3 = 21; //led rojo
const int ledPIN4 = 0; //led verde
const int ledPIN5 = 2; //led verde

//boton para encender los LEDs

//const int F_boton=13; //pin que suministra una corriente necesaria para ser leida por otros pines para saber si se ha pulsado un botón u otro
const int boton = 15; // pin de lectura para saber si se ha pulsado el boton para encender los leds.

bool aux = 0; //variable auxiliar para el botón.


//Variables para joystick
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int valorX = 0; // variable de lectura de eje X
int valorY = 0; // variable de lectura de eje Y
int pinJX = 35; // eje X conectado al pin analogico 35
int pinJY = 32; // eje Y conectado al pin analogico 32


int const inf = 2000; //limite inferior del joystick
int const sup = 2130; //limite superior del joystick


int X = 0; //se almacena el cambio de valores del joystick de 0 a 4095 en -2000 hasta 2100
int Y = 0; //se almacena el cambio de valores del joystick de 0 a 4095 en -2000 hasta 2100
float auxi = 0; //variable auxiliar

int angulo; //variable utilizada en la función angulos
int distan; //variable utilizada en la función distancia
int vel; //variable utilizada en la función velocidad
int v;  //variable que almacena el valor devuelto por la función velocidad

int ac; //variable para cambiar la potencia de los motores

//variables para botones de velocidad
//////////////////////////////////////////////////////////////////////////////////

const int PINjoy1 = 33; //almacena boton para velocidad 1 y 2 en el pin 33
const int PINjoy2 = 25; //boton para los frenos
//bool b=0; //variable que cambia de un boton a otro

int joy1 = 0;//boton velocidad 1 y 2
int joy2 = 0; //boton frenos
int Estjoy1 = 0; //variable estado boton velocidad
int Estjoy2 = 0; //variable estado boton frenos

//variables boton y zumbador
//////////////////////////////////////////////////////////////////////////////////
const int PINbotZ = 26;
const int PINz = 27;
int lecb;
unsigned int beeps = 10;


//int periodo = 1000;
//unsigned long TiempoAhora = 0;
////////////////////////////////////////////////////////////////////////////////


void setup() {

  Serial.begin(115200);

  Dabble.begin("MyEsp32");       //nombre de dispositivo bluetooth
  //robot.begin();               //inicia los motores



  /////////////////////////////////////////////////////////////////////////////
  pinMode(ledPIN1 , OUTPUT);  //definir pin ledPIN1 como salida
  pinMode(ledPIN2 , OUTPUT);//definir pin ledPIN2 como salida
  pinMode(ledPIN3 , OUTPUT);//definir pin ledPIN3 como salida
  pinMode(ledPIN4 , OUTPUT);//definir pin ledPIN4 como salida
  pinMode(ledPIN5 , OUTPUT);//definir pin ledPIN5 como salida
  // pinMode(F_boton , OUTPUT);
  pinMode(PINz, OUTPUT);
  pinMode(control, OUTPUT);

  pinMode(boton, INPUT_PULLDOWN); //definir pin boton como entrada utilizando resistencia interna de pulldown
  pinMode(PINjoy1, INPUT_PULLDOWN);//definir pin PINjoy1 como entrada utilizando resistencia interna de pulldown
  pinMode(PINjoy2, INPUT_PULLDOWN);//definir pin PINjoy2 como entrada utilizando resistencia interna de pulldown
  pinMode(PINbotZ, INPUT_PULLDOWN);//definir pin PINbotz como entrada utilizando resistencia interna de pulldown



}


void loop() {

  Dabble.processInput();
  motorController.Enable();

  //EasyBuzzer.update();
//time1 = millis();
  //digitalWrite(F_boton , HIGH); //pone e HIGH el valor del pin 13 para saber si se ha pulsado algún botón


  //llamadas joystick
  /////////////////////////////////////////////////////////////////////////////////////////////////
  valorX = analogRead ( pinJX); // lectura eje X
  valorY = analogRead ( pinJY); // lectura eje Y


  //codigo para cambiar los valores de X e Y de valores entre numeros negativos y positivos.
  if(valorX >= sup){
    X = valorX - sup;
  }

  else if (valorX <= inf) {
    X = valorX - inf;
  }
  else
    X = 0;

  if (valorY >= sup) {
    Y = valorY - sup;
  }

  else if (valorY <= inf) {
    Y = valorY - inf;
  }
  else Y = 0;

  ///////////////////////////////////////////////////////////////////////////////////////////////////

 // Serial.print("ejeX;  "); //Prueba de testeo por pantalla del ordenador
 // Serial.print(X);
  //Serial.print("  ejeY;  ");
  //Serial.print(Y);
  //Serial.print("   valor de v: ");
  //Serial.print(v);
  //
  //
  //Serial.print ("angulo: ");
  //Serial.print(angulos (X,Y));

  v = velocidad(distancia(X, Y)); //llamada a la función velocidad que a su vez llama a la función distancia para saber los distintos puntos donde va cambiando la velocidad

  //Serial.print ("   velocidad: ");
  //Serial.println(v);

  //lectura de botones de velocidad
  joy1 = digitalRead(PINjoy1);
  joy2 = digitalRead(PINjoy2);



  if (joy2 == HIGH) {
    delay(100);
    Estjoy2 = 1 - Estjoy2;
  }

  if (Estjoy2 == 1) {
    digitalWrite(control, HIGH);
  // Serial.println("voltaje alto");
    
  }

  else {
    digitalWrite(control, LOW);
      //Serial.println("voltaje bajo");
  }

  //codigo para cambiar de un boton a otro almacenando el valor en b
  if (joy1 == HIGH) {
    delay(100);
    Estjoy1 = 1 - Estjoy1;
  }


  //codigo para poner distintas velocidades según el boton pulsado
  if (Estjoy1 == 0) {
    if (v == 1) {
      ac = 135;
    }
    if (v == 2) {
      ac = 140;
    }
    if (v == 3) {
      ac = 145;
    }
    if (v == 4) {
      ac = 150;
    }
    if (v == 5) {
      ac = 155;
    }
    if (v == 6) {
      ac = 160;
    }
    if (v == 7) {
      ac = 165;
    }
    if (v == 8) {
      ac = 170;
    }
    if (v == 9) {
      ac = 175;
    }
    if (v == 10) {
      ac = 180;
    }
    if (v == 11) {
      ac = 185;
    }
    if (v == 12) {
      ac = 190;
    }

  }

  else {
    if (v == 1) {
      ac = 195;
    }
    if (v == 2) {
      ac = 200;
    }
    if (v == 3) {
      ac = 205;
    }
    if (v == 4) {
      ac = 210;
    }
    if (v == 5) {
      ac = 215;
    }
    if (v == 6) {
      ac = 220;
    }
    if (v == 7) {
      ac = 225;
    }
    if (v == 8) {
      ac = 230;
    }
    if (v == 9) {
      ac = 235;
    }
    if (v == 10) {
      ac = 240;
    }
    if (v == 11) {
      ac = 245;
    }
    if (v == 12) {
      ac = 255;
    }
  }



  //codigo para los movimientos de los motores con el joystick




  //up
  if (angulos (X, Y) >= 60 && angulos (X, Y) <= 120 && v > 0 ) {
    motorController.TurnLeftM1(ac);
    motorController.TurnLeftM2(ac);
    //delay(100);
  }

  //back
  if (angulos (X, Y) >= 240 && angulos (X, Y) <= 300 && v > 0) {

    motorController.TurnRightM1(ac);
    motorController.TurnRightM2(ac);
    //delay(100);
  }

  //left
  if (angulos (X, Y) >= 145 && angulos (X, Y) <= 215 && v > 0 ) {
    motorController.TurnRightM2(ac);
    motorController.TurnLeftM1(ac);
    //delay(100);

  }

  //right
  if (((angulos (X, Y) > 0 && angulos (X, Y) <= 35) || (angulos (X, Y) >= 325 && angulos (X, Y) < 360)) && v > 0) {

    motorController.TurnLeftM2(ac);
    motorController.TurnRightM1(ac);
    //delay(100);
  }


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7


  aux = digitalRead(boton); //lectura del pin 15

  //divisor de tensión para saber voltaje de la batería

  r = analogRead(34);
  au = r;
R = ((au * 25)/2300); // operación de conversión del rango que usa ESP32 0-4095 a voltios.
//R=25;

Serial.print("  r = ");
Serial.println(r);


  lecb = digitalRead(PINbotZ);


  //Movimientos motores bluetooth
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  //Motor A hacia adelante y Motor B hacia adelante
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  if (GamePad.isUpPressed() == 1 && GamePad.isLeftPressed() == 0 && GamePad.isRightPressed() == 0 && GamePad.isDownPressed() == 0 && v == 0) {

    motorController.TurnLeftM1(140);
    motorController.TurnLeftM2(140);
    //delay(100);
  }






  //Motor A hacia atras y Motor B hacia atras
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  if (GamePad.isDownPressed() == 1  && GamePad.isUpPressed() == 0 && GamePad.isLeftPressed() == 0 && GamePad.isRightPressed() == 0 && v == 0) {

    motorController.TurnRightM1(140);
    motorController.TurnRightM2(140);
    //delay(100);
  }






  //Los dos motores apagados
  /////////////////////////////////////////////////////////////////////////////////////////////////
  if (GamePad.isDownPressed() == 0 && GamePad.isUpPressed() == 0 && GamePad.isLeftPressed() == 0 && GamePad.isRightPressed() == 0 && v == 0 ) {

    motorController.Stop();
    motorController.Disable();
    //delay(100);
  }

  //Girar a la izquierda
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  if (GamePad.isLeftPressed () == 1 && GamePad.isDownPressed() == 0 && GamePad.isUpPressed() == 0  && GamePad.isRightPressed() == 0  && v == 0) {
    motorController.TurnRightM2(140);
    motorController.TurnLeftM1(140);
    //delay(100);
  }




  //Girar a la derecha
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  if (GamePad.isLeftPressed () == 0 && GamePad.isDownPressed() == 0 && GamePad.isUpPressed() == 0  && GamePad.isRightPressed() == 1 && v == 0) {

    motorController.TurnLeftM2(140);
    motorController.TurnRightM1(140);
    //delay(100);

  }


  //LEDs
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  leds(aux, R); //llamada a la función leds



  //zumbador
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////


if (lecb == HIGH) {
  

 digitalWrite(PINz, HIGH);

 
  
}
else {

   digitalWrite(PINz, LOW);


}


}
//Funciones
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Funcion leds
///////////////////////////////////////////////////////////////////
void leds(int aux, int r) { //utiliza un auxiliar para el boton y la variable R para apagar los leds en función del voltaje de la bateria

  if (aux == 1) {



    if (R >= 24 ) {

      digitalWrite(ledPIN1 , HIGH);
      digitalWrite(ledPIN2 , HIGH);
      digitalWrite(ledPIN3 , HIGH);
      digitalWrite(ledPIN4 , HIGH);
      digitalWrite(ledPIN5 , HIGH);

      

    }

    if (R >= 20 && R < 24 ) {
      digitalWrite(ledPIN1 , HIGH);
      digitalWrite(ledPIN2 , HIGH);
      digitalWrite(ledPIN3 , HIGH);
      digitalWrite(ledPIN4 , HIGH);
      digitalWrite(ledPIN5 , LOW);

      

    }

    if (R <= 18 && R < 20 ) {

      digitalWrite(ledPIN1 , HIGH);
      digitalWrite(ledPIN2 , HIGH);
      digitalWrite(ledPIN3 , HIGH);
      digitalWrite(ledPIN4 , LOW);
      digitalWrite(ledPIN5 , LOW);
      
    }

    if (R <= 15 && R < 18 ) {

      digitalWrite(ledPIN1 , LOW);
      digitalWrite(ledPIN2 , HIGH);
      digitalWrite(ledPIN3 , HIGH);
      digitalWrite(ledPIN4 , LOW);
      digitalWrite(ledPIN5 , LOW);
      
    }

    if (R < 15) {

      digitalWrite(ledPIN1 , LOW);
      digitalWrite(ledPIN2 , LOW);
      digitalWrite(ledPIN3 , HIGH);
      digitalWrite(ledPIN4 , LOW);
      digitalWrite(ledPIN5 , LOW);
      
    }


  }
  else if (aux == 0) {
    digitalWrite(ledPIN1 , LOW);
    digitalWrite(ledPIN2 , LOW);
    digitalWrite(ledPIN3 , LOW);
    digitalWrite(ledPIN4 , LOW);
    digitalWrite(ledPIN5 , LOW);
  }


}

//Funcion angulos
int angulos (int a, int b) { //devuelve el angulo de 0 a 359 en el que se encuentra el joystick calculando su tangente.

  auxi = atan2(a, b);
  angulo = (auxi * 180) / PI;

  if (angulo < 0) {
    angulo = angulo + 360;
  }
  return angulo;
}
//Funcion distancia
int distancia(int c, int d) { // devuelve la distancia desde el centro del joystick usando pitagoras.

  distan = sqrt(pow(c, 2) + pow(d, 2));

  return distan;
}

//Funcion velocidad
int velocidad(int ve) { // devuelve un valor para asignar la velocidad en base a la función distancia.

  if (ve > 300 && ve < 466) {
    vel = 1;
  }

  else if (ve >= 466 && ve < 632) {
    vel = 2;
  }

  else if (ve >= 632 && ve < 798) {
    vel = 4;
  }

  else if (ve >= 798 && ve < 964) {
    vel = 5;
  }

  if (ve > 694 && ve < 1130) {
    vel = 6;
  }

  else if (ve >= 1130 && ve < 1296) {
    vel = 7;
  }

  else if (ve >= 1296 && ve < 1462) {
    vel = 8;
  }

  else if (ve >= 1462 && ve < 1628) {
    vel = 9;
  }

  else if (ve >= 1628 && ve < 1794) {
    vel = 10;
  }

  else if (ve >= 1794 && ve < 1960) {
    vel = 11;
  }

  else if (ve >= 1960) {
    vel = 12;
  }

  else
    vel = 0;

  return vel;
}
