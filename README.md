# BipBip - DIY Variometer

Bipbip is a simple but effective variometer for paragliding purpose.


When you are flying, it's hard to know if you climb or sink. This variometer make beeps with high frequency when going up and with low frequency when going down.

<img src="http://aduprat.com/pub/bipbip1.jpg" alt="drawing" width="200"/> <img src="http://aduprat.com/pub/bipbip2.jpg" alt="drawing" width="200"/>


## Hardware

- Arduino Nano 3
- Piezzo Buzzer
- Resistor 100ohm
- Pressure sensor BMP390L
- Switch on/off
- Battery 9v 1000mAh with usb charger

## Electronic

```
* 9V Battery :
  + -> Switch ON/OFF -> VIN
  - -> GND

* Buzzer : 
  + -> 100ohm resistor -> D9
  - -> D10 

* Pressure Sensor :
  GND -> GND
  VDD -> 3.3V
  SCL -> A5
  SDA -> A4  
```

## Software

Checkout `program.ino`

- Play a jingle at the beginning
- Average altitudes values each 500 ms
- Beep if you go up or down

