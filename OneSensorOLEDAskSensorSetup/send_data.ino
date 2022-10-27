// Function to send data to AskSensor
void send_data(String apiKeyIn, float* temperatures) {
  // Create a URL for updating module 1, module2 and module3 request
    String url = "http://api.asksensors.com/write/";
    url += apiKeyIn + "?";

    // TODO: for loop to send sensor data
    for (int i=1; i < NUMBER_OF_SENSORS+1; i++) {
      url += "module" + String(i) + "=";
      url += temperatures[i-1];
      if (i < NUMBER_OF_SENSORS) url += "&";
    }

    Serial.print("Requesting URL: ");
    Serial.println(url);

    ask.begin(url); // Specify the URL
    // Check for the returning code
    int httpCode = ask.GET();
    if (httpCode > 0) {
      String payload = ask.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request");  
    }
    
    ask.end(); //End
}