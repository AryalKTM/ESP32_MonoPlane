#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define SERVO1_PIN 25
#define SERVO2_PIN 26
#define MOTOR_PIN 33

typedef struct message {
  int servo1_angle;
  int servo2_angle;
  bool motor_status;
} message;

Servo servo1;
Servo servo2;

message myData;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes Received: ");
  Serial.println(len);
  Serial.println();
  Serial.print("Servo 1 Angle: ");
  Serial.println(myData.servo1_angle);
  servo1.write(myData.servo1_angle);
  Serial.print("Servo 2 Angle: ");
  Serial.println(myData.servo2_angle);
  servo2.write(myData.servo2_angle);
  Serial.print("Motor Status: ");
  Serial.print(myData.motor_status);
  if (myData.motor_status == true) {
    digitalWrite(MOTOR_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_PIN, LOW);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error Initializing ESP-NOW");
    return;
  }

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  esp_now_register_recv_cb(OnDataRecv);

  servo1.write(myData.servo1_angle);
  servo2.write(myData.servo2_angle);
}

void loop() {
}