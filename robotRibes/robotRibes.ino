int enableA = 12;
int MotorA1 = 11;
int MotorA2 = 10;

int enableB = 5;
int MotorB1 =7;
int MotorB2 = 4;


void setup() {
  Serial.begin(9600);
  pinMode(enableA,OUTPUT);
  pinMode(MotorA1,OUTPUT);
  pinMode(MotorA2,OUTPUT);

  pinMode(enableB,OUTPUT);
  pinMode(MotorB1,OUTPUT);
  pinMode(MotorB2,OUTPUT);

}

void loop() {
  //enabling motor A
  Serial.println("Enabling Motors");
  digitalWrite(enableA,HIGH);
  digitalWrite(enableB,HIGH);
  delay(1000);
  Serial.println("Motion forward");
  digitalWrite(MotorA1,LOW);
  digitalWrite(MotorA2,HIGH);

  digitalWrite(MotorB1,LOW);
  digitalWrite(MotorB2,HIGH);
  delay(3000);
  
  Serial.println("Motion backwards");
  digitalWrite(MotorA1,HIGH);
  digitalWrite(MotorA2,LOW);

  digitalWrite(MotorB1,HIGH);
  digitalWrite(MotorB2,LOW);

  delay(3000);
  Serial.println("Stopping Motors");
  digitalWrite(enableA,LOW);
  digitalWrite(enableB,LOW);

  delay(3000);

}


