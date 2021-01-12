#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xF0, 0x08, 0xD1, 0xD8, 0x09, 0xE4};

typedef struct struct_message{
  int ledSwitch;
  int readingId;
  int led;
} struct_message;


struct_message myData;

unsigned long previousMillis = 0;  
const long interval = 1000;      

unsigned int readingId = 0;

constexpr char WIFI_SSID[] = "iPhone de Lucas";

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}

int dReadContact(int finger) {
  int val = digitalRead(finger);
  return val;
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\rLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success \n" : "Delivery Fail \n");
}
 
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  int32_t channel = getWiFiChannel(WIFI_SSID);

  WiFi.printDiag(Serial); 
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); 

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;
        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
   myData.ledSwitch=0;
}
 
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (dReadContact(16)==0)
    {
      if (myData.ledSwitch==0)
      {
        myData.ledSwitch = 1;
      }
      else if (myData.ledSwitch==1)
      {
        myData.ledSwitch = 0;
      }
      myData.readingId = readingId++;
      myData.led = 16;
    }

    if (dReadContact(17)==0)
    {
      if (myData.ledSwitch==0)
      {
        myData.ledSwitch = 1;
      }
      else if (myData.ledSwitch==1)
      {
        myData.ledSwitch = 0;
      }
       myData.readingId = readingId++;
       myData.led = 17;
    }

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(struct_message));
        
    if (result == ESP_OK) {
      Serial.println("Sent with success");
      Serial.println(dReadContact(16));
      Serial.println(dReadContact(17));
    }
    else {
      Serial.println("Error sending the data");
    }
}
}
