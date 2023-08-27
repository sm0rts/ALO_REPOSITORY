static float observed_angular_velocity[4] = {1,-1,-1,1};
static float observed_Vb[3] = {2,3,5};
const int l = 589;
const int w = 620;
const int r = 102000000;
const int KH0 = 0.25*r/(l+w);
const float H_0_inverse[3][4] = {
  {   -KH0,      KH0,      KH0,       -KH0   },
  {   0.25*r,   0.25*r,   0.25*r,    0.25*r  },
  {  -0.25*r,   0.25*r,  -0.25*r,    0.25*r  },};

void get_observed_Vb() {
  for (int i = 0; i < 3; i++) {
    observed_Vb[i] = 0;
    for (int j = 0; j < 4; j++) {
      observed_Vb[i] =
        observed_Vb[i]+
        H_0_inverse[i][j]*
        observed_angular_velocity[j];
    }
  }
}

void setup() {Serial.begin(115200);}

void loop() {
  Serial.println(KH0);
  delay(1000);
  get_observed_Vb();
  for (int i = 0; i <3 ; i++){
    //Serial.println(observed_Vb[i]);
  }
}
