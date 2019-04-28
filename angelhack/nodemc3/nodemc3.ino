#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "1234";
const char* password =  "12345678";
const char* mqttServer = "cloud.menbha.com";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
int led=2;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  pinMode(2,OUTPUT);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  //client.publish("esp/down","1");
  //Serial.print(client.subscribe("esp/up"));
  if((client.subscribe("esp/up"))==1)
  {
    digitalWrite(led,HIGH);
  }
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    digitalWrite(led,HIGH);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
  Serial.print(client.subscribe("esp/down"));
  digitalWrite(led,HIGH);
}
