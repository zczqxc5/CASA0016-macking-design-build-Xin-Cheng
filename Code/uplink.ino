// First install "DHT sensor library" via the Library Manager
#include <DHT.h>
#include <Wire.h>  // The I2C library is introduced
#include <MKRWAN.h>
#include <ArduinoLowPower.h>

const char *appEui = "0000000000000000";
const char *appKey = "11786519385ACA2323CF7A82F96C09CB";


#define debugSerial Serial
#define DHTPIN 2  //Digital Pin 2
#define DHTTYPE DHT22

// BH1750 I2C address
int BH1750address = 0x23;

int lightSensorPin = A0;  //A0 LIGHTPIN

DHT dht(DHTPIN, DHTTYPE);

LoRaModem modem;


const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum


// Stores two bytes of data read from the sensor
byte buff[2];


void setup() {

  delay(30000);

  debugSerial.begin(9600);

  Wire.begin();  
  
  BH1750_Init(BH1750address);
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


void loop() {
  debugSerial.println("-- LOOP");
  
  // Read light intensity
  float lux = BH1750_ReadLux(BH1750address);
  int luxInt = (int)(lux * 100);



  uint16_t humidity = 100 * dht.readHumidity(false);

  // false: Celsius (default)
  // true: Farenheit
  uint16_t temperature = 100 * dht.readTemperature(false);


  debugSerial.print("Temperature: ");
  debugSerial.println(temperature);
  debugSerial.print("Humidity: ");
  debugSerial.println(humidity);
  // Print the read light intensity on the serial monitor
  debugSerial.println("Light: ");
  debugSerial.print(luxInt);


  byte payload[7];
  payload[0] = highByte(temperature);
  payload[1] = lowByte(temperature);
  payload[2] = highByte(humidity);
  payload[3] = lowByte(humidity);
  payload[4] = highByte(luxInt);
  payload[5] = lowByte(luxInt);

  //raindrop sensor
  // read the sensor on analog A0:
  int sensorReading = analogRead(A0);
  // map the sensor range (four options):
  // ex: ‘long int map(long int, long int, long int, long int, long int)’
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);

  // range value:
  switch (range) {
    case 0:  // Sensor getting wet
      Serial.println("Flood");
      payload[6] = 1;
      break;
    case 1:  // Sensor getting wet
      Serial.println("Rain Warning");
      payload[6] = 2;
      break;
    case 2:  // Sensor dry – To shut this up delete the ” Serial.println(“Not Raining”); ” below.
      Serial.println("Not Raining");
      payload[6] = 3;
      break;
  }



  modem.beginPacket();
  modem.write(payload, 7);
  int err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message");
  }
  LowPower.deepSleep(60000);
}

void BH1750_Init(int address) {
  Wire.beginTransmission(address);
  // Send a command to the sensor to start the measurement
  Wire.write(0x10);  // 1lx resolution, 120ms measurement time

  Wire.endTransmission();
}

float BH1750_ReadLux(int address) {
  // Request 2 bytes of data
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);

  // reading data

  if (Wire.available()) {
    buff[0] = Wire.read();  
    buff[1] = Wire.read();  
  }
  Wire.endTransmission();

 
  float lux = ((buff[0] << 8) | buff[1]) / 1.2;
  return lux;
}
