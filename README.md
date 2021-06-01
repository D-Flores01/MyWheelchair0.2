# **Silla de ruedas**


Desarrollo de control de una silla de ruedas programado con ESP32 para realizar los movimientos utilizando un joystick, y también inalambricamente desde el móvil por vía Bluetooth.

El estado del proyecto es un prototipo funcional que no se ha llegado a probar con paciente reales, por lo tanto, aún no se sabe la efectividad de ésta.


## **Componentes**

2 Baterias de 33Ah y 12V.   
2 Motores 3718B de 180w y hasta 9,4A a 24V. Cuentan con freno eléctrico.  
Una Antigua Silla de ruedas de la marca "QUICKIE".  
1 Joystick robusto, modelo JH-D400X-R4.  
ESP32 DEVKITC-32E.
Resistencias de diferentes valores para divisor de tensión, pull down, etc.  
Diodo rectificador 1N4001.  
Diodo zener de hasta 3,4V.  
2 Drivers BTS7960 para el control de los motores.  
Módulo de MOSFETs de 4 canales con transistores IRF540N.  
Buzzer piezoeléctrico DC que hace de bocina.  
2 Reguladores de voltaje LM2596S, uno para alimentar el ESP32 y el otro para regular el voltaje de los frenos.  
4 Botones robustos.  
5 LEDs, 3 verdes, 1 amarillo y uno rojo, para el divisor de tensión.  
Cables de diferentes grosores, de mayor grosor para los motores.  


## **Drivers de los motores**

Se ha utilizado la librería de L.Llamas, la cual podeis encontrar en su página web https://github.com/luisllamasbinaburo/Arduino-BTS7960, y a la que se le ha hecho una pequeña modificación duplicando las funciones existentes para que funcione con 2 drivers BTS7960, la librería modificada se encuentra en la carpeta src en el apartado de librerias.


## **Control vía Bluetooth**

Se controla la silla desde el telefono móvil a través de la aplicación de android llamada "*Dabble*", usando la libreria que se puede encontrar en el siguiente enlace https://www.arduinolibraries.info/libraries/dabble.

## **PCB**

No se ha llegado todavía a realizar una pcb del proyecto, pero si cuenta con 2 esquematicos, uno para los drivers y otro para el mando de control.

