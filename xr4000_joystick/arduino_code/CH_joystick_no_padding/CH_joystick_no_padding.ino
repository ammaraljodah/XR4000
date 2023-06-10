const int pot1InPin = A0;  
const int pot2InPin = A1;
const int button1InPin = 8;
const int button2InPin = 9;
int pot1 = 0;        
int pot2 = 0;        
int button1 = 0;
int button2 = 0;
void setup() {
  Serial.begin(9600);
  pinMode(button1InPin, INPUT_PULLUP);
  pinMode(button2InPin, INPUT_PULLUP);
}

void loop() {
  // read the analog in value:
  pot1 = analogRead(pot1InPin);
  pot2 = analogRead(pot2InPin);
  button1 = digitalRead(button1InPin);
  button2 = digitalRead(button2InPin);
  Serial.print(pot1);
  Serial.print(",");
  Serial.print(pot2);
  Serial.print(",");
  Serial.print(button1);
  Serial.print(",");
  Serial.print(button2);
  Serial.print("\n");

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
