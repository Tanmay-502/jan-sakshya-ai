/*
 * JAN-SAKSHYA AI — ESP32 inspection controller scaffold
 * Hardware: buttons on GPIO 18, 19, 21; buzzer control GPIO 23.
 *
 * IMPORTANT:
 * - Configure Wi-Fi/API/device-key/complaint placeholders before use.
 * - Use only simulated complaints and a trusted test network.
 * - Verify exact board and buzzer electrical requirements.
 * - Use an appropriate transistor/driver if the buzzer cannot safely be
 *   driven by the GPIO. Do not attach an unknown load directly.
 * - This is a demo scaffold, not production-grade device provisioning.
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Set these locally for your lab setup. Do not commit real credentials.
const char* WIFI_SSID = "CHANGE_ME";
const char* WIFI_PASSWORD = "CHANGE_ME";
const char* API_BASE_URL = "http://192.168.1.10:8000";
const char* DEVICE_API_KEY = "CHANGE_ME_DEMO_KEY";
const char* DEVICE_ID = "CIVIC-NODE-01";
const char* COMPLAINT_ID = "GRV-DEMO-1042";

constexpr int PIN_START = 18;
constexpr int PIN_EVIDENCE = 19;
constexpr int PIN_REVIEW = 21;
constexpr int PIN_BUZZER = 23;
constexpr unsigned long DEBOUNCE_MS = 45;

struct ButtonState {
  int pin;
  const char* eventType;
  bool lastReading;
  bool stableState;
  unsigned long changedAt;
};

ButtonState buttons[] = {
  {PIN_START, "inspection_started", HIGH, HIGH, 0},
  {PIN_EVIDENCE, "evidence_acknowledged", HIGH, HIGH, 0},
  {PIN_REVIEW, "flag_for_review", HIGH, HIGH, 0},
};

void beepOnce() {
  // Assumes an active buzzer with a suitable driver. Adjust for your hardware.
  digitalWrite(PIN_BUZZER, HIGH);
  delay(90);
  digitalWrite(PIN_BUZZER, LOW);
}

String makeEventId() {
  // Demo ID only. Production must use a durable UUID/event counter that survives retries.
  String id = String(DEVICE_ID) + "-" + String((uint32_t)esp_random(), HEX);
  return id;
}

bool sendEvent(const char* eventType) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("No Wi-Fi: event not sent. Offline queue is not implemented in this sketch.");
    return false;
  }

  HTTPClient http;
  String endpoint = String(API_BASE_URL) + "/api/inspection-events";
  http.begin(endpoint);
  http.setTimeout(4000);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Device-Key", DEVICE_API_KEY);

  StaticJsonDocument<256> doc;
  doc["event_id"] = makeEventId();
  doc["device_id"] = DEVICE_ID;
  doc["complaint_id"] = COMPLAINT_ID;
  doc["event_type"] = eventType;
  doc["client_timestamp"] = ""; // Server timestamp is authoritative for this MVP.

  String body;
  serializeJson(doc, body);
  int status = http.POST(body);
  String response = http.getString();
  http.end();

  Serial.printf("POST status: %d\n", status);
  if (status >= 200 && status < 300) {
    Serial.println(response);
    return true;
  }
  Serial.println("Event was not acknowledged; buzzer will not confirm success.");
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_START, INPUT_PULLUP);
  pinMode(PIN_EVIDENCE, INPUT_PULLUP);
  pinMode(PIN_REVIEW, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 15000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wi-Fi not connected. This sketch currently has no offline queue.");
  }
}

void loop() {
  unsigned long now = millis();
  for (auto &button : buttons) {
    bool reading = digitalRead(button.pin);
    if (reading != button.lastReading) {
      button.changedAt = now;
      button.lastReading = reading;
    }

    if ((now - button.changedAt) > DEBOUNCE_MS && reading != button.stableState) {
      button.stableState = reading;
      if (button.stableState == LOW) {
        Serial.printf("Button event: %s\n", button.eventType);
        if (sendEvent(button.eventType)) {
          beepOnce();
        }
      }
    }
  }
  delay(5);
}
