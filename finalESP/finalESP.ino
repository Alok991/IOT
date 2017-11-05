char ssid[32] = "AliveHome";
char password[32] = "password";
char *CONFIG = "CONFIG";
char *STOP_CONFIG = "STOP_CONFIG";
const char *Unique_id = "12345678";   //uniquely defined for each module
bool startup = false;
const char *ipServer = "10.124.195.10";
const int portServer = 80;
const char * myDNS ="myDNS";
const int MAX_SERIAL_BUFFER=500;

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <EEPROM.h>
#include <WebSocketsClient.h>


void split(char *payload, char *arr[]);
void WiFiEvent(WiFiEvent_t event);
void checkForSerialAndWork();
bool StrComp(const char* a, const char* b, int len);
void webSocketServerEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);
void webSocketclientEvent(WStype_t type, uint8_t * payload, size_t lenght);
void setupESPandBaud();
void connectToWiFi();
void connectToServer();

ESP8266WiFiMulti wiFiMulti;
WebSocketsServer webSocketServer = WebSocketsServer(9000);
WebSocketsClient webSocketclient;


void setup() {

  setupESPandBaud();

  connectToWiFi();
  connectToServer();


  //Setting up websocket server for P2P
  webSocketServer.begin();
  webSocketServer.onEvent(webSocketServerEvent);
}


void loop() {
  
  if (Serial.available())
    checkForSerialAndWork();

  webSocketServer.loop();

}
