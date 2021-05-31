  /*
  
  Wheelchair control adaptation.
  
  Motor movements through joystick and using mobile using bluetooh.
  
  ************************include*******************************
  -Control via bluetooth.
  
  -Speed buttons.
  
  -Acceleration ramp.
  
  -Chair movements with the joystick.
  
  -Movements of the chair with the mobile phone.
  
  -buzzer button
  
  -LED battery indicator
  
  -Battery indicator button
  
  
  Thanks to:
   -www.luisllamas.com for the library "BTS79602M.h" (modified by me to this proyect)
  
  
  Created in 2020.
  
  By David Flores Román.
  dfloresr@alumnos.unex.es
  
  Modified on May 31, 2021.
  
  */
  
  ///////////////////////////////////////////////////////////////////////////////
  /////////////////// Libraries and includes ////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////// 
  
  //bluetooth libraries
  #define CUSTOM_SETTINGS
  #define INCLUDE_GAMEPAD_MODULE
  #include <DabbleESP32.h>
  
  //Motors controlls libraries
  #include "BTS79602M.h"
  
  #include <analogWrite.h>
  
  ///////////////////////////////////////////////////////////////////////////////
  /////////////////// Variable definitions //////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  
  //motors electric brake
  int controlPIN= 12; 
  
  // delay 
  int V_update_period=80; //time in milliseconds
  unsigned long V_last_update = 0; //time since ESP32 is activated
  
  //Motor variable
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  const uint8_t EN1 = 19; //power for BTS1
  const uint8_t L_PWM1 = 18;//pin to turn the motor1 to the left
  const uint8_t R_PWM1 = 5;//pin to turn the motor1 to the Right
  
  const uint8_t EN2 = 4;//power for BTS2
  const uint8_t L_PWM2 = 16;//pin to turn the motor2 to the left
  const uint8_t R_PWM2 = 17;//pin to turn the motor2 to the Right
  
  BTS79602M motorController(EN1, L_PWM1, R_PWM1, EN2, L_PWM2, R_PWM2); // initialize motorController, function of BTS79602M.h
  
  //battery indicator with LEDs
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  float Div_v = 0; //battery power in volts
  float Div = 0; //value battery power 
  
  //definition of LEDs
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  const int ledPIN1 = 23;//green LED
  const int ledPIN2 = 22;//yellow LED
  const int ledPIN3 = 21; //red LED
  const int ledPIN4 = 0; //green LED2
  const int ledPIN5 = 2; //green LED3
  
  //button to turn on the LEDs
  
  const int buttonLED_PIN = 15; // pin LED boton
  
  bool aux = 0; // auxiliary button variable
  
  //joystick variables
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int Xvalue = 0; // X axis joystick
  int Yvalue = 0; // Y axis joystick
  int JX_PIN = 35; // pin 35 X axis
  int JY_PIN = 32; // pin 32 Y axis
  
  int threshold=200; //midpoint threshold 
  int Pmean=2048;//midpoint
  
  int X = 0; //saves X axis between -2048 to 2048
  int Y = 0; //saves Y axis between -2048 to 2048
  float auxi = 0; //auxiliary variable
  
  int Angle; //variable for angulos function
  int ang=0; // variable for saving the value of the angulos function
  int distance; //variable for range function
  
  int v=0;//pwm for motors 0 to 255
  
  int dif1=0; //speed difference as a function of angle to move forward and left
  int dif2=0; //speed difference as a function of angle to move back and left
  int dif3=0; //speed difference as a function of angle to move forward and right
  int dif4=0;//speed difference as a function of angle to move back and right
  
  //speed buttons
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  const int buttonHighPIN = 33; //pin 33 for high speed button
  const int buttonLowPIN = 25; //pin 25 for low speed button
  bool flag_b=0; //flag to switch from one button to another
  
  int buttonHigh = 0;//saves information of pin 33
  int buttonLow = 0; //saves information of pin 25
  
  //button and buzzer variable
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  const int BuzzButtonPIN = 26; // buzzer button
  const int BuzzPIN = 27; // voltage buzzer
  int lecb; //auxiliary variable
  
  //flag forward and back
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int flag_forward=0; //forward
  int flag_back=0; // back
  
  int v_d=0; //real speed of the motors

  
  int local; // auxiliary variable used in function angulos
  int TOL=5; //tolerance of velocity comparal
  int INC_v=3; //increase of real velocity
  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////Setup///////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////
  
  void setup() {
  
    //Serial.begin(115200);
  
    Dabble.begin("MyEsp32");       // bluetooth name
  
    /////////////////////////////////////////////////////////////////////////////
    pinMode(ledPIN1 , OUTPUT); //initialize digital pin ledPIN1 as an OUTPUT
    pinMode(ledPIN2 , OUTPUT);//initialize digital pin ledPIN2 as an OUTPUT
    pinMode(ledPIN3 , OUTPUT);//initialize digital pin ledPIN3 as an OUTPUT
    pinMode(ledPIN4 , OUTPUT);//initialize digital pin ledPIN4 as an OUTPUT
    pinMode(ledPIN5 , OUTPUT);//initialize digital pin ledPIN5 as an OUTPUT
    
    pinMode(BuzzPIN, OUTPUT);//initialize digital pin PINz as an OUTPUT 
    pinMode(controlPIN, OUTPUT);//initialize digital pin control  as an OUTPUT 
  
    pinMode(JX_PIN,INPUT);//initialize analog pin pinJX as an INPUT 
    pinMode(JY_PIN,INPUT);//initialize analog pin pinJY as an INPUT 
  
    pinMode(buttonLED_PIN  , INPUT_PULLDOWN);//initialize digital pin boton as an INPUT_PULLDOWN
    pinMode(buttonHighPIN, INPUT_PULLDOWN);//initialize digital pin PINjoy1 as an INPUT_PULLDOWN
    pinMode(buttonLowPIN, INPUT_PULLDOWN);//initialize digital pin PINjoy2 as an INPUT_PULLDOWN
    pinMode(BuzzButtonPIN, INPUT_PULLDOWN);//initialize digital pin PINbotz as an INPUT_PULLDOWN
  
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////Loop///////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  void loop() {
  
    Dabble.processInput(); // activate bluetooth to use the app
    
    motorController.Enable(); //active motors
  
    //joystick readings
    /////////////////////////////////////////////////////////////////////////////////////////////////
    Xvalue = analogRead ( JX_PIN); // reading X axis
    Yvalue = analogRead ( JY_PIN); // reading Y axis
  
  //change X and Y values ​​from -2048 to 2048
  ////////////////////////////////////////////////////////////////////////////////////////////////
    X=Xvalue-Pmean;
  
if(abs(X) < threshold){
    X=0;
}
                            //X and Y are 0 when the joystick is in the threshold
Y=Yvalue-Pmean;
  
if(abs(Y) < threshold){
   Y=0;
}
   
  //test code
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  
   // Serial.print("ejeX;  "); 
   // Serial.print(X);
    //Serial.print("  ejeY;  ");
    //Serial.print(Y);
    //Serial.print("   valor de v: ");
    //Serial.print(v);
    
    //Serial.print ("angulo: ");
    //Serial.print(angulos (X,Y));
  
    //Serial.print ("   velocidad: ");
    //Serial.println(v);
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  
    //readings speed buttons
    buttonHigh = digitalRead(buttonHighPIN);
    buttonLow = digitalRead(buttonLowPIN);
  
  
  
  //change from one speed button to another
if(buttonHigh==HIGH && buttonLow==LOW && v==0){
  
  flag_b=1;
}
  
if(buttonLow==HIGH && buttonHigh==LOW && v==0){
    
  flag_b=0;
}
  
  //if b is 0, speed is LOW and if b is 1, speed is HIGH 
if (flag_b == 0) {
       
       if(millis() > V_last_update + V_update_period){
            V_last_update = millis();
  
            v_d=(distancia(X, Y)/13);
  
                  if(v_d - v > TOL){             //comparison between desired speed and actual speed
                    v=v+INC_v;
                            }
        if ((v_d-v < TOL) && v>0){
            v=v-INC_v;
                            }
  
                   if(v<0){
                      v=0;
  
                           }
  
          }
 }
   
else {
      
        if(millis() > V_last_update + V_update_period){
              V_last_update = millis();
  
               v_d=(distancia(X, Y)/20)*2;
  
                 if(v_d - v > TOL){
                        v=v+INC_v;
                                 }
                  if ((v_d-v < TOL) && v>0){             //comparison between desired speed and actual speed
                         v=v-INC_v;
                               }
  
                      if(v<0){
                        v=0;
  
                           }
  
             }
  
 }
  
    ang=angulos(X,Y,v);
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //movement of the motors according to the joystick
  
    //left
if (ang >= 15 && ang <= 165 && v>0 ) { //range of values ​​depending on the quadrant in which the joystick is located
     // Serial.println("L");
      if (flag_forward == 1 && flag_back == 0){ //if it has gone forward before, turn left with an engine turning faster
  
          dif1=abs(165-angulos(X,Y,v));
  
            if((v-dif1)<=1){
               dif1=v-1;
                        }
  
           digitalWrite(controlPIN, HIGH);
           //delay(100);
  
           motorController.TurnLeftM2(v);
           motorController.TurnLeftM1((v)-dif1);
        
      }
      
      if(flag_back == 1 && flag_forward == 0){ //if it has gone backwards before, turn left with an engine turning faster
  
      dif2=abs(15-angulos(X,Y,v));
  
            if((v-dif2)<=1){
               dif2=v-1;
                   }
  
      digitalWrite(controlPIN, HIGH);
      //delay(100);
  
      motorController.TurnRightM2(v);
      motorController.TurnRightM1((v)-dif2);
        
      }
  
       if(flag_forward == 0 && flag_back == 0){ // if the joystick was initially at rest, turn left with one wheel forward and one wheel back
           digitalWrite(controlPIN, HIGH);
            //delay(100);
  
          motorController.TurnLeftM2(v/2);
           motorController.TurnRightM1(v/2);
                           }
  }
  
    //right
if (ang >= 195 && ang <= 345 && v>0) {//range of values ​​depending on the quadrant in which the joystick is located
  //Serial.println("R");
       if (flag_forward == 1 && flag_back == 0){//if it has gone forward before, turn right with an engine turning faster
  
      dif3=abs(195-angulos(X,Y,v));
  
          if((v-dif3)<=1){
              dif3=v-1;
                  }  
  
          digitalWrite(controlPIN, HIGH);
          //delay(100);
  
          motorController.TurnLeftM2((v)-dif3);
          motorController.TurnLeftM1(v);
      
      }
      if(flag_back == 1 && flag_forward == 0){//if it has gone backwards before, turn right with an engine turning faster
  
          dif4=abs(345-angulos(X,Y,v));
  
              if((v-dif4)<=1){
                  dif4=v-1;
                           }
  
         digitalWrite(controlPIN, HIGH);
        // delay(100);
  
         motorController.TurnRightM2((v)-dif4);
         motorController.TurnRightM1(v);
        
      }
  
      if(flag_forward == 0 && flag_back == 0){// if the joystick was initially at rest, turn right with one wheel forward and one wheel back
  
         digitalWrite(controlPIN, HIGH);
         // delay(100);
  
         motorController.TurnRightM2(v/2);
         motorController.TurnLeftM1(v/2);
         }
  
  }
  
    //forward
if (ang >= 170 && ang <= 190 && v>0 ) {//range of values ​​depending on the quadrant in which the joystick is located
      //Serial.println("F");
      
         if(flag_back==0 && flag_forward==0){
         flag_forward=1;
                   }
      
         if(flag_forward == 1 && flag_back==0){ //the chair goes forward unless the joystick is moved from state to another bypassing 0
        
             digitalWrite(controlPIN, HIGH);
     
              motorController.TurnLeftM2(v);
                motorController.TurnLeftM1(v);
  
                           }
         if(flag_back==1 &&flag_forward==0){
  
              motorController.TurnRightM2(v);
                motorController.TurnRightM1(v);
    
                          }
  
}
  
    //backward
if (((ang >= 0 && ang <= 10) || (ang >= 350 && ang <= 355)) && v>0) {//range of values ​​depending on the quadrant in which the joystick is located
  //Serial.println("B");
  
      if(flag_forward==0 && flag_back==0){
            flag_back = 1;
                           }
  
      if(flag_back == 1 && flag_forward==0){//the chair goes backwards unless the joystick is moved from state to another bypassing 0
        
            digitalWrite(controlPIN, HIGH);
     
            motorController.TurnRightM2(v);
            motorController.TurnRightM1(v);
      
                         }
            if(flag_forward==1 && flag_back==0){
  
            motorController.TurnLeftM2(v);
            motorController.TurnLeftM1(v);
    
                     }
    
 }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Serial.println(ang);
  
  aux = digitalRead(buttonLED_PIN); //reading pin 15
  
  //voltage divider
  
    Div = analogRead(34); //reading pin 34
    
    Div_v = ((Div * 25)/2300); //range conversion operation using ESP32 (0-2300) to volts.
  
  
    lecb = digitalRead(BuzzButtonPIN); //reading pin buzzer button
  
    //motors movement with bluetooth
    ////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //code test
  //Serial.print("velocidad deseada: ");
  //Serial.print(v_d);
  //Serial.print("         velocidad real: ");
  //Serial.println(v);
  
    //forward
    //////////////////////////////////////////////////////////////////////////////////////////////////////
  if (GamePad.isUpPressed() == 1 && GamePad.isLeftPressed() == 0 && GamePad.isRightPressed() == 0 && GamePad.isDownPressed() == 0 && v == 0) {
      digitalWrite(controlPIN, HIGH);
      delay(100);
      motorController.TurnLeftM2(50);
      motorController.TurnLeftM1(50);
      
    }
    
  //back
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  
  if (GamePad.isDownPressed() == 1  && GamePad.isUpPressed() == 0 && GamePad.isLeftPressed() == 0 && GamePad.isRightPressed() == 0 && v == 0) {
      digitalWrite(controlPIN, HIGH);
      delay(100);
      motorController.TurnRightM2(50);
      motorController.TurnRightM1(50);
      
    }
  
  //Stop motors
  /////////////////////////////////////////////////////////////////////////////////////////////////
  if (GamePad.isDownPressed() == 0 && GamePad.isUpPressed() == 0 && GamePad.isLeftPressed() == 0 && GamePad.isRightPressed() == 0 && v==0) { 
     motorController.Stop(); 
     motorController.Disable();
     digitalWrite(controlPIN, LOW);
  
     flag_forward=0;
     flag_back=0;
       
    }
  
    //Left
    /////////////////////////////////////////////////////////////////////////////////////////////////////
  if (GamePad.isLeftPressed () == 1 && GamePad.isDownPressed() == 0 && GamePad.isUpPressed() == 0  && GamePad.isRightPressed() == 0  && v == 0) {
      digitalWrite(controlPIN, HIGH);
      delay(100);
      motorController.TurnLeftM2(50);
      motorController.TurnRightM1(50);
   
    }
  
    //right
    //////////////////////////////////////////////////////////////////////////////////////////////////////
  if (GamePad.isLeftPressed () == 0 && GamePad.isDownPressed() == 0 && GamePad.isUpPressed() == 0  && GamePad.isRightPressed() == 1 && v == 0) {
      digitalWrite(controlPIN, HIGH);
      delay(100);
      motorController.TurnRightM2(50);
      motorController.TurnLeftM1(50);
      
    }
  
    //LEDs
    //////////////////////////////////////////////////////////////////////////////////////////////////////
  
    leds(aux, Div_v); //leds function call
  
  //buzzer
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  if (lecb == HIGH) { //if button is Hight buzzer sounds 
    
   digitalWrite(BuzzPIN, HIGH); //buzzer in high
  
         }
  
  else {
  
     digitalWrite(BuzzPIN, LOW); //buzzer in low
  
        }
  
}
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////Functions///////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //leds function
  ///////////////////////////////////////////////////////////////////
