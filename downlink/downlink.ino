#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// 替换为您的WiFi网络名称和密码
const char* ssid = "CE-Hub-Student";
const char* password = "casa-ce-gagarin-public-service";
const char* mqttuser = "datasense@ttn";
const char* mqttpass = "NNSXS.NHPXHJCKORY4WGEK2UDYXNETHH3RC2MGBTLKEDI.GJ3GL5LODBZB6UKXNCKFFC3GQM3S6ZPEP5G6QKALX53XLOMW76UA";
// 替换为您的MQTT代理地址和端口
const char* mqttServer = "eu1.cloud.thethings.network";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // 连接到Wi-Fi网络
  setup_wifi();

  // 设置MQTT服务器和端口
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.setBufferSize(4096);

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
