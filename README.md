# Standing desk projet

```/projet5/table-2/```: KiCAD files<br>
```/projet/???/```: Arduino files

### Hardware
- 2x 12VDC Linear Actuators (12")
- ESP32 (To access the web and gather info for date, time, weather)
- Dual H-Bridge motor driver (each linear actuator pulls 5 A continuous)
- LED strip (5V Adressable LEDs)
- USB charging hub (2x USB-A & 1x USB-C)
- Ultrasonic sensor to detect when you go from standing to sitting
- MPU6050 (3 axis gyro and accelerometer) to measure the flatness of the table (in case the linear actuators are out of sync)
- 4 Debounced push buttons
- 1 SPDT toggle switch to control linear actuators (ON - OFF - ON)
- 1.8" OLED SPI display


### Schematic
![alt text](https://github.com/Jahjahhhh/projet5/blob/main/schematic.png?raw=true)
<br>

### PCB
![alt text](https://github.com/Jahjahhhh/projet5/blob/main/pcb.png?raw=true)

