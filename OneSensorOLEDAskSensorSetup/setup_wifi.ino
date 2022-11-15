// Function to establish WiFi Connection
bool setup_wifi(const char* ssid, const char* password) {
  Serial.println("*****************************************************");
  Serial.println("Connecting ESP32 to AskSensors...");

  // connecting to the WiFi network
  
  WiFiMulti.addAP(ssid, password);
  int i = 0;
  bool success = true;
  while (WiFiMulti.run() != WL_CONNECTED) {
    if (i == i) {
      success = false;
      break;
    }
    Serial.println("Waiting for WiFi...");
    delay(500);
    i++;
  }
  
  if (success) {
    // connected
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    // could not connect
    Serial.println("Could not connect to WiFi.");
    
  }

  return success;
}