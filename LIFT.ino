const int lred = 13;  
const int lgreen = 12;  
const int sred = 26;  
const int sgreen = 25;  
const int trigpin = 14;  
const int fobpin = 4;  
const int nanopin = 16;    

unsigned long startTime = millis(); // Record the start time
bool inputDetected = false;         // Flag to track if input becomes zero
void setup() {  
  Serial.begin(115200);
  pinMode(lred, OUTPUT);  
  pinMode(lgreen, OUTPUT);  
  pinMode(sred, OUTPUT);  
  pinMode(sgreen, OUTPUT);  
  pinMode(trigpin, INPUT);  
  pinMode(fobpin, OUTPUT);  
  pinMode(nanopin, OUTPUT); 
  pinMode(2,OUTPUT); 
  delay(1000);
}  

void loop() {
  delay(1000);  
    while (digitalRead(trigpin) == HIGH){
      digitalWrite(nanopin,HIGH);
            digitalWrite(fobpin,HIGH);
            Serial.println("HIGH");
                        
    }
    while (digitalRead(trigpin) == LOW)
    {
      blink();
    }


// Loop for up to 15 seconds
while (millis() - startTime < 15000) {
    if (digitalRead(trigpin) == LOW) { // Check if input becomes zero
       // blink();                        // Call the blink function
        inputDetected = true;           // Mark that input was detected
        break;                          // Exit the loop early
    }
}

// After the loop
if (inputDetected) {
    // Input became zero within 15 seconds
    blink();
} else {
    // Input did not become zero within 15 seconds
    digitalWrite(lred, LOW);   // Turn off the red LED
    digitalWrite(lgreen,HIGH);
}


    digitalWrite(nanopin,LOW);
    delay(2000);
    digitalWrite(nanopin,HIGH);
    for(int i=0;i<15;i++) {
        digitalWrite(sred,HIGH);
        delay(500);
        digitalWrite(sred,LOW);
        delay(500);
}
        digitalWrite(sgreen,HIGH);
    digitalWrite(fobpin,LOW);
    delay(2000);
    digitalWrite(fobpin,HIGH);
}  
void blink()
{
      while(digitalRead(trigpin) == LOW){
      digitalWrite(lgreen, LOW); 
      digitalWrite(sgreen, LOW); 
      digitalWrite(2, LOW);

      digitalWrite(lred, HIGH);  
      delay(500);  
      digitalWrite(lred, LOW); 
      delay(500); 
      }
}
