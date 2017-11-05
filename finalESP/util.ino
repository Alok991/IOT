bool StrComp(const char* a, const char* b, int len) {


  for (int i = 0; i < len; i++)
  {
    if (a[i] == b[i])
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

// Following function is written by Abhishek chauhan https://www.facebook.com/abhishek.chauhan.9655?fref=ts
//Because of some magin happening in strtok :)
void split(char *payload, char *arr[]){
  int n=payload[0]-'0';
  uint8_t wordCount = 0;
//  char *arr[n];
  char *temp=NULL;
  int j = 0;
  for(int i=0;i<n; i++){
    temp = new char[20];
    int k=0;
    for(; payload[j]!='\0'; j++){
      if(payload[j]!='-')
        temp[k++]=payload[j];
      else{
        arr[i] = temp;
        temp[k] = '\0';
        k=0;
        j++;
        break;
      }
    }
  }
  for(int i=0;i<n;i++){
    Serial.println(arr[i]);
    delay(4);
  }
}

// splits by "-" into n diff strings
//void split(char *payload, char *arr[]) {
//  u8 i = 0;
//  arr[i] = strtok (payload, "-");
//  Serial.println(arr[0]);
//
//  while (arr[i] != NULL) {
//    arr[++i] = strtok(NULL, "-");
//    Serial.println(arr[i]);
//  }
//  Serial.println(arr[0]);
//  delay(5);
//  Serial.println(arr[1]);
//  delay(5);
//  Serial.println(arr[2]);
//
//  return;
//}

void setupESPandBaud() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(100);
  Serial.println("Setup started");
  EEPROM.begin(512);
  for (uint8_t t = 4; t > 0; t--)
    delay(500);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("Setup Success");
}
void connectToWiFi()
{
  char  wifissid[32];
  char  wifipass[32];
  //Deconstructor for WiFiMulti to remove stored settings
  if (!startup) {
    Serial.println("Destoryingggggggggg!!!!!!!!!!!!!!!!!!!!");
    wiFiMulti.~ESP8266WiFiMulti();
  }
  //Reading wifi credentials from EEPROM ssid(64-96) and pass(96-128)
  memset(&wifissid[0], 0, sizeof(wifissid));
  memset(&wifipass[0], 0, sizeof(wifipass));

  /*
     We are storing the length of wifi ssid and password at address 0 and 32.
  */

  u8 wifinamelen = EEPROM.read(0);
  u8 wifipasslen = EEPROM.read(33);

  for (u8 i = 0; i < wifinamelen; i++) {
    wifissid[i] = (char)EEPROM.read(i + 1);
  }
  for (u8 i = 0; i < wifipasslen; i++) {
    wifipass[i] = (char)EEPROM.read(i + 34);
  }
  if (wifinamelen != 0 && wifipasslen != 0) {
    wiFiMulti.addAP(wifissid, wifissid);
    while (wiFiMulti.run() != WL_CONNECTED) {
      delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
void connectToServer() {
  //Starting websocket connection with callback function websocketEvent
  webSocketclient.begin(ipServer, portServer);
  webSocketclient.onEvent(webSocketclientEvent);
}

