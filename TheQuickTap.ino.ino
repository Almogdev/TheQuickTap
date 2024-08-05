#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int buttonPin = D1;  // הכפתור מחובר לפין D1
const int ledPin = D2;     // נורה מחוברת לפין D2
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverUrl = "http://api.kits4.me/GEN/api.php";

unsigned long pressStartTime;
unsigned long pressDuration;
bool buttonPressed = false;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH && !buttonPressed) {
    pressStartTime = millis();
    buttonPressed = true;
  }
  
  if (buttonState == LOW && buttonPressed) {
    pressDuration = millis() - pressStartTime;
    buttonPressed = false;
    sendDurationToServer(pressDuration);
  }
}
