
void checkForSerialAndWork() {
  char dataIn[MAX_SERIAL_BUFFER];
  int datalen = 0;  // stores the lenght of data stored
  while (Serial.available()) {
    char c = (char)Serial.read();
    dataIn[datalen++] = c;

    if (datalen > MAX_SERIAL_BUFFER) {
      Serial.print(F("Buffer Data too LARGE!!!"));
      break;
    }
    delay(5); //LET THE NEXT DATA BYTE COME IN
  }//while ends

  datalen--; //removing extra addition in last iteration of  while due to last datalen++
  int payloadlen = (dataIn[0] - '0') ;
  char* token[payloadlen];

  split(dataIn, token);
  const  char* hyphen = "-";

  if (StrComp(token[1], "Z", 1)) {
    memset(&dataIn[0], 0, sizeof(dataIn));

    for (int i = 1; i < payloadlen - 1; i++) {
      strcat(dataIn, token[i]);
      strcat(dataIn, hyphen);
    }
    webSocketServer.sendTXT(dataIn[payloadlen - 1] - '0', dataIn);
  }

  else if (StrComp(token[1], "H", 1) && StrComp(token[2], "T", 1)) {
    memset(&dataIn[0], 0, sizeof(dataIn));
    for (int i = 1; i < payloadlen; i++) {
      strcat(dataIn, token[i]);
      strcat(dataIn, hyphen);
    }
    webSocketclient.sendTXT(dataIn);
  }

  // doing things based on data recvd...
  if (StrComp(dataIn, CONFIG, datalen)) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    memset(&dataIn[0], 0, sizeof(dataIn));
  }



} // func ends

