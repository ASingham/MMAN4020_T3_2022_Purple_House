#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// Function that sets up the OLED Screen for use
void setup_oled()   {
  // Start OLED
  display.begin(0, true); // we dont use the i2c address but we will reset!

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
void display_wifi_success() {
  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 20);
  display.println("WiFi setup complete.");
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
    Serial.print("Sensor setup complete.\n" + device_string);
    display.println("Sensor setup complete\n"  + device_string);
  } else {
    Serial.print("Sensor setup failed.\n" + device_string);
    display.println("Sensor setup failed.\n" + device_string);    
  }
  
  display.display();
  delay(2000);
}

// Function that updates the screen with
void update_temperature(float sens1C, float sens2C, float sens3C, float sens4C) {
  // Clear the buffer.
  display.clearDisplay();
  
  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Sensor 1 (*C): \n");
  
  display.setTextSize(3);
  display.println(String(sens1C));     
  display.display();
  delay(500);
}