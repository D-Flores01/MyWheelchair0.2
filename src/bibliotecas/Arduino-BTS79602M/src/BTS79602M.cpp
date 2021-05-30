/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
#include <analogWrite.h> 
#include "BTS79602M.h"

BTS79602M::BTS79602M(uint8_t L_EN1, uint8_t R_EN1, uint8_t L_PWM1, uint8_t R_PWM1,uint8_t L_EN2, uint8_t R_EN2, uint8_t L_PWM2, uint8_t R_PWM2){

	_R_PWM1 = R_PWM1;
	_L_PWM1 = L_PWM1;
	_L_EN1 = L_EN1;
	_R_EN1 = R_EN1;

        _R_PWM2 = R_PWM2;
	_L_PWM2 = L_PWM2;
	_L_EN2 = L_EN2;
	_R_EN2 = R_EN2;




	pinMode(_R_PWM1, OUTPUT);
	pinMode(_L_PWM1, OUTPUT);
	pinMode(_L_EN1, OUTPUT);
	pinMode(_R_EN1, OUTPUT);
        pinMode(_R_PWM2, OUTPUT);
	pinMode(_L_PWM2, OUTPUT);
	pinMode(_L_EN2, OUTPUT);
	pinMode(_R_EN2, OUTPUT);
}

void BTS79602M::TurnRightM1(uint8_t pwm){
     analogWrite(_L_PWM1, 0);
	 delayMicroseconds(100);
     analogWrite(_R_PWM1, pwm);
}


void BTS79602M::TurnRightM2(uint8_t pwm){
     analogWrite(_L_PWM2, 0);
	 delayMicroseconds(100);
     analogWrite(_R_PWM2, pwm);
}


void BTS79602M::TurnLeftM1(uint8_t pwm){
     analogWrite(_R_PWM1, 0);
	 delayMicroseconds(100);
     analogWrite(_L_PWM1, pwm);
}


void BTS79602M::TurnLeftM2(uint8_t pwm){
     analogWrite(_R_PWM2, 0);
	 delayMicroseconds(100);
     analogWrite(_L_PWM2, pwm);
}


void BTS79602M::Enable(){
	digitalWrite(_L_EN1,1);
	if(_R_EN1 != 0) digitalWrite(_R_EN1, HIGH);

digitalWrite(_L_EN2,1);
	if(_R_EN2 != 0) digitalWrite(_R_EN2, HIGH);   
}

void BTS79602M::Disable(){
	digitalWrite(_L_EN1,0);
	if(_R_EN1 != 0) digitalWrite(_R_EN1, LOW);

digitalWrite(_L_EN2,0);
	if(_R_EN2 != 0) digitalWrite(_R_EN2, LOW);


}

void BTS79602M::Stop(){
  analogWrite(_L_PWM1, LOW);
  analogWrite(_R_PWM1, HIGH);

  analogWrite(_L_PWM2, LOW);
  analogWrite(_R_PWM2, HIGH);

}
