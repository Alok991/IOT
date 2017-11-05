void webSocketServerEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      // Try to reconnect
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocketServer.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      {
        Serial.printf("[%u] get Text: %s\n", num, payload);
        String payload_str = (char*)payload;
        int payloadlen = ((char)payload[0] - '0') ;
        char* token[payloadlen];
        split((char*)payload, token);

        if (StrComp(token[1], "A", 1)) {

          if (StrComp(token[2], "N", 1) && StrComp(token[3], Unique_id, strlen(Unique_id))) {

            u8 token_4_size = strlen(token[4]);

            if (sizeof(token_4_size > 0)) {
              EEPROM.write(0, token_4_size);
              //saving ssid
              for (u8 i = 0; i < token_4_size; i++) {
                EEPROM.write(i + 1, token[4][i]);
              }
            }


            u8 token_5_size = strlen(token[5]);

            if (token_5_size > 0) {
              EEPROM.write(33, token_5_size);
              //saving password
              for (u8 i = 0; i < token_5_size; i++) {
                EEPROM.write(i + 34, token[5][i]);
              }
            }
            EEPROM.commit();
            delay(10);

            webSocketServer.sendTXT(num, String("H-R-") + String(myDNS));
            delay(10);
            startup = true;
          }
          else if (StrComp(token[2], "P", 1)) {
            //Transmit the IDs to stm by serial and store them in EEPROM
            Serial.println(payload_str);
          }
          else if (StrComp(token[2], "Q", 1)) {
            Serial.println(payload_str + String(num) + "-");
          }
          else if (StrComp(token[2], "O", 1)) {
            //(#-a-home_id-hub_id-slave_id-SwNum-state-)
            char msg[64];
            strcat(msg, token[0]); strcat(msg, "-"); strcat(msg, "a-");
            for (int i = 3; i < payloadlen; i++) {
              strcat(msg, token[i]);
              strcat(msg, "-");
            }
            Serial.println(msg);
          }
          else if (StrComp(token[2], "U", 1)) {
            if (startup) {
              setup();
            }
          }
        }
      }
      break;
    case WStype_BIN:
      hexdump(payload, lenght);
      break;
  }
}


//Android → Hub (P2P)
//*Uniques key exchange  with ssid and password : #-A-N-unique_key-SSID-password reply: mDNS name
//*control messages #-A-O-MasterID-SlaveID-data-  $data is to be parsed by slave nodes reply=SUCCESS
//*Send_ids #-A-P-Home_ID-Master_ID-Slave_ID1-Slave_ID2...-
//*update request #-A-Q-MasterID-SlaveID reply=data
//*reconnect wifi #-A-U-

//Hub → Android(P2P)
//*mDNS name H-R-mDNS
//Update request reply #-1-0-1-1-4-0, #-19  ******


//Websocket Event Call Back Function
void webSocketclientEvent(WStype_t type, uint8_t * payload, size_t lenght)
{
  switch (type) {
    case WStype_DISCONNECTED:

      break;
    case WStype_CONNECTED: {
        String msg = "H-S-" + String(Unique_id);
        webSocketclient.sendTXT(msg);
      }
      break;
    case WStype_TEXT: {
        int payloadlen = ((char)payload[0] - '0') ;
        String payload_str = (char*)payload;
        char* token[payloadlen];
        split((char*)payload, token);
        if (StrComp(token[1], "S", 1)) {
          if (StrComp(token[2], "W", 1)) {
            //(#-a-home_id-hub_id-slave_id-SwNum-state-)
            char msg[64];
            strcat(msg, token[0]); strcat(msg, "-"); strcat(msg, "a-");
            for (int i = 3; i < payloadlen; i++) {
              strcat(msg, token[i]);
              strcat(msg, "-");
            }
            Serial.println(msg);
          }
        }
      }

      break;
    case WStype_BIN:
      delay(1);
      // hexdump(payload, lenght);
      break;
  }
}
