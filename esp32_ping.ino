#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define LED 2

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* serverAddress = "https://<supabase-project-id>.supabase.co/rest/v1/<location>_logs";
const char* apikey = "xxx";
const char* location = "your_Location";
const char* errorReportingAddress = "https://<supabase-project-id>.supabase.co/rest/v1/<location>_errors";

volatile boolean isError = false;
int errorCount = 0;
const int maxErrorCount = 5;
String errorMessages[maxErrorCount];

const int delayTime = 300000; // 5 minutes
const int onErrorDelayTime = 60000; // 1 minute

HTTPClient http;
boolean enableErrorReporting = true; // Set to false to disable error reporting

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(4000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, HIGH);
    Serial.println("LED is on");
    delay(500);
    Serial.println("Connecting...");
  }

  Serial.print("Connected to the WiFi network with IP: ");
  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  Serial.println(WiFi.localIP());
}

void logError(String errorMessage) {
  if (errorCount < maxErrorCount) {
    // Add a timestamp to the error message
    unsigned long timestamp = millis();
    String timestampStr = String(timestamp);
    errorMessages[errorCount] = timestampStr + ": " + errorMessage;
    errorCount++;
  }
}

void reportErrors() {
  if (enableErrorReporting && errorCount > 0) {
    http.begin(errorReportingAddress);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", apikey);

    DynamicJsonDocument jsonDocument(1024);
    jsonDocument["source"] = location;

    JsonArray errorsArray = jsonDocument.createNestedArray("errors");

    for (int i = 0; i < errorCount; i++) {
      JsonObject errorObject = errorsArray.createNestedObject();
      
      // Extract timestamp and message from the error message
      int timestampEnd = errorMessages[i].indexOf(": ");
      String timestamp = errorMessages[i].substring(0, timestampEnd);
      String message = errorMessages[i].substring(timestampEnd + 2);

      errorObject["timestamp"] = timestamp;
      errorObject["message"] = message;
    }

    String errorData;
    serializeJson(jsonDocument, errorData);

    int httpResponseCode = http.POST(errorData);
    if (httpResponseCode == 201) {
      Serial.println("Errors reported successfully");
      errorCount = 0;
    } else {
      Serial.println("Error reporting errors");
    }

    http.end();
  }
}


void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(serverAddress); // Use the server address for POST request
    http.addHeader("apikey", apikey); // Authorization 
    http.addHeader("Content-Type", "application/json"); // Specify the content type

    // Create a JSON document for the POST data
    DynamicJsonDocument postData(1024);
    postData["status"] = "ONLINE";
    postData["source"] = location;

    String postDataStr;
    serializeJson(postData, postDataStr);

    int httpResponseCode = http.POST(postDataStr);
    if (httpResponseCode == 201) { // Check for the success response code (201 Created)
      digitalWrite(LED, LOW);
      Serial.println("LED is off");
      delay(200);

      digitalWrite(LED, HIGH);
      Serial.println("LED is on");
      delay(500);

      digitalWrite(LED, LOW);
      Serial.println("LED is off");
      delay(500);

      Serial.println(httpResponseCode); // Print return code
    } else {
      isError = true;
      Serial.print("Error on sending POST request: ");
      digitalWrite(LED, HIGH);
      Serial.println("LED is on");
      Serial.println(httpResponseCode);
      logError("Failed to send POST request. HTTP response code: " + String(httpResponseCode));
    }

    http.end(); // Free resources
  } else {
    isError = true;
    Serial.print("Error in WiFi connection ");
    digitalWrite(LED, HIGH);
    Serial.println("LED is on");
    logError("Error in WiFi connection");
  }

  if (isError) {
    delay(onErrorDelayTime);
    isError = false;
  } else {
    delay(delayTime);
  }

  reportErrors();
}
