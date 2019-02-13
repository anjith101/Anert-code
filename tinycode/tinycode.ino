int mode0;
int mode1;
void setup() {
  // put your setup code here, to run once:
pinMode (1, OUTPUT);
pinMode (0, OUTPUT);
digitalWrite(1, LOW);
digitalWrite(0, LOW);
pinMode (2, INPUT_PULLUP);
pinMode (4, INPUT_PULLUP);
mode0 = 0;
mode1 = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(2) == LOW){
    if(mode0 == 0){
      analogWrite(1, 158);
      mode0 = 1;
    }
    else if(mode0 == 1){
      digitalWrite(1, LOW);
      mode0 = 0;
    }
  }
  if(digitalRead(4) == LOW){
    if(mode1 == 0){
      analogWrite(0, 158);
      mode1 = 1;
    }
    else if(mode1 == 1){
      digitalWrite(0, LOW);
      mode1 = 0;
    }
  }

}
