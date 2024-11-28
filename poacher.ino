#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "HUAWEI-079E";
const char* password = "19468559";
const char* serverURL = "http://192.168.8.103:3002/addSensorData";

const int pirPin = 25;
bool motionState = false;
bool previousMotionState = false;

const int soundSensorPin = 34;
const int buttonPin = 27;
bool buttonPressed = false;
bool previousButtonState = false;

float lastReading = 0;   // Last reading for the low-pass filter
float alpha = 0.1;       // Smoothing factor (you can adjust this)

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  // PIR Sensor Code
  motionState = digitalRead(pirPin);
  String motionStatus = motionState ? "Motion Detected" : "No Motion Detected";

  // Sound Sensor Code with Low-Pass Filter
  int rawSoundValue = analogRead(soundSensorPin);

  // Apply Low-Pass Filter to smooth the reading
  lastReading = alpha * rawSoundValue + (1 - alpha) * lastReading;

  // Map the smoothed value to a dB level (you may adjust the mapping range)
  int db = map(lastReading, 0, 1024, 49, 90);

  // Button Code
  buttonPressed = !digitalRead(buttonPin);
  String buttonStatus = buttonPressed ? "Wildlife detected in Room B006" : "No Wildlife Detected";

  // Print sensor data to Serial Monitor
  Serial.println("----- Sensor Data -----");
  Serial.print("Motion Status: ");
  Serial.println(motionStatus);
  Serial.print("Sound Value (dB): ");
  Serial.println(db);
  Serial.print("Button Status: ");
  Serial.println(buttonStatus);
  Serial.println("-----------------------");

  // Send data if there's a change in motion or button state
  if (motionState != previousMotionState || buttonPressed != previousButtonState) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverURL);
      http.addHeader("Content-Type", "application/json");

      // Construct JSON payload
      String jsonData = "{\"motion_status\":\"" + motionStatus +
                        "\", \"sound_value\":" + String(db) +
                        ", \"button_status\":\"" + buttonStatus + "\"}";

      int httpResponseCode = http.POST(jsonData);
      if (httpResponseCode > 0) {
        Serial.print("Data sent successfully. HTTP Response code: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Error sending data: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("Error in WiFi connection");
    }
  }

  // Update previous states
  previousMotionState = motionState;
  previousButtonState = buttonPressed;
  
  delay(500);  // Delay for stability and to avoid rapid repeated sends
}