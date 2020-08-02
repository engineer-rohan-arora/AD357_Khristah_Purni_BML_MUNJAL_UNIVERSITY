#define BLYNK_MAX_SENDBYTES 256
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "3C-UaXJChYEmrBi1gOlUWt5U9DaGorde";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "saxena family";
char pass[] = "chitransh@545";

const int sensor_pin = A0;  /* Connect Soil moisture analog sensor pin to A0 of NodeMCU */

void setup() {
  Serial.begin(9600); /* Define baud rate for serial communication */
  Blynk.begin(auth, ssid, pass);
}

void sendNotification(String message){

  Serial.println("- connecting to Home Router SID: " + String(ssid));
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("- succesfully connected");
  Serial.println("- starting client");
  
  WiFiClient client;
}

void loop() {
  int moisture_percentage;

  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

 Serial.print("Soil moisture = ");
 Serial.print(moisture_percentage );
 Serial.println("%");

 if(moisture_percentage >=45 && moisture_percentage <=80)
 {
  Blynk.notify("Be alert! Soil moisture level raise ..");
 }
  Blynk.run();
  delay(1000);
}
