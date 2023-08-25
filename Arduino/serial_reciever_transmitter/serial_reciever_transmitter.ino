String myCmd;
String dump;


void setup() {
  //serial com setup
  Serial.begin(115200);

  pinMode(PC13, OUTPUT);
  //interrupt setup
}

void loop() {// constantly checks if there is any serial com
  while(Serial.available()==0){
  }
  myCmd = Serial.readStringUntil('\r');
  if (myCmd == "UP"){
    Serial.write("UP");
    digitalWrite(PC13, LOW);
    Serial.println("LED is ON");
  }
  if (myCmd == "DOWN"){
    Serial.write("DOWN");
    digitalWrite(PC13, HIGH);
    Serial.println("LED is OFF");
  }
  if (myCmd == "LEFT"){
    Serial.write("DOWN");
    digitalWrite(PC13, HIGH);
    Serial.println("LED is OFF");
  }
  if (myCmd == "RIGHT"){
    Serial.write("DOWN");
    digitalWrite(PC13, HIGH);
    Serial.println("LED is OFF");
  }
  else if(myCmd != "ON" && myCmd !="OFF"){
    Serial.println("unknown command");
  }
  dump = Serial.readStringUntil('\r');
}
