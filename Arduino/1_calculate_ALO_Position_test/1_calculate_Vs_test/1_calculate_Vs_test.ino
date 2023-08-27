static int observed_angular_velocity[4] = { 0, 0, 0, 0 };
static int observed_Vs[3] = { 2, 3, 5 };
static int observed_Vb[3] = { 2, 3, 5 };
const int H_Phi[3][3]{
  { 1, 2, 3 },
  { 2, 3, 4 },
  { 3, 4, 5 },
};
void get_observed_Vs() {
  for (int i = 0; i < 3; i++) {
    observed_Vb[i] = 0;
    for (int j = 0; j < 3; j++) {
      observed_Vs[i] =
        observed_Vs[i] + H_Phi[i][j] * observed_Vb[j];
    }
  }
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
