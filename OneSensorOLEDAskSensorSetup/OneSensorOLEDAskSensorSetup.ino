
/*
Connect ESP32 to AskSensors
* Description: This sketch connects to a website (https://asksensors.com) using an ESP32 Wifi module.
* Author: https://asksensors.com, 2018
* github: https://github.com/asksensors
*/
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

WiFiMulti WiFiMulti;
HTTPClient ask;
// TODO: ESP32 user config
const char* ssid = "NotThisOne"; //Wifi SSID
const char* password = "CantFollowInstructions?"; //Wifi Password
String apiKeyIn = "NOJqSJL3EhVkv1Dixm4rPzE5xngy5mwM"; // API Key
const unsigned int writeInterval = 25000; // write interval (in ms)
// ASKSENSORS host config
const char* host = "api.asksensors.com";
const int httpPort = 80; // port

// Data wire is connected to GPIO4
#define ONE_WIRE_BUS 4
#define NUMBER_OF_SENSORS 4
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


//  Device addresses found previously using file GenerateTempSensorAddress and manually reading the serial monitor output and then writing addresses below.
// This process should be autonomised with code but hasn't been yet.
DeviceAddress sensor1 = { 0x28, 0xDA, 0x59, 0x95, 0xF0, 0x1, 0x3C, 0x47 };

void setup(){
  // open serial
  Serial.begin(115200);
  sensors.begin();

  setup_wifi(ssid, password);
  setup_oled();
  
  if (!get_sensor_address()) {
    Serial.print("Sensor Check failed");  
  }

  

}

void loop(){

  // // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, httpPort)) {
    Serial.print(".");
    return;
  } else {

    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("DONE");

    Serial.print("Sensor 1(*C): ");
    float sens1C = sensors.getTempC(sensor1);
    Serial.print(sens1C); 

    send_data(apiKeyIn, sens1C); 
       
    Serial.println("********** End ");
    Serial.println("*****************************************************");
    
  }
  
  client.stop(); // stop client
  delay(writeInterval); // delay
}