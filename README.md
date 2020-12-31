# Pommodoro Timer based on Arduino

The circuit:

LCD:
* LCD RS pin to digital pin 12
* LCD Enable pin to digital pin 11
* LCD D4 pin to digital pin 5
* LCD D5 pin to digital pin 4
* LCD D6 pin to digital pin 3
* LCD D7 pin to digital pin 2
* LCD R/W pin to ground
* LCD VSS pin to ground
* LCD VCC pin to 5V
* 10K resistor:
* ends to +5V and ground
* wiper to LCD VO pin (pin 3)

Buttons:
* digital pin 8:
* digital pin 9: START/STOP button

LEDs:
* digital pin 6: "Pommodoro in progress" LED, I use a red LED
* digital pin 7: "Waiting" LED, I use a green LED

