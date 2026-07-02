//#include <SPI.h>
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

char txpacket[101];
String msg;

void (* resetFunc) (void) = 0;
/*
void resetFunc(){
  digitalWrite(resetPin, HIGH);
}
//*/

void setup() {
  //Serial Setup
  Serial.begin(9600);
  delay(1000);
  //while (!Serial);
  //End Serial Setup

  //SPI.begin(SCK,MISO,MOSI,csPin);
  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  //Set LoRa Params
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(5);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setPreambleLength(8);
  LoRa.setTxPower(TXPOWER, PA_OUTPUT_PA_BOOST_PIN);

  pinMode(measurementLed, OUTPUT);
  for(int i=0; i<100; i++){
      txpacket[i] = (char) random(65,90);
    }
    txpacket[100] = "\0";
    msg = String(txpacket).substring(0,100);
}

void loop() {
  digitalWrite(measurementLed, HIGH);
  sendPacket(msg);
  Serial.println(msg);
  digitalWrite(measurementLed, LOW);
  delay(period);
  resetFunc();
}

void sendPacket(String out){
  LoRa.beginPacket();
  LoRa.print(out);
  LoRa.endPacket();
}
