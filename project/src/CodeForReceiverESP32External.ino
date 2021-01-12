#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>

const char* ssid = "iPhone de Lucas";
const char* password = "huihui2em";

unsigned long previousMillis = 0; 
const long interval = 10000;   

unsigned int readingId = 0;

constexpr char WIFI_SSID[] = "iPhone de Lucas";

typedef struct struct_message{
  int ledSwitch;
  int readingId;
  int led;
} struct_message;

struct_message incomingReadings;

JSONVar board;

AsyncWebServer server(80);
AsyncEventSource events("/events");

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
  board["ledSwitch"] = String(incomingReadings.ledSwitch);
  board["readingId"] = String(incomingReadings.readingId);
  board["led"] = String(incomingReadings.led);
  String jsonString = JSON.stringify(board);
  
  events.send(jsonString.c_str(), "new_readings", millis());
 
  Serial.printf("ledSwitch: %d \n", incomingReadings.ledSwitch);
  Serial.printf("readingID value: %d \n", incomingReadings.readingId);
  Serial.printf("command from finger: %d \n", incomingReadings.led);
  Serial.println();

  if (incomingReadings.ledSwitch == 1 && incomingReadings.led == 16)
  {
    digitalWrite(16, HIGH);
  }
  else if (incomingReadings.ledSwitch == 1 && incomingReadings.led == 17)
  {
    digitalWrite(17, HIGH);
  }
  else if (incomingReadings.ledSwitch == 0 && incomingReadings.led == 16)
  {
    digitalWrite(16, LOW);
  }
  else if (incomingReadings.ledSwitch == 0 && incomingReadings.led == 17)
  {
    digitalWrite(17, LOW);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .ledswitch { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .onLed { font-size: 2.8rem; color: green;}
    .offLed { font-size: 2.8rem; color: red;}
    .packet { color: #bebebe; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-NOW DASHBOARD</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="ledswitch">
        <h4><i class="far fa-lightbulb"></i> GREEN LED</h4><p><span class="onLed"><span id="t1"></span></span><span class="offLed"><span id="t2"></span></span></p><p class="packet">Reading ID: <span id="rt1"></span></p>
      </div>
      <div class="ledswitch">
        <h4><i class="far fa-lightbulb"></i> BLUE LED</h4><p><span class="onLed"><span id="t3"></span></span><span class="offLed"><span id="t4"></span></span></p><p class="packet">Reading ID: <span id="rt2"></span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  if (obj.ledSwitch == 1 && obj.led == 16)
  {
    document.getElementById("t1").innerHTML = "ON";
    document.getElementById("t2").innerHTML = "";
  }
  else if (obj.ledSwitch == 0 && obj.led == 16)
  {
    document.getElementById("t2").innerHTML = "OFF";
    document.getElementById("t1").innerHTML = "";
  }
  else if (obj.ledSwitch == 1 && obj.led == 17)
  {
    document.getElementById("t3").innerHTML = "ON";
    document.getElementById("t4").innerHTML = "";
  }
  else if (obj.ledSwitch == 0 && obj.led == 17)
  {
    document.getElementById("t3").innerHTML = "OFF";
    document.getElementById("t4").innerHTML = "";
  }
  if (obj.led == 16)
  {
  document.getElementById("rt1").innerHTML = obj.readingId;
  }
  else if(obj.led == 17)
  {
  document.getElementById("rt2").innerHTML = obj.readingId;
  }
 }, false);
}
</script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  WiFi.mode(WIFI_AP_STA);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
   
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}
 
void loop() {
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000; 
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    events.send("ping",NULL,millis());
    lastEventTime = millis();
  }
}
