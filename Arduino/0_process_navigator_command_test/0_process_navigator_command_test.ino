static int P_desired[3] = {200,1000,400};
static int P_observed[3] = {800,1000,5000};
static int Vb_desired[3] = {1000,0,0};
static int Vb_observed[3] = {1,5,1000};
void up_click_response()    {P_desired[1] = P_desired[1]+1000;}
void down_click_response()  {P_desired[1] = P_desired[1]-1000;}
void left_click_response()  {P_desired[0] = P_desired[0]-1000;}
void right_click_response() {P_desired[0] = P_desired[0]+1000;}
void print_status(){
  Serial.println("P_desired P_observed V_desired V_observed");
  for (int i = 0; i < 3; i++){    
    Serial.print(P_desired[i]);
    Serial.print("\t    ");
    Serial.print(P_observed[i]);
    Serial.print("\t    ");
    Serial.print(Vb_desired[i]);
    Serial.print("\t    ");
    Serial.println(Vb_observed[i]); }}
void process_motor_command(){
  String myCmd = Serial.readStringUntil('\r'); 
  if (myCmd == "REQ"){print_status();}
  if (myCmd == "UP")    {up_click_response();   }
  if (myCmd == "DOWN")  {down_click_response(); }
  if (myCmd == "LEFT")  {left_click_response(); }
  if (myCmd == "RIGHT") {right_click_response();}
  Serial.print(myCmd);
  Serial.println("");}
void setup() {Serial.begin(115200);}
//void loop () {if(Serial.available()==1){process_motor_command();}}
void loop () {print_status();delay(1000);}