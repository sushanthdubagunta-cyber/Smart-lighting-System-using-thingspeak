#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials (Wokwi)
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak API Key
String apiKey = "YOUR_WRITE_API_KEY";

// Pins
const int ldrPin = 34;
const int ledPin = 2;

void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  // Read LDR value
  int ldrValue = analogRead(ldrPin);

  Serial.print("LDR Value = ");
  Serial.println(ldrValue);

  // LED ON when dark
  if (ldrValue < 2000) {
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" +
                 apiKey +
                 "&field1=" +
                 String(ldrValue);

    http.begin(url);

    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.print("Data Sent Successfully. HTTP Code: ");
      Serial.println(httpCode);
    }
    else {
      Serial.print("Error Sending Data. HTTP Code: ");
      Serial.println(httpCode);
    }

    http.end();
  }

  else {
    Serial.println("WiFi Disconnected");
  }

  // ThingSpeak Free Version: Minimum 15 sec delay
  delay(20000);

}