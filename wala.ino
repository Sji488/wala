#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "Sjie";
const char* password = "sjie2002";

// Define the pin for the LED
const int ledPin = 2; // D4 on ESP8266 NodeMCU

// Create an instance of the server on port 80
ESP8266WebServer server(80);

const char MAIN_page[] PROGMEM = R"====(
<!DOCTYPE html>
<html>
<head>
  <title>ESP8266 Control</title>
</head>
<body>
  <h1>Control ESP8266</h1>
  <button onclick="sendAction('on')">Turn On</button>
  <button onclick="sendAction('off')">Turn Off</button>

  <script>
    function sendAction(action) {
      fetch('/control', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: `action=${action}`
      })
      .then(response => response.text())
      .then(data => alert(data));
    }
  </script>
</body>
</html>
)====";

void setup() {
  Serial.begin(115200);

  // Initialize LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Make sure LED is off initially

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", MAIN_page);
  });

  server.on("/control", HTTP_POST, []() {
    String action = server.arg("action");
    Serial.print("Action received: ");
    Serial.println(action);
    if (action == "on") {
      digitalWrite(ledPin, HIGH); // Turn LED on
      Serial.println("LED is ON");
      server.send(200, "text/plain", "LED is ON");
    } else if (action == "off") {
      digitalWrite(ledPin, LOW); // Turn LED off
      Serial.println("LED is OFF");
      server.send(200, "text/plain", "LED is OFF");
    } else {
      Serial.println("Invalid action");
      server.send(400, "text/plain", "Invalid action");
    }
  });

  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}
