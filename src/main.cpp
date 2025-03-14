#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
String         completeWsString = "";

void setup() {
  Serial.begin(115200);

  WiFi.begin("nomeWifi", "passwordWifi");
  while (WiFi.status() != WL_CONNECTED) {};
  Serial.println("WiFi connected! IP Address: " + WiFi.localIP().toString());

  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                uint8_t *data, size_t len) {
    const bool hasData = type == WS_EVT_DATA;
    if (!hasData) { return; }
    
    AwsFrameInfo *info                     = (AwsFrameInfo *) arg;
    const String  thisWsReceivedStringData = String((char *) data).substring(0, len);
    completeWsString += thisWsReceivedStringData;
    if (!(info->final)) { return; };

    ws.textAll(completeWsString);

    completeWsString = "";
  });

  server.addHandler(&ws);
  server.begin();
}

void loop() {}