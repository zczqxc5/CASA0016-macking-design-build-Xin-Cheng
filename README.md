# CASA0016-macking-design-build-Xin-Cheng

## ✨ Overview
The goal of this project is to develop an interactive and educational connected sensors system. This system not only provides real-time environmental data, but also alerts users to engage in outdoor activities when the weather is favourable. The system comprises two distinct yet interconnected devices<br>
<img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/weather%20architecture.png"><br>
<br>


## 📕 Functions
### Outdoor device: Sense & transmit environmental data
The outdoor component is equipped with sensors collecting environmental data, including sunlight intensity, rainfall, temperature, and humidity levels. This data is transmitted using LoRa technology.<br>
<img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/b92368a096995f71f4d00a8a3bcd61d.jpg" width="400px"><br>

### Indoor device: 
1. Servo controlled sun face---showing a smiling face in clear, rainless conditions, and a sad face otherwise
2. Screen---dispaly data of temperature & humidity
3. Sound---Play sound between 10-15 (one time per hour) when the weather is good out side<br>
<img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/1724fa920981c3b5d56a8d092e87ba5.jpg" width="300px"><img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/d8acf7c89188b6c385a391ac87f4fb3.jpg" width="300px"><br>
<br>
 

## ⚙ Material List
<img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/a396f846d5060c7879a8871fbb6a116.png" width="600px"><br>
<br>


## 🔍 How to reproduce
### 1. Connect circuit as follow:
<img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/0d4d23f186c4b5db83dcbaf0f865f0a.png" width="600px"><br>
<img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/b3461ce087e138f5aa7e03dc9f5504d.png" width="600px"><br>

### 2. Download code from 'code' folder, and upload tem to Arduino MKR WAN 1310 & ESP8266 respectively

### 3. Set up network server
In The Things Network (TTN), an application is established to receive information from the Arduino MKR WAN 1310 board. By modifying the information in the uplink's payload formatters, the data can then be accessed in the live data section.<br>
<img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/Weixin%20Screenshot_20240110011127.png" ><br>
<img src="https://github.com/zczqxc5/CASA0016-macking-design-build-Xin-Cheng/blob/main/pic%26vid/Weixin%20Screenshot_20240110011342.png" width="350px"><br>

### 4. Use MQTT infomation fron TTN server, set that in your downloaded 'downlink' code file 

### 5.Dwonload 3d-printing and laser cuting files, make them, and set up your device


## 🔍 Reference
1. CASA0016 - making, design, build (no date) CE Workshops. Available at: https://workshops.cetools.org/casa0016/ (Accessed: 10 January 2024).
2. Abhishek0697 (no date) , GitHub. Available at: https://github.com/Abhishek0697/Rain-Sensing-Automatic-Car-Wiper-using-AT89C51-Microcontroller (Accessed: 10 January 2024).
3. How to make Sun Tracking Sunflower Robot using Arduino (no date) Tinkercad. Available at: https://www.tinkercad.com/projects/How-to-Make-Sun-Tracking-Sunflower-Robot-Using-Ard (Accessed: 10 January 2024). 
