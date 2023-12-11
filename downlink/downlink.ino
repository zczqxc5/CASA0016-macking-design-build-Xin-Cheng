#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <Wire.h>                                                         //调用库文件
#include <LiquidCrystal_I2C.h>                                            //调用库文件
 
LiquidCrystal_I2C lcd(0x3F, 16, 2);                                       //配置LCD地址及行列
 
 
#define lcd1602TimeInterval 1000                                          //检测一次的时间间隔                   
unsigned long lcd1602Times = 0;                                           //记录设备运行时间
// 替换为您的WiFi网络名称和密码
const char* ssid = "CE-Hub-Student";
const char* password = "casa-ce-gagarin-public-service";
const char* mqttuser = "datasense@ttn";
const char* mqttpass = "NNSXS.NHPXHJCKORY4WGEK2UDYXNETHH3RC2MGBTLKEDI.GJ3GL5LODBZB6UKXNCKFFC3GQM3S6ZPEP5G6QKALX53XLOMW76UA";
// 替换为您的MQTT代理地址和端口
const char* mqttServer = "eu1.cloud.thethings.network";
const int mqttPort = 1883;

int buzzer_pin = 14;
int length;

WiFiClient espClient;
PubSubClient client(espClient);
Servo myServo; // 创建伺服电机对象
//中音NTF 0为空拍
#define NTF0 -1
#define NTF1 350
#define NTF2 393	
#define NTF3 441
#define NTF4 495
#define NTF5 556
#define NTF6 624
#define NTF7 661

//高音NTFH
#define NTFH1 700
#define NTFH2 786
#define NTFH3 882
#define NTFH4 935
#define NTFH5 965
#define NTFH6 996
#define NTFH7 1023

//低音NTFL
#define NTFL1 175
#define NTFL2 196
#define NTFL3 221
#define NTFL4 234
#define NTFL5 262
#define NTFL6 294
#define NTFL7 330

//音符频率数组 
int tune[]=
{
	NTF3,NTF3,NTF3,NTF3,NTF3,NTF3
	
};
//音符节拍数组
float durt[]=
{
	0.5,0.5,1,0.5,0.5,1
};
void setup() {
  lcd.init();                                                             //初始化LCD
  lcd.backlight();                                                        //打开背光   注意：如果电流不够的时候，可以将背光关闭
  Serial.begin(115200);
  myServo.attach(2,500,1700);

  // 连接到Wi-Fi网络
  setup_wifi();

  // 设置MQTT服务器和端口
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.setBufferSize(4096);
  length = sizeof(tune)/sizeof(tune[0]);
  // 连接到MQTT
  reconnect();
}

void loop() {  
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

  // 解析 JSON
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
    myServo.write(180); // 旋转伺服电机到180度
      play();
    }
    else {
    myServo.write(0);
    }
    
  int roundedTem = round(temperature);
 
  int roundedHum = round(humidity);

  if (millis() - lcd1602Times >= lcd1602TimeInterval) {                   //一定时间执行一次
    lcd1602Times = millis();
 
    lcd.setCursor(0, 0);                                                  //设置显示位置
    lcd.print("Temperature:");    
    lcd.setCursor(12, 0); 
    lcd.print(roundedTem);   
                                             //显示字符数据
    lcd.setCursor(0, 1);                                                  //设置显示位置
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

void play(){
  for(int x=0;x<length;x++)
	{
		tone(buzzer_pin, tune[x]);
    
		delay(500*durt[x]);	
    digitalWrite(ledp,LOW);	
    delay(100*durt[x]);				//这里的500为控制每个音符的时长来定曲子的节奏
		noTone(buzzer_pin);
	}
}
