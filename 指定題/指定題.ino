int AIN1 = 2;
int AIN2 = 3;
int BIN1 = 5;
int BIN2 = 6;
int PWMA = 7;
int PWMB = 12;
int L2 = 32;
int L1 = 34;
int M = 36;
int R1 = 38;
int R2 = 40;
void setup() {
  pinMode(L2,INPUT);
  pinMode(L1,INPUT);
  pinMode(M,INPUT);
  pinMode(R1,INPUT);
  pinMode(R2,INPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  pinMode(PWMB,OUTPUT);
  Serial.begin(9600);
}

void MotorSpeed(int A_speed, int B_speed) {
  if (A_speed >= 0) {
    digitalWrite(AIN1,HIGH);
    digitalWrite(AIN2,LOW);
  }
  else {
    digitalWrite(AIN1,LOW);
    digitalWrite(AIN2,HIGH);
    A_speed *= -1;
  }
  if (B_speed >= 0) {
    digitalWrite(BIN1,HIGH);
    digitalWrite(BIN2,LOW);
  }
  else {
    digitalWrite(BIN1,LOW);
    digitalWrite(BIN2,HIGH);
    B_speed *= -1;
  }
  analogWrite(PWMA,A_speed);
  analogWrite(PWMB,B_speed);
}

int weight[5] = {-2,-1,0,1,2};
int sensors[5] = {L2,L1,M,R1,R2};
void tracking() {
  int delta = 0;
  int high_num = 0;
  for (int i = 0; i < 5; i++) {
    delta += weight[i] * digitalRead(sensors[i]);
    high_num += digitalRead(sensors[i]);
  }
  delta /= high_num;
  if(delta > 1) {
    MotorSpeed(200,-200);
    return;
  }
  if (delta < -1) {
    MotorSpeed(-200,200);
    return;
  }
  MotorSpeed(120+130*delta,120);
  Serial.println(delta);
}

void reverse() {
  MotorSpeed(255,-255);
  delay(300);
  while(!digitalRead(M)) {
    MotorSpeed(150,-150);
    delay(10);
  } 
}
void right_turn() {
  while(!digitalRead(M)) {
    MotorSpeed(200,100);
    delay(10);
  }
}
void loop(){
  tracking();
  delay(100);
  if (digitalRead(R1) && digitalRead(R2) && digitalRead(M) && digitalRead(L2) && digitalRead(L1)){
    reverse();
    tracking();
    if (digitalRead(R1) && digitalRead(R2) && digitalRead(M) && digitalRead(L2) && digitalRead(L1))
      right_turn();
  }
}