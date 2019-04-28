void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    switch(Serial.read())
    {
      case '0': digitalWrite(13,LOW);
      break;
      case '1': digitalWrite(13, HIGH);
      break;
      default:
      break;
    }
  }

}
