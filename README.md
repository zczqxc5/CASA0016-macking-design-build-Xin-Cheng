# CASA0016-macking-design-build-Xin-Cheng

## Overview
The goal of this project is to develop an interactive and educational connected sensors system. This system not only provides real-time environmental data, but also alerts users to engage in outdoor activities when the weather is favourable. The system comprises two distinct yet interconnected devices

https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/b92368a096995f71f4d00a8a3bcd61d.jpg



## Functions
### 1. Real-time sensor data monitoring
Get Moisture, Temperature(/C),Temperature(#F), Humidity data via DHT22, GET Distance data from ultrasonic rangefinder sensor.
### 2. Live data presentation and transmission
Send data via wifi to CE MQTT server, and historic data (Distance,Temperature(/C),Temperature(#F), Humidity) could be stored in a webserver.
### 3. Remind user to water plant 
When plant needs water, get close to it, the LED light Fast flashing <br>
When plant don't need water, get close to it, it plays music “jingle bell”

## Prerequisites
· Raspberry Pi<br>
· MQTT protocol<br>
· pre-designed Huzzah(with WIFI module)<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/a4bf1737a54f534ed13b4dd59454edd.png" width="400px"><br>
· Sensors: nails, DHT22, ultrasonic rangefinder <br>
· Actuators: LED, active buzzer <br>
· libraries(see in code)
· resistor<br>

## How to reproduce
See detail steps in CASA0014-Connected Environment Workshop. https://workshops.cetools.org/codelabs/CASA0014-2-Plant-Monitor/index.html?index=..%2F..casa0014#0

1. Set up the circuit on pre-designed ESP8266<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/a464000de3174844b494284d1e9cbd8.jpg" height="500px"><img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/7a7cc263fc17feafa366a0c61c7ca0d.jpg" height="500px"><br>
3. git clone code "Plant-Monitor.ino"<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/7a67e23657ca535a5b71973d5f24f92.png" width="200px"><br>
4. Configure Raspberry PI to be a cloud server and gateway
5. Flashing Raspberry PI SD card and installing InflusxDB, and Grafana software on it for data visualization and storage
