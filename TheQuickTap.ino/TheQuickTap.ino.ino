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
  if (digitalRead(performanceModePin) == LOW) {
    server.handleClient();
  } else {
    int buttonState = digitalRead(buttonPin);
  
    if (buttonState == HIGH && !buttonPressed) {
      pressStartTime = millis();
      buttonPressed = true;
    }
  
    if (buttonState == LOW && buttonPressed) {
      pressDuration = millis() - pressStartTime;
      buttonPressed = false;
      recordPressDuration(pressDuration);
      sendDurationToServer(pressDuration);
    }
  }
}

void sendDurationToServer(unsigned long duration) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(serverUrl) + "?duration=" + duration;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      if (payload == "shorter") {
        digitalWrite(ledPin, LOW); 
        analogWrite(ledPin, 32); 
      } else {
        digitalWrite(ledPin, HIGH);
        analogWrite(ledPin, 255);
      }
    } else {
      Serial.println("Error in HTTP request");
    }
    http.end();
  }
}

void setLEDColor(int r, int g, int b) {
  analogWrite(ledPin, r);
}

void setupServer() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Arduino_Performance");
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}