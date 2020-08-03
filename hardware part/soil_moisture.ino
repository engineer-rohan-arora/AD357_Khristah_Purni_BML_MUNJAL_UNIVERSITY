
//#define BLYNK_MAX_SENDBYTES 256
//#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
//#include <FirebaseArduino.h>
//#include <BlynkSimpleEsp8266.h>



////#define FIREBASE_HOST "sja-alumni.firebaseio.com"
///#define FIREBASE_AUTH "qdM8yON8Kqy6zfaHLpYVoiutURlJ4bg2nIrI2ZuT"

//char auth[] = "3C-UaXJChYEmrBi1gOlUWt5U9DaGorde";

String apiKey = "EFOTEIN4HWLKMI5N"; 

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "saxena family";
char pass[] = "chitransh@545";
const char* server = "api.thingspeak.com";

const int sensor_pin = A0;  /* Connect Soil moisture analog sensor pin to A0 of NodeMCU */

WiFiClient client;

void setup() {
  Serial.begin(9600); /* Define baud rate for serial communication */
  //Blynk.begin(auth, ssid, pass);
  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
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

// Firebase.pushString("Soil_Moisture", moisture_percentage);
 delay(1000);

  /*if (Firebase.failed()) 
    {
 
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }*/
  if (isnan(moisture_percentage)) 
                 {
                     Serial.println("Failed to read from soil moisture sensor");
                      return;
                 }

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
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
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);
}
