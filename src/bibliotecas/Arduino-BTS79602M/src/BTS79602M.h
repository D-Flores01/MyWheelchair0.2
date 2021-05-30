/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#ifndef BTS79602M_h
#define BTS79602M_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class BTS79602M
{
  public:
	BTS79602M(uint8_t EN1, uint8_t L_PWM1, uint8_t R_PWM1,uint8_t EN2, uint8_t L_PWM2, uint8_t R_PWM2) : BTS79602M(EN1, 0, L_PWM1, R_PWM1,EN2, 0, L_PWM2, R_PWM2) {}
    BTS79602M(uint8_t L_EN1, uint8_t R_EN1, uint8_t L_PWM1, uint8_t R_PWM1,uint8_t L_EN2, uint8_t R_EN2, uint8_t L_PWM2, uint8_t R_PWM2);
    void Enable();
    void Disable();

	void TurnLeftM1(uint8_t pwm);

	void TurnLeftM2(uint8_t pwm);

	void TurnRightM1(uint8_t pwm);

        void TurnRightM2(uint8_t pwm);
	void Stop();

  private:
    uint8_t _L_EN1;
    uint8_t _R_EN1;
    uint8_t _L_PWM1;
    uint8_t _R_PWM1;


    uint8_t _L_EN2;
    uint8_t _R_EN2;
    uint8_t _L_PWM2;
    uint8_t _R_PWM2;
};
#endif
