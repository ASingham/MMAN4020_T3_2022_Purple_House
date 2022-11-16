
/*
MMAN4020_T3_2022_Purple_House_Final_Solution
* Description: This code manages the following functions
  - Accessing and reading data from connected temperature sensors
  - Displaying read in temperature data and current battery level on an OLED display
  - Connecting to WiFi and sending read in temperature data to a AskSensor (https://asksensors.com)
* Author: Adam Tan, Max Neville, Tommaso De Battista, Sachin Singham
* Github: https://github.com/ASingham/MMAN4020_T3_2022_Purple_House
*/

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SH110X.h>

WiFiMulti WiFiMulti;
HTTPClient ask;

// TODO: ESP32 user config
const char* ssid = "INPUT WiFi SSID"; // Wifi SSID
const char* password = "INPUT WiFi Password"; // Wifi Password

String apiKeyIn = "qxj0vIPw25K2R5K6onSJ84yS72apbMil"; // API Key
const unsigned long writeInterval = 25000; // write interval (in ms)
// ASKSENSORS host config
const char* host = "api.asksensors.com";
const int httpPort = 80; // port
bool wifiConnected;
unsigned long currentTime;
unsigned long startTime;
unsigned long lastUpdatedTime = 0;

// Data wire is connected to GPIO4
#define ONE_WIRE_BUS 4
#define NUMBER_OF_SENSORS 4

// Definitions for the OLED screen
#define OLED_MOSI     18
#define OLED_CLK      5
#define OLED_DC       16
#define OLED_CS       21
#define OLED_RST      17

// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

void setup() {
  Serial.println("======================================");
  // open serial
  Serial.begin(115200);
  sensors.begin();
  startTime = millis();

  // Setup OLED Screen
  Serial.println("Setting up OLED...");
  setup_oled();
  Serial.println("OLED setup complete.");

  // Setup WiFi connection
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 20);
  display.println("Setting up WiFi...");
  display.display();
  bool success = setup_wifi(ssid, password);
  if (success) {
    Serial.println("WiFi setup complete.");
  }
  display_wifi_success(success);
  
  // Setup sensors
  int devices = sensor_check();
  display_sensor_success(devices);

}

void loop() {
  currentTime = millis();
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  // Initialise temperature array
  float temperatures[4] = { -127.0, -127.0, -127.0, -127.0 };
  get_sensors_data(temperatures);

  // Print temperatures to serial monitor
  for (int i=0; i < NUMBER_OF_SENSORS; i++) {
    Serial.print("Sensor " + String(i+1) + ": ");
    Serial.print(temperatures[i]);
    Serial.println(" C");
  }

  // Update data on display
  int battValue = analogRead(A13);
  WiFiClient client; // Use WiFiClient class to create TCP connections
  bool wifiConnected = client.connect(host, httpPort);
  update_screen(temperatures, battValue, wifiConnected);

  // Send data to AskSensors
  if (wifiConnected && (currentTime - startTime >= writeInterval)) {
    send_data(apiKeyIn, temperatures); 
    startTime = currentTime;

  }  
  Serial.println("==============================");

  // Keep retrying to connect to WiFi
  if (!wifiConnected) {
    WiFiMulti.addAP(ssid, password);
    WiFiMulti.run();
  }
  
  client.stop(); // stop client
  // delay(writeInterval); // delay
}
