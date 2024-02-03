#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0x4E, 0x08, 0x3C};

typedef struct message{
  char a[32];
  int b;
  float c;
  bool d;
} message;

message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\n Last Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Successful" : "Delivery Failed");
}

void setup(){
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if(esp_now_init() != ESP_OK) {
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
  strcpy(myData.a, "CHAR TEST");
  myData.b = random(1, 20);
  myData.c = 1.2;
  myData.d = false;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with Success");
  }
  else {
  Serial.println("Error Sending Data");
  }
  delay(2000);
}