// Pins
const int PWMA = 9;
const int AIN1 = 7;
const int AIN2 = 8;
const int STBY = 6;
const int ENC_A = 2;
const int ENC_B = 3;

volatile long encoderCount = 0;
volatile uint8_t lastState = 0;

// Quadrature decoding (4x): +1 / -1 per transition
void encoderISR() {
  static const int8_t table[16] = {0,-1,1,0, 1,0,0,-1, -1,0,0,1, 0,1,-1,0};
  uint8_t state = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
  encoderCount += table[(lastState << 2) | state];
  lastState = state;
}

void motorForward(uint8_t speed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);
}

void motorReverse(uint8_t speed) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, speed);
}

void motorStop() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);
}

void setup() {
  Serial.begin(115200);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(ENC_A, INPUT);   // encoder has its own 10k pull-ups
  pinMode(ENC_B, INPUT);

  digitalWrite(STBY, HIGH);  // enable driver

  lastState = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encoderISR, CHANGE);
}

void loop() {
  Serial.println("Forward");
  motorForward(255);
  delay(2000);
  motorStop();
  Serial.print("Count: "); Serial.println(encoderCount);
  delay(1000);

  Serial.println("Reverse");
  motorReverse(255);
  delay(2000);
  motorStop();
  Serial.print("Count: "); Serial.println(encoderCount);
  delay(1000);
}