#include <WiFi.h>
#include <HTTPClient.h>
#define LED 2
 
const char* ssid = "cool_wifi";
const char* password =  "password";
 
void setup() {
 
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(4000);   
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to the WiFi network with IP: ");
  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  Serial.println(WiFi.localIP());
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;   
 
   http.begin("https://directus-instance.url/flows/trigger/flow-id");  //Specify destination for HTTP request
   //  http.addHeader("Content-Type", "text/plain");             //Specify content-type header if needed
   http.addHeader("Location", "Makumbusho");            
   http.addHeader("Authorization", "Token bla bla bla");  

      int httpResponseCode = http.GET();
   if(httpResponseCode==200){
    digitalWrite(LED, LOW);
    Serial.println("LED is off");
    delay(200);

    digitalWrite(LED, HIGH);
    Serial.println("LED is on");

    delay(500);
    digitalWrite(LED, LOW);
    Serial.println("LED is off");
    delay(500);
 
    Serial.println(httpResponseCode);   //Print return code
 
   }else{
 
    Serial.print("Error on sending request: ");
    digitalWrite(LED, HIGH);
    Serial.println("LED is on");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();  //Free resources
 
 }else{
    digitalWrite(LED, HIGH);
    Serial.println("LED is on");
    Serial.println("Error in WiFi connection");   

 }
 
  delay(300000);  //Send a request every 5 Minute
 
}
