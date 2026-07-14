#include "thermistor.h"         //Biblioteca para uso de Thermistors (Sensor de Temperatura)
#include "GravityTDS.h"         //Biblioteca para o uso do TDS
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

//Thermistor Variables
#define NTC_PIN A2 // Analog pin used to read the NTC

// Thermistor object
THERMISTOR thermistor(NTC_PIN,        // Analog pin
                      10000,          // Nominal resistance at 25 ºC
                      3950,           // thermistor's beta coefficient
                      10000);         // Value of the series resistor
float temp; //Variável para armazenar o valor da temperatura

//End Thermistor Variables


//PH Variables
#define PH_PIN A0
float valor_calibracao = 21.34; // Fator de calibração para o ajuste fino
//End PH Variables

//TDS Variables
#define TdsSensorPin A1 //Pino onde está o TDS
GravityTDS gravityTds;  //Variável utilizada pela biblioteca para realizar as medições
//End TDS Variables

int TXPOWER = 4;

int count_packets = 100;

bool lora_idle = true;

void OnTxDone( void );

void LoRaReset(void){
  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onTxDone(OnTxDone);

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
  //while (!Serial);
  //End Serial Setup

  //TDS Setup
  gravityTds.setPin(TdsSensorPin); //define o pino para medição
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  //End TDS Setup

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
  if(lora_idle && count_packets < 100){
    String msg = coleta();
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
  LoRa.endPacket();
  count_packets++;
	lora_idle = true;
}

String coleta(){
  String msg;
  msg += coleta_temperatura();
  msg += coleta_PH();
  msg += coleta_TDS();
  return msg; // has at least 23 chars
}

String coleta_temperatura(){
  temp = thermistor.read()/10; //Returns (int) temp*10;
  return String(temp) + "ºC;";
}

String coleta_PH(){
  float valor_pH = -5.70 * (analogRead(PH_PIN) * 5.0) / 1024.0; + valor_calibracao; //Calcula valor de pH
  return "pH "+ String(valor_pH) + ";";
}

String coleta_TDS(){
  gravityTds.setTemperature(temp);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  float tdsValue = gravityTds.getTdsValue();  // then get the value
  return String(tdsValue) + "ppm;";
}

/*
void OnTxDone( void ){
  Serial.println("Tx Done");
  LoRaReset();
  count_packets++;
	lora_idle = true;
}
//*/

