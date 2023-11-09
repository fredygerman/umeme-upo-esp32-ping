#include <WiFi.h>
#include <HTTPClient.h>

#define LED 2

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* serverAddress = "your_server_address";
const char* authorizationHeader = "Your_Authorization_Header";
const char* locationHeader = "Your_Location_Header";
const char* errorReportingAddress = "your_error_reporting_address";

volatile boolean isError = false;
int errorCount = 0;
const int maxErrorCount = 50;
String errorMessages[maxErrorCount];

const int delayTime = 300000; // 5 minutes
const int onErrorDelayTime = 60000; // 1 minute

HTTPClient http;
boolean enableErrorReporting = false; // Set to false to disable error reporting

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
    errorMessages[errorCount] = errorMessage;
    errorCount++;
  }
}

void reportErrors() {
  if (enableErrorReporting && errorCount > 0) {
    http.begin(errorReportingAddress); // Use the error reporting address
    http.addHeader("Location", locationHeader);
    http.addHeader("Authorization", authorizationHeader);

    String errorData = "";
    for (int i = 0; i < errorCount; i++) {
      errorData += "Error " + String(i + 1) + ": " + errorMessages[i] + "\n";
    }

    int httpResponseCode = http.POST(errorData);
    if (httpResponseCode == 200) {
      Serial.println("Errors reported successfully");
      errorCount = 0;
    } else {
      Serial.println("Error reporting errors");
    }

    http.end(); // Free resources
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(serverAddress); // Use the server address for GET request
    http.addHeader("Location", "Makumbusho");
    http.addHeader("Authorization", authorizationHeader);

    int httpResponseCode = http.GET();
    if (httpResponseCode == 204) {
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
      Serial.print("Error on sending GET request: ");
      digitalWrite(LED, HIGH);
      Serial.println("LED is on");
      Serial.println(httpResponseCode);
      logError("Failed to send GET request. HTTP response code: " + String(httpResponseCode));
    }

    http.end(); // Free resources
  } else {
    digitalWrite(LED, HIGH);
    Serial.println("LED is on");
    Serial.println("Error in WiFi connection");
  }

  if (isError) {
    delay(onErrorDelayTime);
    isError = false;
  } else {
    delay(delayTime);
  }

  reportErrors();
}
