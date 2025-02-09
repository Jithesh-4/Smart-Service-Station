#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your network credentials
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

// Firebase Realtime Database URL and Authentication Key
#define FIREBASE_HOST "DATABASE_URL"
#define FIREBASE_AUTH "API_KEY"

// Define ADC pins for the LDRs
#define PIN_25 34
#define PIN_26 39
#define PIN_32 32
#define PIN_33 33
#define PIN_35 35
#define trig 15
#define nano 4 
#define sw 21
#define ecr 16
#define ecg 17
#define inr 5
#define ing 19
#define inspr 13// temp ports
#define inspg 12// temp ports 

bool trigstat = HIGH;  // the number of the pushbutton pin
bool swstat = HIGH;  // the number of the pushbutton pin
//bool image=HIGH;




void setup() {
  // Initialize serial communication
     pinMode(trig, INPUT);
        pinMode(sw, INPUT);
            pinMode(ecr, OUTPUT);
            pinMode(ecg, OUTPUT);
            pinMode(inr, OUTPUT);
            pinMode(ing, OUTPUT);
            pinMode(nano, OUTPUT);
            pinMode(inspr, OUTPUT);
            pinMode(inspg, OUTPUT);
digitalWrite(nano,HIGH);

  Serial.begin(115200); 
  delay(1000); // Allow time for Serial Monitor to start
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to Wi-Fi");
  delay(1000);
}

void loop() {
  delay(1000);
  trigstat= digitalRead(trig);
   while(trigstat==HIGH)
   {
       trigstat= digitalRead(trig);
       digitalWrite(nano,HIGH);
        }
       digitalWrite(ing,LOW);
       digitalWrite(inspg,LOW);
       digitalWrite(ecg,LOW);
for(int i=0;i<15;i++){
  digitalWrite(ecr,HIGH);
  delay(500);
  digitalWrite(ecr,LOW);
  delay(500);
}
  digitalWrite(ecg,HIGH);

  int pin25Value = analogRead(PIN_25);
  int pin26Value = analogRead(PIN_26);
  int pin32Value = analogRead(PIN_32);
  int pin33Value = analogRead(PIN_33);
  int pin35Value = analogRead(PIN_35);

  // Convert raw ADC values to percentages (0% to 100%)
  int pin25Percent = map(pin25Value, 0, 4095, 0, 100);
  int pin26Percent = map(pin26Value, 0, 4095, 0, 100);
  int pin32Percent = map(pin32Value, 0, 4095, 0, 100);//34
  int pin33Percent = map(pin33Value, 0, 4095, 0, 100);
  int pin35Percent = map(pin35Value, 0, 4095, 0, 100);

  // Print the raw values and percentages to the Serial Monitor
  Serial.print("COOLANT TEMPERATURE : "); Serial.print(pin25Percent); Serial.print("%");
  Serial.print(" | ENGINE LOAD: "); Serial.print(pin26Percent); Serial.print("%");
  Serial.print(" | BRAKE : "); Serial.print(pin32Percent); Serial.print("%");
  Serial.print(" | BATTERT LEVEL: "); Serial.print(pin33Percent); Serial.print("%");
  Serial.print(" | RPM : "); Serial.print(pin35Percent); Serial.println("%");


  // Construct JSON payload
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["Coolant Temperature"] = pin25Percent;
  jsonDoc["Engine Load"] = pin26Percent;
  jsonDoc["Brake"] = pin32Percent;
  jsonDoc["Battery Level"] = pin33Percent;
  jsonDoc["RPM"] = pin35Percent;
  jsonDoc["image"] = 1;

  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);

  // Send HTTP PATCH request to Firebase
  HTTPClient http;
  String firebaseURL = String(FIREBASE_HOST) + "/SensorData.json?auth=" + FIREBASE_AUTH;
  http.begin(firebaseURL.c_str());
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.PATCH(jsonBuffer);

  if (httpResponseCode > 0) {
    Serial.print("Data uploaded successfully. Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error uploading data. Response code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  // Add a small delay before reading again
  delay(500); // Adjust as needed
  if(pin32Percent>50)
  {
    swstat= digitalRead(sw);
    while(swstat==HIGH){
  digitalWrite(inspr,HIGH);
  delay(500);
  digitalWrite(inspr,LOW);
  delay(500);
  swstat= digitalRead(sw);
  }
   if(swstat==LOW){
    jsonDoc["image"] = 0;
    delay(1000);
  }

  }
    digitalWrite(inspg,HIGH);
    digitalWrite(nano,LOW);
    delay(3000);
        digitalWrite(nano,HIGH);


  for(int i=0;i<15;i++){
  digitalWrite(inr,HIGH);
  delay(500);
  digitalWrite(inr,LOW);
  delay(500);
}
  digitalWrite(ing,HIGH);
}
