#include <SPI.h>
#include <LoRa.h>

#define measurementLed 8

//LoRa Pins
#define csPin 10
#define resetPin 7
#define irqPin 2

//SPI Interface Pins
#define SCK 13
#define MISO 12
#define MOSI 11

#define period 750//60000 * 5 // Minutos de delay entre coleta

#define startBut 6

int TXPOWER = 4;

char txpacket[24];
String msg;

int count_packets = 100;

bool lora_idle = true;

void OnTxDone( void );

void LoRaReset(void){
  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.onTxDone(OnTxDone);

  //Set LoRa Params
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(5);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setPreambleLength(8);
  LoRa.setTxPower(TXPOWER, PA_OUTPUT_PA_BOOST_PIN);
}

void setup() {
  //Serial Setup
  Serial.begin(9600);
  delay(1000);
  //End Serial Setup

  //SPI.begin(SCK,MISO,MOSI,csPin);
  
  pinMode(measurementLed, OUTPUT);
  digitalWrite(measurementLed, LOW);
  pinMode(startBut, INPUT);
  Serial.println("\nAwainting button press for start...");
  while(!digitalRead(startBut)){
    delay(10);
  }
  Serial.println("Starting experiment.");
}

void loop() {
  if(lora_idle && count_packets < 100 && TXPOWER < 21){
    for(int i=0; i<23; i++){
      txpacket[i] = (char) random(65,90);
    }
    txpacket[23] = "\0";
    msg = String(txpacket).substring(0,24);

    lora_idle = false;
    sendPacket(msg);
    //Serial.println(msg);
  }
  if(count_packets >= 100){
    digitalWrite(measurementLed, LOW);
    delay(period);

    if(TXPOWER < 21 && lora_idle){
      count_packets = 0;
      TXPOWER++;
      LoRaReset();
      digitalWrite(measurementLed, HIGH);
    }
    if(TXPOWER == 21){
      Serial.println("Finished.");
      TXPOWER++;
    }
  }
}

void sendPacket(String out){
  LoRa.beginPacket();
  LoRa.print(out);
  LoRa.endPacket(true);
}

void OnTxDone( void ){
  //Serial.println("Tx Done");
  LoRaReset();
  count_packets++;
	lora_idle = true;
}
