#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

// Replace with your network credentials
const char* ssid = "Your SSID";
const char* password = "Your WiFi Password";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Input Parameters
const char* PARAM_INPUT_OUTPUT = "output";
const char* PARAM_INPUT_STATE = "state";

// Set number of outputs
#define NUM_OUTPUTS  1

// Set ESP32 GPIO 26 outputs for motor control
int outputGPIOs[NUM_OUTPUTS] = {26};

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Return JSON with Current Output States, it's for multiple O/Ps, but here only GPIO 26 is used.
String getOutputStates() {
  JSONVar myArray;
  for (int i =0; i<NUM_OUTPUTS; i++) {
    myArray["gpios"][i]["output"] = String(outputGPIOs[i]);
    myArray["gpios"][i]["state"] = String(digitalRead(outputGPIOs[i]));
    //myArray["gpios"][i] ="{\"output\":\"" + String(outputGPIOs[i]) + "\", \"state\": \"" + String(digitalRead(outputGPIOs[i])) + "\"}";
  }
  String jsonString = JSON.stringify(myArray);
  Serial.print(jsonString);
  return jsonString;
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Set GPIOs as outputs
  for (int i =0; i<NUM_OUTPUTS; i++){
    pinMode(outputGPIOs[i], OUTPUT);
  }

  initWiFi();
  initSPIFFS();
    
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html",false);
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/states", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getOutputStates();
    request->send(200, "application/json", json);
    json = String();
  });

  //GET request to <ESP_IP>/update?output=<output>&state=<state>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String output;
    String state;
    // GET input1 value on <ESP_IP>/update?output=<output>&state=<state>
    if (request->hasParam(PARAM_INPUT_OUTPUT) && request->hasParam(PARAM_INPUT_STATE)) {
      output = request->getParam(PARAM_INPUT_OUTPUT)->value();
      state = request->getParam(PARAM_INPUT_STATE)->value();
      // Control GPIO
      digitalWrite(output.toInt(), state.toInt());
    }
    else {
      output = "No message sent";
      state = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(output);
    Serial.print(" - Set to: ");
    Serial.println(state);
    
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop() {

}
