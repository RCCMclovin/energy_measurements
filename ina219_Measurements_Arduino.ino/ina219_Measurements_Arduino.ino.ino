#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

float power = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;

int input = 7;
int test_count = 1;
unsigned long t = 0;

void setup(){

  Serial.begin(9600);
  while (!Serial) {
  // will pause Zero, Leonardo, etc until serial console opens
  delay(1);
  }
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V,2A). However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  Serial.println("Measuring voltage and current with INA219 ...");
  pinMode(input, INPUT);
  Serial.print("Start test:"); Serial.println(test_count);
  t = millis();
}

void loop(void){
  if(test_count < 101){
    int start_reading = digitalRead(input);
    if(start_reading){
      test_count++;
      while(start_reading){
        power = ina219.getPower_mW();
        busvoltage = ina219.getBusVoltage_V();
        current_mA = ina219.getCurrent_mA();
  
        if(current_mA > 0){
          Serial.print(busvoltage); Serial.print(" V,"); Serial.print(power); Serial.print(" mW,"); Serial.print(current_mA); Serial.println(" mA");
        }
        start_reading = digitalRead(input);
      }
    }
  }
  if(test_count == 101){
    Serial.print("Time: "); Serial.println(millis() - t);
    test_count++;
  }
  
}
