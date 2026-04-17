#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// ================= WIFI =================
const char* ssid = "Sobirinn";
const char* password = "barokah354";

// ================= MQTT =================
const char* mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient client(espClient);

// ================= PIN =================
#define WATER_SENSOR 35
#define BUZZER 19
#define SERVO_PIN 5

Servo myServo;

// ================= VARIABLE =================
String mode = "otomatis";
int servoPos = 0;
bool buzzerManual = false;

unsigned long lastBlink = 0;
bool buzzerState = false;

unsigned long lastSend = 0;

// ================= WIFI =================
void setup_wifi() {
  Serial.println("Menghubungkan WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

// ================= MQTT CALLBACK =================
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";

  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  String t = String(topic);

  Serial.print("MQTT Masuk [");
  Serial.print(t);
  Serial.print("] = ");
  Serial.println(msg);

  if (t == "ando/iot/mode") {
    mode = msg;
  }

  else if (t == "ando/iot/servo" && mode == "manual") {
    servoPos = constrain(msg.toInt(), 0, 180);
    myServo.write(servoPos);
  }

  else if (t == "ando/iot/buzzer" && mode == "manual") {
    buzzerManual = msg.toInt();
    digitalWrite(BUZZER, buzzerManual);
  }
}

// ================= MQTT RECONNECT =================
void reconnect() {
  while (!client.connected()) {
    Serial.println("Menghubungkan MQTT...");

    if (client.connect("esp32_bendungan_ando")) {
      Serial.println("MQTT Connected!");

      client.subscribe("ando/iot/mode");
      client.subscribe("ando/iot/servo");
      client.subscribe("ando/iot/buzzer");

    } else {
      Serial.println("Gagal MQTT, retry...");
      delay(2000);
    }
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  myServo.attach(SERVO_PIN);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ================= LOOP =================
void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  int level = analogRead(WATER_SENSOR);

  // ===== STATUS =====
  String statusLevel;
  if (level <= 800) statusLevel = "AMAN";
  else if (level <= 1500) statusLevel = "WASPADA";
  else statusLevel = "BAHAYA";

  // ================= MODE OTOMATIS =================
  if (mode == "otomatis") {

    if (level <= 800) servoPos = 0;
    else if (level <= 1500) servoPos = 90;
    else servoPos = 180;

    myServo.write(servoPos);

    // ===== BUZZER =====
    if (level > 1500) {
      Serial.println(">>> KONDISI BAHAYA - BUZZER AKTIF <<<");

      if (millis() - lastBlink > 500) {
        lastBlink = millis();
        buzzerState = !buzzerState;
        digitalWrite(BUZZER, buzzerState);
      }
    } else {
      digitalWrite(BUZZER, LOW);
    }

  } else {
    digitalWrite(BUZZER, buzzerManual);
  }

  // ================= SERIAL MONITOR =================
  Serial.print("Level: ");
  Serial.print(level);
  Serial.print(" | Status: ");
  Serial.print(statusLevel);
  Serial.print(" | Mode: ");
  Serial.print(mode);
  Serial.print(" | Servo: ");
  Serial.print(servoPos);
  Serial.print(" | Buzzer: ");
  Serial.println(digitalRead(BUZZER));

  // ================= MQTT =================
  if (millis() - lastSend > 1000) {
    lastSend = millis();

    client.publish("ando/iot/level", String(level).c_str());
    client.publish("ando/iot/status", statusLevel.c_str());
    client.publish("ando/iot/servo", String(servoPos).c_str());
    client.publish("ando/iot/buzzer_status", digitalRead(BUZZER) ? "ON" : "OFF");
    client.publish("ando/iot/mode_status", mode.c_str());
  }

  delay(500);
}