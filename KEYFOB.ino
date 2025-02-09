#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SLAVE_ADDRESS 0x08
//#define key 14
#define ecu 27
//#define kbr 26
#define kbg 33 //FIRST
#define kbsr 32
#define kbsg 26//STATUS

#define WIFI_SSID "SSID" // Replace with your Wi-Fi SSID
#define WIFI_PASSWORD "PASSWORD"                 // Replace with your Wi-Fi password
// Firebase credentials
#define FIREBASE_API_KEY "API_KEY" // Replace with your Firebase API Key
#define FIREBASE_PROJECT_URL "DATABASE_URL" // Replace with your Firebase database URL

//bool keystat = true;  // the number of the pushbutton pin
int key;

void setup() {
       //pinMode(key, INPUT);
       pinMode(ecu, OUTPUT);
     //  pinMode(kbr, OUTPUT);
       pinMode(kbg, OUTPUT);
       pinMode(kbsr, OUTPUT);
       pinMode(kbsg, OUTPUT);

  Serial.begin(115200);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent); 
  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  delay(1000);
}

void loop() {
       
  delay(100);
}

void receiveEvent(int bytes) {
  digitalWrite(kbsg,LOW);
  digitalWrite(kbg,LOW);  
  String receivedData = "";
  digitalWrite(kbg,HIGH);
    while (Wire.available()) {
    char c = Wire.read();
    receivedData += c;
  }

  if (receivedData.length() > 0) {
    //digitalWrite(kbsg,LOW);
      Serial.println(receivedData);
    Serial.println("Uploading car data to Firebase...");
    Serial.println("Wait for 5 seconds");
    if(receivedData=="1C FA 7E 0"){
      uploadToFirebase("1");
    }
    else if(receivedData=="CD 54 4 2"){
      uploadToFirebase("2");
    }
    else if(receivedData=="3 76 6B 14"){
      uploadToFirebase("3");
    }
    else if(receivedData=="27 9 7E 0"){
      uploadToFirebase("4");
    }
     else if(receivedData=="9A BE 7D 0"){
      uploadToFirebase("5");
    }
    delay(100);
  }
}

void uploadToFirebase(String data) {
  key=1;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Convert the Firebase URL to String and concatenate the endpoint
    String path = String(FIREBASE_PROJECT_URL) + "/KeyFob.json?auth="+FIREBASE_API_KEY;
    http.begin(path);  // Initialize the HTTPClient with the URL
    http.addHeader("Content-Type","application/json");

    // JSON payload
    String payload = "\"" + data + "\"";

    int httpResponseCode = http.PUT(payload);

    if (httpResponseCode > 0) {
      Serial.println("Car Data uploaded successfully.");
    } else {
      Serial.println("Error uploading data: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("Wi-Fi not connected. Cannot upload data.");
}
if(key==1){
    digitalWrite(kbg,HIGH);
for(int i=0;i<10;i++){
  digitalWrite(kbsg,LOW);
  digitalWrite(kbsr,HIGH);
  delay(500);
  digitalWrite(kbsr,LOW);
  delay(500);
}
digitalWrite(kbsg,HIGH);
digitalWrite(ecu,LOW);
Serial.println("ecu low");
delay(2000);
digitalWrite(ecu,HIGH);
}
else{
  digitalWrite(kbg,LOW);
  digitalWrite(kbsg,LOW);
}
key=0;
}
