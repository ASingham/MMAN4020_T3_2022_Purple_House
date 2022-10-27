#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define BATT_ICON_W 16
#define BATT_ICON_H 8
#define WIFI_ICON_W 16
#define WIFI_ICON_H 14
const float MAX_ANALOG_VAL = 4095.0;
const float MAX_BATTERY_VOLTAGE = 4.2; // Max LiPoly voltage of a 3.7 battery is 4.2

const unsigned char battIconEmpty [] = { 0xff, 0xfe, 0x80, 0x02, 0xa0, 0x03, 0xa0, 0x03, 0xa0, 0x03, 0xa0, 0x03, 0x80, 0x02, 0xff, 0xfe };
const unsigned char battIconFull [] = { 0xff, 0xfe, 0x80, 0x02, 0xbf, 0xfb, 0xbf, 0xfb, 0xbf, 0xfb, 0xbf, 0xfb, 0x80, 0x02, 0xff, 0xfe };
const unsigned char battIconHigh [] = { 0xff, 0xfe, 0x80, 0x02, 0xbf, 0xc3, 0xbf, 0xc3, 0xbf, 0xc3, 0xbf, 0xc3, 0x80, 0x02, 0xff, 0xfe };
const unsigned char battIconLow [] = { 0xff, 0xfe, 0x80, 0x02, 0xbc, 0x03, 0xbc, 0x03, 0xbc, 0x03, 0xbc, 0x03, 0x80, 0x02, 0xff, 0xfe };

const unsigned char wifiIconTrue [] = { 0x00, 0x00, 0x0f, 0xf0, 0x70, 0x0e, 0xc0, 0x03, 0x00, 0x00, 0x07, 0xe0, 0x18, 0x18, 0x00, 0x00,
                                        0x00, 0x00, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const unsigned char wifiIconFalse [] = { 0x30, 0x00, 0x19, 0xf0, 0x74, 0x0e, 0xc2, 0x03, 0x03, 0x00, 0x07, 0x80, 0x18, 0x48, 0x00, 0x20,
                                          0x00, 0x30, 0x03, 0xd8, 0x01, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Function that sets up the OLED Screen for use
void setup_oled()   {
  // Start OLED
  display.begin(0, true);

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("OLED setup complete.");
  display.display();
  delay(2000);
}

// Function that displays that the wifi connection was completed
void display_wifi_success(bool success) {
  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  if (success) display.println("done.");
  else display.println("failed.");
  display.display();
  delay(2000);
}

// Function that displays that the sensor connection was completed
void display_sensor_success(int devices) {
  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 40);

  String device_string = String(devices) + " sensor(s) setup.";
  if (devices != 0) {
    Serial.println("Sensor setup complete.\n" + device_string);
    display.println("Sensor setup complete\n"  + device_string);
  } else {
    Serial.println("Sensor setup failed.\n" + device_string);
    display.println("Sensor setup failed.\n" + device_string);    
  }
  
  display.display();
  delay(5000);
}

void display_wifi_icon(bool connected) {
  int x_pos = 90;
  int y_pos = 0;
  if (connected) {
    display.drawBitmap(x_pos, y_pos, wifiIconTrue, WIFI_ICON_W, WIFI_ICON_H, SH110X_WHITE);
  } else {
    display.drawBitmap(x_pos, y_pos, wifiIconFalse, WIFI_ICON_W, WIFI_ICON_H, SH110X_WHITE);
  }
}

void display_battery_icon(int battValue) {
  float voltageLevel = (battValue / MAX_ANALOG_VAL) * 2 * 1.1 * 3.3; // calculate voltage level
  float batteryFraction = voltageLevel / MAX_BATTERY_VOLTAGE;
  
  Serial.println((String)"Raw:" + battValue + " Voltage:" + voltageLevel + "V Percent: " + (batteryFraction * 100) + "%");

  int x_pos = 112;
  int y_pos = 2;

  if (batteryFraction >= 1.0) {           // Full reading
    display.drawBitmap(x_pos, y_pos, battIconFull, BATT_ICON_W, BATT_ICON_H, SH110X_WHITE);
  } else if (batteryFraction > 0.5) {     // High reading
    display.drawBitmap(x_pos, y_pos, battIconHigh, BATT_ICON_W, BATT_ICON_H, SH110X_WHITE);
  } else if (batteryFraction < 0.2) {     // Empty reading
    display.drawBitmap(x_pos, y_pos, battIconEmpty, BATT_ICON_W, BATT_ICON_H, SH110X_WHITE);
  } else {                                // Low reading
    display.drawBitmap(x_pos, y_pos, battIconLow, BATT_ICON_W, BATT_ICON_H, SH110X_WHITE);
  }
}

// Updates temperatures displayed on screen
void update_screen(float* temperatures, int battValue, bool wifiConnected) {
  // Clear the buffer.
  display.clearDisplay();

  display_wifi_icon(wifiConnected);
  display_battery_icon(battValue);

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 4);
  display.println("TEMP READINGS");
  
  for (int i=0; i < numberOfDevices; i++) {
    display.setCursor(0, 20 + i*10);
    display.print("Sensor " + String(i+1) + ":   ");
    display.print(String(temperatures[i]));
    display.print(" C");
  }

  display.display();
  delay(500);
}