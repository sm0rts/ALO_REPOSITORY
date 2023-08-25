const int l = 800; 
const int w = 800;
const int r = 250;

float Pd[3] = {0,0,0};
float P[3] = {0,0,0};
float Ep[3] = {0,0,0};
float Kp[3] = {0,0,0};

float Vd[3] = {0,0,0};
float V[3] = {0,0,0};

float Ud[4] = {0,0,0,0};
float U[4] = {0,0,0,0};
float Eu[4] = {0,0,0,0};
float Ku[4] = {0,0,0,0};

const int H0[4][3] = {
  {   -l-w, 1 , -1   },
  {    l+w, 1 ,  1   },
  {    l+w, 1 , -1   },
  {   -l-w, 1,   1   }
};

int add_vec(float V1[], float V2[]){
  float Vr[3] = {0};
  for(int i = 0; i = 2; i++){
    Vr[i] = V1[i]+V2[i];
  }
}


int desired_velocity_calculator(int Pd[3], int P[3]){

}

int motor_input_command_calculator(int Ud[3], int U[3]){
  int I = Ud-U;
  return(I);
}

void setup() {
  Serial.begin(115200);

}

void loop() {
  add_vec(Vd,V);
  Serial.println("");
  delay (1000);

}
