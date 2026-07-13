#include <SPI.h>
#include <LoRa.h>

#define measurementLed 8

//LoRa Pins
#define csPin 10
#define resetPin 7
#define irqPin 6

//SPI Interface Pins
#define SCK 13
#define MISO 12
#define MOSI 11

#define period 750//60000 * 5 // Minutos de delay entre coleta

#define startBut 6

int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode(measurementLed, OUTPUT);
  pinMode(startBut, INPUT);
  Serial.println("\nAwainting button press for start...");
  while(!digitalRead(startBut)){
    delay(10);
  }
  Serial.println("Starting experiment.");
  digitalWrite(measurementLed, HIGH);
  while(count <= 100){
    delay(4300);
    count++;
  }
  digitalWrite(measurementLed, LOW);
}

void loop() {
}