void leds(int aux, int Div_v) { //button and value of voltage
  
    if (aux == 1) {
  
      if (Div_v >= 24 ) {
  
        digitalWrite(ledPIN1 , HIGH);
        digitalWrite(ledPIN2 , HIGH);
        digitalWrite(ledPIN3 , HIGH);
        digitalWrite(ledPIN4 , HIGH);
        digitalWrite(ledPIN5 , HIGH);
  
                                   }
  
      if (Div_v >= 20 && Div_v < 24 ) {
        digitalWrite(ledPIN1 , HIGH);
        digitalWrite(ledPIN2 , HIGH);
        digitalWrite(ledPIN3 , HIGH);
        digitalWrite(ledPIN4 , HIGH);
        digitalWrite(ledPIN5 , LOW);
  
                                   }
           
      if (Div_v <= 18 && Div_v < 20 ) {
  
        digitalWrite(ledPIN1 , HIGH);
        digitalWrite(ledPIN2 , HIGH);
        digitalWrite(ledPIN3 , HIGH);
        digitalWrite(ledPIN4 , LOW);
        digitalWrite(ledPIN5 , LOW);
        
                                   }
    
      if (Div_v <= 15 && Div_v < 18 ) {
  
        digitalWrite(ledPIN1 , LOW);
        digitalWrite(ledPIN2 , HIGH);
        digitalWrite(ledPIN3 , HIGH);
        digitalWrite(ledPIN4 , LOW);
        digitalWrite(ledPIN5 , LOW);
        
                                    }
  
      if (Div_v < 15) {
  
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
  
  //angulos function
int angulos (int x, int y, int V) { //returns the angle from 0 to 359 at which the joystick is, calculating its arc tangent.
  
    auxi = atan2(x, y);
    Angle = (auxi * 180) / PI;
  
   if (Angle < 0) {
      Angle = Angle + 360;
    }
  
       if(v>0 && distancia(X,Y)>0 ){
         local=Angle;
                 }
  
           if( v==0 && distancia(X,Y)==0){ 
               return 14;
                     }
  
             else
                 if( v>0 && distancia(X,Y)==0){  
                      return local;
                           }
                             else
                                return Angle;
 }
  
  //Distance function
int distancia(int x, int y) { //returns the distance from the center of the joystick doing the modulus.
  
    distance = sqrt(pow(x, 2) + pow(y, 2));
  
    return distance;
  }
