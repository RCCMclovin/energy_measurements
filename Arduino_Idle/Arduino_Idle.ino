#include <SPI.h>
#include <LoRa.h>

#define measurementLed 8
#define resetPin 4

//LoRa Pins
#define csPin 10
#define resetPin 7
#define irqPin 6

//SPI Interface Pins
#define SCK 13
#define MISO 12
#define MOSI 11

#define period 750//60000 * 5 // Minutos de delay entre coleta

int TXPOWER = 20;
int count = 0;

char txpacket[101];
String msg;

void (* resetFunc) (void) = 0;
/*
void resetFunc(){
  digitalWrite(resetPin, HIGH);
}
//*/

void setup() {
  pinMode(measurementLed, OUTPUT);
  digitalWrite(measurementLed, HIGH);
  while(count <= 100){
    delay(216);
    count++;
  }
  digitalWrite(measurementLed, LOW);
}

void loop() {
}
