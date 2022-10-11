
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
const char* ssid = "network name"; //Wifi SSID
const char* password = "password"; //Wifi Password
String apiKeyIn = "NOJqSJL3EhVkv1Dixm4rPzE5xngy5mwM"; // API Key
const unsigned int writeInterval = 25000; // write interval (in ms)
// ASKSENSORS host config
const char* host = "api.asksensors.com";
const int httpPort = 80; // port

// Data wire is connected to GPIO4
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


//  Device addresses found previously using file GenerateTempSensorAddress and manually reading the serial monitor output and then writing addresses below.
// This process should be autonomised with code but hasn't been yet.
DeviceAddress sensor1 = { 0x28, 0xDA, 0x59, 0x95, 0xF0, 0x1, 0x3C, 0x47 };
DeviceAddress sensor2 = { 0x28, 0xF, 0x99, 0x95, 0xF0, 0x1, 0x3C, 0x4A };
DeviceAddress sensor3= { 0x28, 0x9F, 0x5D, 0x95, 0xF0, 0x1, 0x3C, 0xC6 };


void setup(){
// open serial
Serial.begin(115200);
Serial.println("*****************************************************");
Serial.println("********** Program Start : Connect ESP32 to AskSensors.");
Serial.println("Wait for WiFi... ");
sensors.begin();

//Init WiFi as Station, start SmartConfig
WiFi.mode(WIFI_AP_STA);
WiFi.beginSmartConfig();

//Wait for SmartConfig packet from mobile
Serial.println("Waiting for SmartConfig.");
while (!WiFi.smartConfigDone()) {
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.println("SmartConfig received.");

//Wait for WiFi to connect to AP
Serial.println("Waiting for WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("WiFi Connected.");

Serial.print("IP Address: ");
Serial.println(WiFi.localIP());

}


void loop(){
// Use WiFiClient class to create TCP connections
WiFiClient client;
if (!client.connect(host, httpPort)) {
Serial.print(".");
return;
}else {

Serial.print("Requesting temperatures...");
sensors.requestTemperatures(); // Send the command to get temperatures
Serial.println("DONE");

Serial.print("Sensor 1(*C): ");
float sens1C = sensors.getTempC(sensor1);
Serial.print(sens1C); 
Serial.print(" Sensor 1(*F): ");
float sens1F = sensors.getTempF(sensor1);
Serial.println(sens1F); 

Serial.print("Sensor 2(*C): ");
float sens2C = sensors.getTempC(sensor2);
Serial.print(sens2C); 
Serial.print(" Sensor 2(*F): ");
float sens2F = sensors.getTempF(sensor2);
Serial.println(sens2F);

Serial.print("Sensor 3(*C): ");
float sens3C = sensors.getTempC(sensor3);
Serial.print(sens3C); 
Serial.print(" Sensor 3(*F): ");
float sens3F = sensors.getTempF(sensor3);
Serial.println(sens3F);

// Create a URL for updating module 1, module2 and module3 request
String url = "http://api.asksensors.com/write/";
url += apiKeyIn;
url += "?module1=";
url += sens1C;
url += "&module2=";
url += sens2C;
url += "&module3=";
url += sens3C;
Serial.print("********** requesting URL: ");
Serial.println(url);

ask.begin(url); //Specify the URL
//Check for the returning code
int httpCode = ask.GET();
if (httpCode > 0) {
String payload = ask.getString();
Serial.println(httpCode);
Serial.println(payload);
} else {
Serial.println("Error on HTTP request");
}
ask.end(); //End
Serial.println("********** End ");
Serial.println("*****************************************************");
}
client.stop(); // stop client
delay(writeInterval); // delay
}