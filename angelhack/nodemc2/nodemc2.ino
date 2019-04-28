#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "1234";
const char* password =  "12345678";
const char* mqttServer = "cloud.menbha.com";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
WiFiClient espClient;
PubSubClient client(espClient);


int height0 = 0; //initial height
int height1 = 0; //final height
int time_left = 0; //for evacuation
int trig = 8;
int echo = 7;
int buzzer=6; //purple one at the end
int led_signal1=9;
int led1=2, led2=3, led3=4, led4=5;
const int analogInPin = A0; // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0; // value read from the pot
int outputValue = 0; // value output to the PWM (analog out)
//int piezoPin = 8;
void setup() 
{
  // initialize serial communications at 9600 bps:
  //Serial.begin(9600);

  /*WiFi module communications set up*/
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) 
    {
      Serial.println("connected");  
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
 // client.publish("esp/down", "{'lat':'1.0','long':'2.0','level':'3'}");
  Serial.print(client.subscribe("esp/up"));

  
  /*Other Pin configs*/
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
}


/*callback method for mttq client communications*/
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void loop() 
{
  client.loop();
  long t = 0, h = 0, hp = 0;
  // read the analog in value:
  Serial.write("Acessing sensor");
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  t = pulseIn(echo, HIGH);
 Serial.print("Inside UV");
  // Calculating distance 
  h = t / 58;
 
  h = h - 6;  // offset correction
  h = 50 - h;  // water height, 0 - 50 cm
  
  hp = 2 * h;  // distance in %, 0-100 %
  
  // Sending to computer
  Serial.println(h);

  if(h>=0)//checking for uv sensor
  {
    digitalWrite(led_signal1,HIGH);
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);

    
    /*calculating height change rate*/
    height1 = h;
    int rate = (height1-height0)/5; //rate in cm/s
    Serial.print("last height = ");
    Serial.print(height0);
    Serial.print("Current Height = ");
    Serial.print(height1);
    Serial.print("rate = ");
    Serial.println(rate);
    height0 = height1;
    
    
    
    sensorValue = analogRead(analogInPin);
    // map it to the range of the analog out:
    outputValue = map(sensorValue, 0, 1023, 0, 255);
    // change the analog out value:
    analogWrite(analogOutPin, outputValue);
    
    // print the results to the Serial Monitor:
    Serial.print("sensor = ");
    Serial.print(sensorValue);
    Serial.print("\t output = ");
    Serial.println(outputValue);     /*Apply serial.read(output value)*/


    /*Condition for water flow sensor*/
    /*Serial.print("Enter the output value");
    while(Serial.available()==0);
    int ov = Serial.read();
    Serial.print(ov);*/
    if (outputValue>80 && rate>=0) //checking for water level sensor
    {
      Serial.print(outputValue);
      tone(buzzer,1000,500);
      delay(1000);
      //send signal and delay(5000);
      time_left = (5-height1)/rate;
      Serial.print("The time left is");
      Serial.println(time_left);
      delay(1000);
      if(time_left <=0)
      {
        //Ask user which doors to open
        Serial.println("Enter 1 2 3 or 4 and 5 to exit");
        while(Serial.available()==0);
        //int val = Serial.read();
         client.publish("esp/down", "1");
        while(val!='5')
        {
          Serial.print("val = ");
          Serial.println(val);
          if(val=='1')
          digitalWrite(led1,HIGH);
          if(val=='2')
          digitalWrite(led2,HIGH);
          if(val=='3')
          digitalWrite(led3,HIGH);
          if(val=='4')
          digitalWrite(led4,HIGH);
          while(Serial.available()==0);
          val=Serial.read();
        }
        delay(1000);
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        digitalWrite(led4,LOW);
      }
      delay(1000);// delay between beeps
    }
    
    // wait 2 milliseconds before the next loop for the analog-to-digital
    // converter to settle after the last reading:
    delay(2);
    }
   /* else
    {
      digitalWrite(buzzer,LOW);
      Serial.print("\n");
    }*/ 
}
