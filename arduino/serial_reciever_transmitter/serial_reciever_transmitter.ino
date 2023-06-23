String myCmd;
String dump;


void setup() {
  //serial com setup
  Serial.begin(9600);

  pinMode(PC13, OUTPUT);
  //interrupt setup
}

void loop() {// constantly checks if there is any serial com
  while(Serial.available()==0){
      //loop while no data
  }
  myCmd = Serial.readStringUntil('\r');
  if (myCmd == "ON"){
    digitalWrite(PC13, LOW);
    Serial.println("LED is ON");
  }
  if (myCmd == "OFF"){
    digitalWrite(PC13, HIGH);
    Serial.println("LED is OFF");
  }
  else if(myCmd != "ON" && myCmd !="OFF"){
    Serial.println("unknown command");
  }
  dump = Serial.readStringUntil('\r');
}
