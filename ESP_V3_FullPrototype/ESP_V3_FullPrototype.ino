#include "thermistor.h"         //Biblioteca para uso de Thermistors (Sensor de Temperatura)
#include <Adafruit_ADS1X15.h>   //Biblioteca para o uso do Analogic to Digital Serial (ADS 1115)
#include "GravityTDS.h"         //Biblioteca para o uso do TDS
#include "LoRaWan_APP.h"
#include "Arduino.h"

#define RF_FREQUENCY                                915000000 // Hz

int TX_OUTPUT_POWER=                                 4;        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       12         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 250 // Define the payload size here

#define measurementLed 7
#define startBut 0

String outgoing;
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

//unsigned long timeTaken;

int count_packets = 100;

bool lora_idle=true;

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );

//Thermistor Variables
#define NTC_PIN 6 // Analog pin used to read the NTC

// Thermistor object
THERMISTOR thermistor(NTC_PIN,        // Analog pin
                      10000,          // Nominal resistance at 25 ºC
                      3950,           // thermistor's beta coefficient
                      10000);         // Value of the series resistor
float temp; //Variável para armazenar o valor da temperatura

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
float valor_calibracao = 21.34; // Fator de calibração para o ajuste fino
#define TdsSensorPin 5 //Pino onde está o TDS
GravityTDS gravityTds;  //Variável utilizada pela biblioteca para realizar as medições


void setup() {
  Serial.begin(115200);
  Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);

  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;

  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  /*
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
  //*/
  
  if (!ads.begin()){
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  gravityTds.setPin(TdsSensorPin); //define o pino para medição
  gravityTds.setAref(3.3);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(4096);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  
  pinMode(measurementLed, OUTPUT);
  digitalWrite(measurementLed,LOW);
  pinMode(startBut, INPUT_PULLUP);
  Serial.println("\nAwainting button press for start...");
  while(digitalRead(startBut)){
    delay(10);
  }
  Serial.println("Starting experiment.");

}

void loop() {
  if(lora_idle == true && count_packets < 100 && TX_OUTPUT_POWER < 21)
	{
    outgoing = coleta();
    outgoing.getBytes((uint8_t *) txpacket, outgoing.length()+1);

		//Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));

    //timeTaken = millis();
		Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out
    lora_idle = false;
	}
  Radio.IrqProcess( );
  if(count_packets == 100){
    digitalWrite(measurementLed,LOW);
    delay(750);
    if(TX_OUTPUT_POWER < 21){
      TX_OUTPUT_POWER++;
      Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
      count_packets = 0;
      digitalWrite(measurementLed,HIGH);
    }
    if(TX_OUTPUT_POWER == 21){
      Serial.println("Finished");
      TX_OUTPUT_POWER++;
    }
  }
}

String coleta(){
  String msg;
  msg += coleta_temperatura();
  msg += coleta_PH();
  msg += coleta_TDS();
  //msg += coleta_NTU();
  return msg;
}

String coleta_temperatura(){
  temp = thermistor.read()/10;
  return String(temp) + "C;";   // Read temperature);
}

String coleta_PH(){
  float valor_pH = -5.70 * ads.computeVolts(ads.readADC_SingleEnded(0)) + valor_calibracao; //Calcula valor de pH
  return "pH "+ String(valor_pH) + ";";
}

String coleta_TDS(){
  gravityTds.setTemperature(temp);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  float tdsValue = gravityTds.getTdsValue();  // then get the value
  return String(tdsValue) + "ppm;";
}

String coleta_NTU(){
  float ntu = map(ads.computeVolts(ads.readADC_SingleEnded(1)),2.5, 4.1, 3000.0, 0.0);
  return String(ntu) + "ntu;";
}

float map(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void OnTxDone( void )
{
  //timeTaken = millis() - timeTaken;
	//Serial.println("TX done......");
  //Serial.print("Tempo total: ");
  //Serial.print(timeTaken);
  //Serial.println(" ms");
	lora_idle = true;
  count_packets++;
}

void OnTxTimeout( void )
{
  //timeTaken = millis() - timeTaken;
    Radio.Sleep( );
    //Serial.println("TX Timeout......");
    lora_idle = true;
    //Serial.print("Tempo total: ");
    //Serial.print(timeTaken);
    //Serial.println(" ms");
}