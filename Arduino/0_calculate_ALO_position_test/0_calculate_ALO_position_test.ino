static float Q_observed[4] = {500,-600,-600,600};
static int Vb_observed[3] = {2,3,5};
static int Vs_observed[3] = {2,3,5};
static int P_observed[3] = {0,0,0};
const float dt = 0.005;
const float l = 0.589;
const float w = 0.620;
const float KH0 = 1/(l+w);
float phi = 1000;
float phi_rad = 0;

const float H_0_inverse[3][4] = {
  { -KH0, KH0, KH0, -KH0 },
  {   1,   1,   1,    1  },
  {  -1,   1,  -1,    1  },};
float H_Phi[3][3]{
  {1,      0,        0    },
  {0,   cos(phi), sin(phi)},
  {0,  -sin(phi), cos(phi)}};
  
void get_Vb_observed() {
  for (int i = 0; i < 3; i++) {
    Vb_observed[i] = 0;
    for (int j = 0; j < 4; j++) {
      Vb_observed[i] =
        Vb_observed[i]+
        H_0_inverse[i][j]*
        Q_observed[j];
    }}}

void get_H_Phi(){
  phi = phi+ dt*Vb_observed[0];
  phi_rad = phi/1000;
  float a = cos(phi_rad);
  float b = sin(phi_rad);
  H_Phi[1][1] = a;
  H_Phi[1][2] = b;
  H_Phi[2][1] =-b;
  H_Phi[2][2] = a;
}

void get_Vs_observed(){
  for (int i = 0; i < 3; i++) {
    Vs_observed[i] = 0;
    for (int j = 0; j < 3; j++) {
      Vs_observed[i] =
        Vs_observed[i]+
        H_Phi[i][j]*
        Vb_observed[j];
    }}}
    
void get_P_observed(){
  for (int i =0; i<3 ;i++){
    P_observed[i]= P_observed[i]+dt*Vs_observed[i];
  }
  
}
void setup() {Serial.begin(115200);}

void loop() {
  for (int k =0; k < 200;k++){
    delay(5);
    get_Vb_observed();
    get_H_Phi();
    get_Vs_observed();
    get_P_observed();
  }
  Serial.println("");
  Serial.println("  Vb\t Vs\t qs");
  for (int i = 0; i <3 ; i++){
    Serial.print(Vb_observed[i]);
    Serial.print("\t");
    Serial.print(Vs_observed[i]);
    Serial.print("\t");
    Serial.println(P_observed[i]);
  }
}