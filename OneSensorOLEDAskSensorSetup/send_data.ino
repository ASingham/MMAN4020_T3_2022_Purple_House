// Function to send data to AskSensor
void send_data(String apiKeyIn, float sens1C) {
  // Create a URL for updating module 1, module2 and module3 request
    String url = "http://api.asksensors.com/write/";
    url += apiKeyIn;
    url += "?module1=";
    url += sens1C;
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
}