
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
#include <vector>

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

byte wireAddresses[4][8];

OneWire ds(4);  //data wire connected to GPIO 4

// Data wire is connected to GPIO4
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


//  Device addresses found previously using file GenerateTempSensorAddress and manually reading the serial monitor output and then writing addresses below.
// This process should be autonomised with code but hasn't been yet.


DeviceAddress sensor1 = { 0x28, 0x52, 0xC6, 0x95, 0xF0, 0x1, 0x3C, 0xD4 };
DeviceAddress sensor2 = { 0x28, 0xB2, 0xE4, 0x95, 0xF0, 0x1, 0x3C, 0x86 };
DeviceAddress sensor3 = { 0x28, 0xAA, 0xB3, 0x95, 0xF0, 0x1, 0x3C, 0xB8 };
DeviceAddress sensor4 = { 0x28, 0x2D, 0x9, 0x95, 0xF0, 0xFF, 0x3C, 0x60 };

void getAddress()
{
   int j = 0;
   byte i;
   byte addr[8];
     
   while(ds.search(addr)){
    Serial.print(" ROM =");
    for (i = 0; i < 8; i++) {
        Serial.write(' ');
        Serial.print(addr[i], HEX);
        wireAddresses[j][i] = addr[i];
    }
    j++;
   }
  
}


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
getAddress();

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
float sens1C = sensors.getTempC(wireAddresses[0]);
Serial.print(sens1C); 
Serial.print(" Sensor 1(*F): ");
float sens1F = sensors.getTempF(wireAddresses[0]);
Serial.println(sens1F); 

Serial.print("Sensor 2(*C): ");
float sens2C = sensors.getTempC(wireAddresses[1]);
Serial.print(sens2C); 
Serial.print(" Sensor 2(*F): ");
float sens2F = sensors.getTempF(wireAddresses[1]);
Serial.println(sens2F);

Serial.print("Sensor 3(*C): ");
float sens3C = sensors.getTempC(wireAddresses[2]);
Serial.print(sens3C); 
Serial.print(" Sensor 3(*F): ");
float sens3F = sensors.getTempF(wireAddresses[2]);
Serial.println(sens3F);

Serial.print("Sensor 4(*C): ");
float sens4C = sensors.getTempC(wireAddresses[3]);
Serial.print(sens3C); 
Serial.print(" Sensor 4(*F): ");
float sens4F = sensors.getTempF(wireAddresses[3]);
Serial.println(sens4F);
}


//// Create a URL for updating module 1, module2 and module3 request
//String url = "http://api.asksensors.com/write/";
//url += apiKeyIn;
//url += "?module1=";
//url += sens1C;
//url += "&module2=";
//url += sens2C;
//url += "&module3=";
//url += sens3C;
//Serial.print("********** requesting URL: ");
//Serial.println(url);
//
//ask.begin(url); //Specify the URL
////Check for the returning code
//int httpCode = ask.GET();
//if (httpCode > 0) {
//  String payload = ask.getString();
//  Serial.println(httpCode);
//  Serial.println(payload);
//} else {
//  Serial.println("Error on HTTP request");
//}
//  ask.end(); //End
//  Serial.println("********** End ");
//  Serial.println("*****************************************************");
//}
//  client.stop(); // stop client
//  delay(writeInterval); // delay
}
