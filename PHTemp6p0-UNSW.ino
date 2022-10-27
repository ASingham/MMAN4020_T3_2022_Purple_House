/*
  Arduino-based temp sample system for PurpleHouse
  PHTemp Ver. 5p2 - "Universal"

*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NTPClient.h>
#include <ESP8266HTTPClient.h>

// # include <WiFiUdp.h>

// # defines....
#define ONE_WIRE_BUS D4

// Globals...
float t1 = 0;               // Temp on OneWire address [0].
float t1raw = 0;
float t1offset = 0;
float t2 = 0;               // Temp on OneWire address [0].
float t2raw = 0;
float t2offset = 0;

// Time and day constants
long unsigned int utcOffsetInSeconds = 0;   // 9.5 * 60 * 60; //Darwin timezone
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
long unsigned int timestamp0;             // Epoch time
int sampleperiodmin = 10;   // sample period in minutes
int sampleperiodsec = sampleperiodmin * 60;     // sampleperiod in seconds
int sampleperiodmsec = sampleperiodsec * 1000;  // sampleperiod in msec

bool autoConnect = 0;

// address dallas
int deviceCount = 0;
DeviceAddress deviceaddr;

// Addresses of DS18B20s

const uint8_t sensorArray[16][8] = {
  {0x28, 0xC8, 0x00, 0x00, 0xE3, 0x04, 0x00, 0x38},
  {0x28, 0x64, 0x00, 0x00, 0x1C, 0x0B, 0x00, 0xD3},
  {0x28, 0xFF, 0xD0, 0xA8, 0x51, 0x17, 0x04, 0x90},
  {0x28, 0xAF, 0x00, 0x00, 0xB3, 0x4F, 0x00, 0xBD},

  {0x28, 0xAF, 0x00, 0x00, 0x6F, 0x51, 0x00, 0x2F},
  {0x28, 0xAF, 0x00, 0x00, 0x49, 0x3B, 0x00, 0xD7},
  {0x28, 0x4B, 0x00, 0x00, 0xAD, 0xBB, 0x00, 0x54},
  {0x28, 0x4B, 0x00, 0x00, 0x8A, 0xB4, 0x00, 0xA2},

  {0x7D, 0x6F, 0x00, 0x00, 0x00, 0x92, 0x5D, 0xEE},
  {0x28, 0xAF, 0x00, 0x00, 0x2D, 0x59, 0x03, 0x27},
  {0x28, 0x2F, 0x00, 0x00, 0xED, 0xC3, 0x00, 0x1D},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

const float offsetArray[16] = {
  0.0,
  0.8,
  0.0,
  3.0,

  58.0,
  0.0,
  0.0,
  -4.0,

  0.0,
  2.0,
  -1.0,
  0.0,

  0.0,
  0.0,
  0.0,
  0.0
};

// HTTPS constants:
const char* apiKeyIn = "cEHowoUZaM9WpFcwtsd9lGvd3FIN492f";      // API KEY IN
String host = "http://api.asksensors.com";

// Pin assignments for non-standard Arduino D1 mini
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t RX   = 3;
static const uint8_t TX   = 1;

// ESP8266WebServer server(80);    // Webserver setup
OneWire oneWire(ONE_WIRE_BUS);  // One Wire setup...
DallasTemperature sensors(&oneWire);
WiFiUDP ntpUDP;   // Define NTP Client to get time
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Functions...

void setup(void) {

  // Serial output setup
  Serial.begin(115200);
  delay(1000);
  Serial.println("Serial Port up OK.");

  // Hardware pins re-setup
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.println("Hardware pins set OK.");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // WiFi setup
  WiFi.setAutoConnect(autoConnect);
  WiFi.mode(WIFI_STA);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Already connected to network ");
    Serial.print(WiFi.SSID());
    Serial.print(" with status ");
    Serial.println(WiFi.status());
  }

  // Wait for connection UNSW PUT YOUR WIFI SSID and PASSWORDS IN HERE
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    if (WiFi.status() != WL_CONNECTED) {
      char* ssid =   "SSID1";
      char* password = "password1";
      wifiFive(ssid, password);
    }
    if (WiFi.status() != WL_CONNECTED) {
      char* ssid =   "SSID2";
      char* password = "password2";
      wifiFive(ssid, password);
    }
  }

  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("WiFi Setup OK.");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // OneWire setup
  sensors.begin();
  Serial.println("OneWire setup OK.");

  // Sensor setup
  deviceCount = sensors.getDeviceCount();
  Serial.print("Device Count: ");
  Serial.println(deviceCount);
  Serial.println("Sensors up OK.");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // Device addressing setup
  Serial.println("Organising addresses...");
  float deviceOffset = 0;

  for (int i = 0;  i < deviceCount;  i++) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(" : ");
    sensors.getAddress(deviceaddr, i);
    printAddress(deviceaddr);
    deviceOffset = findDeviceOffset(deviceaddr);
    Serial.println("Device temp offset is " + String(deviceOffset));
  }
  Serial.println("Device addressing OK.");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // Time client setup
  timeClient.begin();
  Serial.println("Time Client setup OK.");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // ALL SETUPS DONE
  Serial.println("Setups done.");
  for (int i = 1; i <= 5; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
}
void loop(void) {

  // Notify user...

  Serial.println("Awake!");
  pinMode(LED_BUILTIN, OUTPUT);     // Re-Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  // Time handling
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  Serial.print(", ");
  // Serial.println(timeClient.getFormattedTime());
  timestamp0 = timeClient.getEpochTime() - utcOffsetInSeconds;
  Serial.print("Epoch timestamp: " + String(timestamp0));
  Serial.print(", ");

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);

  // Sensor handling
  pinMode(LED_BUILTIN, OUTPUT);     // Re-Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);
  sensors.begin();
  delay(1000);
  deviceCount = sensors.getDeviceCount();
  Serial.print("Device Count: ");
  Serial.println(deviceCount);
  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures();
  pinMode(LED_BUILTIN, OUTPUT);     // Re-Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);

  // Show output for TWO sensors
  pinMode(LED_BUILTIN, OUTPUT);     // Re-Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Printing temperatures...");

  sensors.getAddress(deviceaddr, 0);
  printAddress(deviceaddr);
  t1raw = sensors.getTempC(deviceaddr);
  t1offset = findDeviceOffset(deviceaddr);
  t1 = t1raw + t1offset;
  Serial.println("Sensor 1 uncorrected: " + String(t1raw) + " degrees C");
  Serial.println("Sensor 1 corrected: " + String(t1) + " degrees C");

  sensors.getAddress(deviceaddr, 1);
  printAddress(deviceaddr);
  t2raw = sensors.getTempC(deviceaddr);
  t2offset = findDeviceOffset(deviceaddr);
  t2 = t2raw + t2offset;
  Serial.println("Sensor 2 uncorrected: " + String(t2raw) + " degrees C");
  Serial.println("Sensor 2 corrected: " + String(t2) + " degrees C");

  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);

  // HTTPS bit
  pinMode(LED_BUILTIN, OUTPUT);     // Re-Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);
  HTTPClient http;
  // Serial.print("[HTTP] begin...\n");

  // Create a URL for updating module 1 and module 2
  String url = "";
  url += host;
  url += "/write/";
  url += apiKeyIn;
  url += "?module1=";
  url += String(t1);
  url += "&t=";
  url += String(timestamp0);
  url += "000";
  url += "&module2=";
  url += String(t2);
  url += "&t=";
  url += String(timestamp0);
  url += "000";

  Serial.print("Requesting URL: ");
  Serial.println(url);
  http.begin(url); // HTTP

  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      // Serial.print("HTTP response: ");
      // Serial.println(payload);
      for (int i = 1; i <= 5; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(125);
        digitalWrite(LED_BUILTIN, LOW);
        delay(125);
      }
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    for (int i = 1; i <= 5; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
  }
  http.end();
  Serial.println("HTTPS bit done.");
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  // END HTTPS bit

  // Show user sleep state
  pinMode(LED_BUILTIN, OUTPUT);     // Re-Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("Sleeping");
  for (int i = 1; i < sampleperiodsec; i++) {    //short flash every five seconds
    if (i % 5 == 0) {
      Serial.print(".");
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(750);
    } else {
      delay(1000);
    }
  }
}
// END MAIN LOOP

// ODDBALL FUNCTIONS

float findDeviceOffset(DeviceAddress deviceAddress) {
  float deviceOffset = 0;
  bool notfound = 1;
  int rowCount = sizeof(sensorArray) / sizeof(sensorArray[0]);
  // Serial.println("Searching this many rows: " + String(rowCount));
  for (int r = 0; r < rowCount; r += 1) {
    // Serial.println("Searching row " + String(r));
    int par = 0;
    for (int i = 0; i < 8; i++) {
      // Serial.println("Comparing " + String(deviceAddress[i], HEX) + " and " + String(sensorArray[r][i], HEX) + "...");
      if (deviceAddress[i] == sensorArray[r][i]) {
        par++;
      } else {
        par = 0;
      }
    }
    if (par == 8) {
      Serial.println("Device address found at row" + String(r));
      notfound = 0;
      deviceOffset = offsetArray[r];
    }
  }

  if (notfound) {
    deviceOffset = 0;
    Serial.println("Device temp offset is unknown, set to zero.");
  }
  return deviceOffset;
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) {
      Serial.print(", ");
    }
  }
  Serial.println("");
}

void wifiFive(char* ssid, char* password) {
  WiFi.begin(ssid, password);
  delay (1000);
  Serial.print("Trying to connect to ");
  Serial.print(ssid);

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print(".");
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
  Serial.println("");
}
