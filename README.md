# Energy Monitoring Tests

This repository contain code and logs for our *Water Quality Monitoring Prototype*.

It is organized as follows:

- *ina219_Measurements*: This folder contains code for an Arduino Uno with an INA219 voltage and current sensor. All tests use this code for measuring. This code expects an Input in port 7, while this input is on HIGH it will do all measurements (Bus Voltage, Power, Current) and print to the screen.

- *read_serial.py*: This file contains a handy python script to read the serial output of the *ina219_Measurements* and save to log files.

- *ESP_V3_LoRa_Sender*: This code is for an ESP32 (Heltec WiFi LoRa 32 V3). An Output in port seven connects to the arduino running the *ina219_Measurements* code and the power source cable goes through the INA219 sensor. This code send 100 LoRa Packets with Spread Factor 12 (To maximize energy consumption) and variable Transmission Power (txpower, varies between 5 and 20).

- *ESP_V3_LoRa_Idle*: Same setup from *ESP_V3_LoRa_Sender*, but this code only counts to a 100, with 177 ms delays. The objective of this code is to measure ESP32 energy consumption without LoRa transmission, while generating the same number of reports for the measurements code.

- *logs*: Logs generatad by *read_serial.py*.

- *data_analysis*: Notebook containing code for making graphs and visualizing data.
