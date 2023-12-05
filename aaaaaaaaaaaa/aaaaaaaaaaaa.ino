// First install "DHT sensor library" via the Library Manager
#include <DHT.h>

#include <MKRWAN.h>
#include <ArduinoLowPower.h>

const char *appEui = "0000000000000000";
const char *appKey = "11786519385ACA2323CF7A82F96C09CB";


#define debugSerial Serial
#define DHTPIN 2  //Digital Pin 2
#define DHTTYPE DHT22

int lightSensorPin = A0; //A0 LIGHTPIN

DHT dht(DHTPIN, DHTTYPE);

LoRaModem modem;


const int sensorMin = 0; // sensor minimum
const int sensorMax = 1024; // sensor maximum



#include <Wire.h>
#include <math.h>

 
int BH1750address = 0x23;//BH1750 I2C地址  //ADDR="L" for this module
byte buff[2];


unsigned long previousMillis = 0; // 保存上次触发时间
const long interval = 2000; // 定时器间隔时间，2000毫秒（2秒）


void setup(){

  Wire.begin();

  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  //Initialise the DHT sensor
  dht.begin();
  
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
  }
 
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
  }
 
}


void loop(){

  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
    // 保存下次触发的时间
    previousMillis = currentMillis;

    // 执行定时任务
    Serial.print(BH1750());
    Serial.println("[lux]");
  }



  debugSerial.println("-- LOOP");

  uint16_t humidity = 100*dht.readHumidity(false);

  // false: Celsius (default)
  // true: Farenheit
  uint16_t temperature = 100*dht.readTemperature(false);
  uint16_t sensorValue = analogRead(lightSensorPin); // 读取光敏传感器的值

  debugSerial.print("Temperature: ");
  debugSerial.println(temperature);
  debugSerial.print("Humidity: ");
  debugSerial.println(humidity);
  debugSerial.print("Light: ");
  debugSerial.println(sensorValue); // 在串行监视器中打印这个值
  
  byte payload[7];
  payload[0] = highByte(temperature);
  payload[1] = lowByte(temperature);
  payload[2] = highByte(humidity);
  payload[3] = lowByte(humidity);
  payload[4] = highByte(sensorValue);
  payload[5] = lowByte(sensorValue);

  //raindrop sensor
  // read the sensor on analog A0:
  int sensorReading = analogRead(A0);
  // map the sensor range (four options):
  // ex: ‘long int map(long int, long int, long int, long int, long int)’
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);

  // range value:
  switch (range) {
  case 0: // Sensor getting wet
  Serial.println("Flood");
  payload[6] = 1;
  break;
  case 1: // Sensor getting wet
  Serial.println("Rain Warning");
  payload[0] = 2;
  break;
  case 2: // Sensor dry – To shut this up delete the ” Serial.println(“Not Raining”); ” below.
  Serial.println("Not Raining");
  payload[0] = 3;
  break;
  }

  

  modem.beginPacket();
  modem.write(payload, 6);
  int err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message");
  }
  
  
  



   LowPower.deepSleep(10000);
}





double BH1750() {
  int i = 0;
  double val = 0;

  // 开始I2C通信
  Wire.beginTransmission(BH1750address);
  Wire.write(0x10); // 使用write代替send
  Wire.endTransmission();

  delay(200);

  // 读取数据
  Wire.beginTransmission(BH1750address);
  Wire.requestFrom(BH1750address, 2);
  while (Wire.available()) {
    buff[i] = Wire.read();  // 使用read代替receive
    i++;
  }
  Wire.endTransmission();

  if (i == 2) {
    val = ((buff[0] << 8) | buff[1]) / 1.2;
  }
  return val;
}
