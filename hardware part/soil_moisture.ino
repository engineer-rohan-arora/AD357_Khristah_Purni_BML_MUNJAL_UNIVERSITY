
#define BLYNK_MAX_SENDBYTES 256
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char auth[] = "3C-UaXJChYEmrBi1gOlUWt5U9DaGorde";

String apiKey = "EFOTEIN4HWLKMI5N"; 

//  WiFi credentials.

char ssid[] = "saxena family";
char pass[] = "chitransh@545";
const char* server = "api.thingspeak.com";

const int sensor_pin = A0;  

WiFiClient client;

void setup() {
  Serial.begin(9600);
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

 delay(1000);

  if (isnan(moisture_percentage)) 
                 {
                     Serial.println("Failed to read from soil moisture sensor");
                      return;
                 }

                         if (client.connect(server,80))   
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(moisture_percentage);
             
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Soil_Moisture: ");
                             Serial.print(moisture_percentage);
                             Serial.print(" % ");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  delay(10000);

   if(moisture_percentage >=1 && moisture_percentage <=40)
 {
  Blynk.notify("Don't Worry you are safe..");
 }
 else if (moisture_percentage >=40 && moisture_percentage <=80)
 {
  Blynk.notify("Be alert! Flood chances raise..");
 }
 else if (moisture_percentage >=80 && moisture_percentage <=140){
  Blynk.notify("Flood overflow");
 }
}
