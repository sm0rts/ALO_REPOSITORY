const int matrix1[4][3] = {
  //matrix collums 
  {1 , 2 , 3 },
  {3 , 4 , 5 },
  {3 , 4 , 5 },
  {6 , 7 , 8 }
};

const int matrix2[3][4] = {
  {1 , 2 , 3 , 4},
  {2 , 3 , 4 , 5},
  {3 , 4 , 5 , 6},
};

int observed_angular_velocity[4] = {1,2,3,20};
int  desired_angular_velocity[4] = {1,2,3,5};
int  desired_Vb[3] = {3,3,5};
int observed_Vb[3] = {2,3,5};

void get_desired_angular_velocity(){
  for (int i = 0; i < 4; i++){
    desired_angular_velocity[i] = 0;
    for (int j = 0; j < 3; j++){
      desired_angular_velocity[i]= 
      desired_angular_velocity[i]+
      matrix1[i][j]*desired_Vb[j];
    }
  }
}
void get_observed_robot_poisition(){
  for (int i = 0; i < 3; i++){
    observed_Vb[i] = 0;
    for (int j = 0; j < 4; j++){
      observed_Vb[i]= 
      observed_Vb[i]+
      matrix2[i][j]*observed_angular_velocity[j];
    }
  }
}

void setup() {Serial.begin(115200);}

void loop() {
  delay(1000);
  Serial.println("desired angular velocity");
  get_desired_angular_velocity();
  for(int k = 0; k <4 ;k++){
    Serial.println(desired_angular_velocity[k]);
  }
  Serial.println("observed Vb");
  get_observed_robot_poisition();
  for(int k = 0; k <3 ;k++){
    Serial.println(observed_Vb[k]);
  }
}
