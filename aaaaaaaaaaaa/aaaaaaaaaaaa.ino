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

void setup(){

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
  
  byte payload[6];
  payload[0] = highByte(temperature);
  payload[1] = lowByte(temperature);
  payload[2] = highByte(humidity);
  payload[3] = lowByte(humidity);
  payload[4] = highByte(sensorValue);
  payload[5] = lowByte(sensorValue);

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
