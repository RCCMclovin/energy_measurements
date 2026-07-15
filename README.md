# Energy Monitoring Tests

This repository contain code and logs for our *Water Quality Monitoring Prototype*.

It is organized as follows:

- *ESP_V3_LoRa_Sender*: This code is for an ESP32 (Heltec WiFi LoRa 32 V3). An Output in port seven connects to the arduino running the *ina219_Measurements* code and the power source cable goes through the INA219 sensor. This code sends 100 LoRa Packets with Spread Factor 12 (To maximize energy consumption) and variable Transmission Power (txpower, varies between 5 and 20) and 23 characters as does the FullPrototype.

- *ESP_V3_Idle*: Same setup from *ESP_V3_LoRa_Sender*, but this code only counts to a 100, with 1616 ms delays. The objective of this code is to measure ESP32 energy consumption without LoRa transmission, while generating the same number of reports for the measurements code. The delay time was chosen so there were at least the same number of entries as the log from *ESP_V3_FullPrototype* or *ESP_V3_LoRa_Sender* with most entries, so all values can be directly compared in a graph to the ESP doing nothing.

- *ESP_V3_FullPrototype*: This code includes the complete prototype used by us for water quality monitoring with ESP32 (Heltec WiFi LoRa 32 V3). This code include the sensors: "TDS Meter V1.0", "NTC 10K waterproof - MF58" and "PH-4502C". To guarantee effective measumerements from the PH sensor (which works with 5v logic) a ADS1115 is also included in this prototype to convert 5v logic to 3.3v. An Output in port seven connects to the arduino running the *ina219_Measurements* code and the power source cable goes through the INA219 sensor. This code sends 100 LoRa Packets with Spread Factor 12 (To maximize energy consumption) and variable Transmission Power (txpower, varies between 5 and 20). Packets contain only 23 characters.

- *Arduino_Idle*: Using the same logic from *ESP_V3_Idle*, this code counts to 100 with 216 ms delays.

- *Arduino_FullPrototype*: This code includes the complete prototype used by us for water quality monitoring with Arduino Uno. This code include the sensors: "TDS Meter V1.0", "NTC 10K waterproof - MF58" and "PH-4502C". The LoRa Transceiver used is the RFM96. An Output in port seven connects to the arduino running the *ina219_Measurements* code and the power source cable goes through the INA219 sensor. This code sends 100 LoRa Packets with Spread Factor 12 (To maximize energy consumption) and a fixed Transmission Power (txpower, may vary between 5 and 20). Packets contain only 23 characters.

- *Arduino_LoRa_Sender*: This code is for an Arduino Uno with an LoRa transceiver (RFM96 was used). An Output in port seven connects to the arduino running the *ina219_Measurements* code and the power source cable goes through the INA219 sensor. This code sends 100 LoRa Packets with Spread Factor 12 (To maximize energy consumption), a fixed Transmission Power (txpower, may vary between 5 and 20) and 23 characters as does the FullPrototype.

- *ina219_Measurements*: This folder contains code for an Arduino Uno with an INA219 voltage and current sensor. All tests use this code for measuring. This code expects an Input in port 7, while this input is on HIGH it will do all measurements (Bus Voltage, Power, Current) and print to the screen.

- *read_serial.py*: This file contains a handy python script to read the serial output of the *ina219_Measurements* and save to log files.

- *logs*: Logs generatad by *read_serial.py*.

- *data_analysis*: Notebook containing code for making graphs and visualizing data.
