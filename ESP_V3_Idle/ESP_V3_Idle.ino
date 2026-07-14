int count = 0;
void setup() {
  Serial.begin(115200);
  pinMode(7,OUTPUT);
  pinMode(0, INPUT_PULLUP);
    Serial.println("\nAwainting button press for start...");
    while(digitalRead(0)){
      delay(10);
    }
    Serial.println("Starting experiment.");

  digitalWrite(7,HIGH);
  while(count <= 100){
    delay(1616);
    count++;
  }
  digitalWrite(7,LOW);

}

void loop() {
  // put your main code here, to run repeatedly
  
}
