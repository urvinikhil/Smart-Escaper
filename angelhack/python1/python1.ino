int data;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //initialize serial COM at 9600 baudrate
  //pinMode(LED_BUILTIN, OUTPUT); //make the LED pin (13) as output
  //digitalWrite (LED_BUILTIN, LOW);
  Serial.println("Hi!, I am Arduino");
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available())
  {
    data = Serial.read();
  }
  
  //if (data == '1')
  if(data>=0)
  Serial.write("Positive");
  else
  Serial.write("Negative");
  //digitalWrite (LED_BUILTIN, HIGH);
  
  //else if (data == '0')
  //digitalWrite (LED_BUILTIN, LOW);

}
