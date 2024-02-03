#include <esp_now.h>
#include <WiFi.h>
#include <ezButton.h>

#define BUTTON1_PIN 21
#define BUTTON2_PIN 19
#define BUTTON3_PIN 27

ezButton button1(BUTTON1_PIN);
ezButton button2(BUTTON2_PIN);
ezButton button3(BUTTON3_PIN);

uint8_t broadcastAddress[] = { 0xC8, 0xF0, 0x9E, 0x4E, 0x08, 0x3C };

typedef struct message {
  int servo1_angle;
  int servo2_angle;
  bool motor_status;
} message;

message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\n Last Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Successful" : "Delivery Failed");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error Initializing ESP_NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to Add Peer");
    return;
  }
}

void loop() {
  button1.loop();
  button2.loop();

  if (button1.isPressed()) {
    if (myData.servo1_angle == 0) {
      myData.servo1_angle = 180;
    } else if (myData.servo1_angle == 180) {
      myData.servo1_angle = 0;
    }
  }

  if (button2.isPressed()) {
    if (myData.servo2_angle == 0) {
      myData.servo2_angle = 180;
    } else if (myData.servo2_angle == 180) {
      myData.servo2_angle = 0;
    }
  }

  if (button3.isPressed()) {
    if (myData.motor_status == true) {
      myData.motor_status == false;
    } else if (myData.motor_status == false) {
      myData.motor_status == true;
    }
  }

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with Success");
    Serial.println("Servo 1 Angle: ");
    Serial.print(myData.servo1_angle);
    Serial.println();
    Serial.println("Servo 2 Angle: ");
    Serial.print(myData.servo2_angle);
    Serial.println();
    Serial.println("Motor Status: ");
    Serial.print(myData.motor_status);
  } else {
    Serial.println("Error Sending Data");
  }
  delay(2000);
}