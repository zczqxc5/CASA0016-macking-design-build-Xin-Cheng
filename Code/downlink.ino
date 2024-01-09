#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <Wire.h>     
#include <NTPClient.h>
#include <WiFiUdp.h>                                                    //Call the library file
#include <LiquidCrystal_I2C.h>                                            //Call the library file
 
LiquidCrystal_I2C lcd(0x3F, 16, 2);                                       //Configure the LCD address and column
 
 
#define lcd1602TimeInterval 1000                                          //Detect the time interval of a test                   
unsigned long lcd1602Times = 0;                                           //Record device running time

const char* mqttServer = "eu1.cloud.thethings.network";
const int mqttPort = 1883;

int buzzer_pin = 14;
const int noteDo = 262;
const int noteRe = 294;
const int noteMi = 330;
const int noteFa = 349;
const int noteSo = 392;

int currentHour;
int currentMinute;

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); 

WiFiClient espClient;
PubSubClient client(espClient);
Servo myServo; // Create a servo motor object

void setup() {
  lcd.init();                                                             //Initialize LCD
  lcd.backlight();                                                        //Turn on the backlight  
  Serial.begin(115200);
  myServo.attach(2,500,1700);

  //Connect to a Wi-Fi network
  setup_wifi();

  // Set up MQTT server and port
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.setBufferSize(4096);
   pinMode(buzzer_pin, OUTPUT);
  //Connect to MQTT
  reconnect();
  WiFi.begin(ssid, password);
}

void loop() {  
  timeClient.update();
  currentHour = timeClient.getHours();
  currentMinute = timeClient.getMinutes();
    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      if (client.subscribe("v3/datasense@ttn/devices/eui-a8610a3233227908/up")) {
        Serial.println("Subscription successful");
      } else {
        Serial.println("Subscription failed");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.println("Callback triggered");

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  String payload;
  for (unsigned int i = 0; i < length; i++) {
    payload += (char)message[i];
  }
  Serial.println(payload);

  // Parsing JSON
  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("JSON deserialization failed: ");
    Serial.println(error.c_str());
    return;
  }

  JsonObject decoded_payload = doc["uplink_message"]["decoded_payload"];
  if (!decoded_payload.isNull()) {
    float temperature = decoded_payload["temperature"];
    float humidity = decoded_payload["humidity"];
    float light = decoded_payload["light"];
    float rain = decoded_payload["rain"];

    if (rain == 3 && light > 100) {
    myServo.write(180); // Rotate servo motor to 180 
  }
  
    else {
    myServo.write(0);
    }

     if (rain == 3 && light > 100 && currentHour >= 10 && currentHour < 15 && currentMinute == 0) {
    playMelody(); // Play melody
    delay(60000); // Wait one minute to avoid repeating in the same hour
     }

  int roundedTem = round(temperature);
 
  int roundedHum = round(humidity);

  if (millis() - lcd1602Times >= lcd1602TimeInterval) {                   //This command is executed once in a certain period
    lcd1602Times = millis();
 
    lcd.setCursor(0, 0);                                                  //Set display position
    lcd.print("Temperature:");    
    lcd.setCursor(12, 0); 
    lcd.print(roundedTem);   
                                          
    lcd.setCursor(0, 1);                                                  //Set display position
    lcd.print("Humidity:");    
     lcd.setCursor(12, 1);  
     lcd.print(roundedHum); 
}

    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Light: ");
    Serial.println(light);
    Serial.print("Rain: ");
    Serial.println(rain);
  } else {
    Serial.println("decoded_payload is null or not found");
  }
  
  
}

void playMelody() {
  int melody[] = {noteDo, noteRe, noteMi, noteFa, noteSo, noteSo, noteFa, noteMi, noteRe, noteDo};
  int noteDuration = 200; 
  int pauseBetweenNotes = 400; 

  for (int i = 0; i < 10; i++) {
    tone(buzzer_pin, melody[i], noteDuration);
    delay(noteDuration); // Play note
    noTone(buzzer_pin);
    delay(pauseBetweenNotes); // The space between notes
  }
}
