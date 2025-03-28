#include <Arduino.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <map>
#include <WiFi.h>


/*
  RICORDA DI SCRIVERE QUESTO COMMANDO:
  pio run --target uploadfs --verbose
  PRIMA DI COMPILARE TRAMITE IL BUTTONE BUILD
*/

/*
  IN CHROME, ATTIVA:
  chrome://flags/#unsafely-treat-insecure-origin-as-secure
  INSERENDO http://192.168.252.0/ CHE TROVI NEL SERIAL MONITOR
*/

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

std::map<uint32_t, String> mapCompleteWsStrings;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {};
  Serial.println("WiFi connected! IP Address: " + WiFi.localIP().toString());
  // Serial.println("Gateway:" + WiFi.gatewayIP().toString() + "\t" +
  //                "subnet: " + WiFi.subnetMask().toString());

  if (!LittleFS.begin()) {
    Serial.print("LittleFS filesystem fail");
    delay(1000);
    ESP.restart();
    return;
  };

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                uint8_t *data, size_t len) {
    uint32_t thisClientID = client->id();

    const bool isFirstConnection = type == WS_EVT_CONNECT;
    const bool isDisconnected    = type == WS_EVT_DISCONNECT;

    if (isFirstConnection) {
      mapCompleteWsStrings[thisClientID] = "";
      return;
    };

    if (isDisconnected) {
      mapCompleteWsStrings.erase(thisClientID);
      return;
    };

    const bool hasData = type == WS_EVT_DATA;
    if (!hasData) { return; }

    const String thisWsReceivedStringData = String((char *) data).substring(0, len);
    mapCompleteWsStrings[thisClientID] += thisWsReceivedStringData;

    AwsFrameInfo *info = (AwsFrameInfo *) arg;
    if (!(info->final)) { return; };

    const String thisCompleteWsString = mapCompleteWsStrings[thisClientID];
    Serial.println(thisCompleteWsString);

    mapCompleteWsStrings[thisClientID] = "";
  });

  server.addHandler(&ws);
  server.begin();
};

void loop() {};