int count = 0;
void setup() {
  pinMode(7,OUTPUT);

  digitalWrite(7,HIGH);
  while(count <= 100){
    delay(177);
    count++;
  }
  digitalWrite(7,LOW);

}

void loop() {
  // put your main code here, to run repeatedly
  
}
