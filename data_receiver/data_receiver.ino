#include <esp_now.h>
#include <WiFi.h>

typedef struct message{
  char a[32];
  int b;
  float c;
  bool d;
} message;

message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes Received: ");
  Serial.println(len);

  Serial.print("Char: ");
  Serial.println(myData.a);

  Serial.print("Int: ");
  Serial.println(myData.b);

  Serial.print("Float: ");
  Serial.println(myData.c);

  Serial.print("Bool: ");
  Serial.println("myData.d");
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error Initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}