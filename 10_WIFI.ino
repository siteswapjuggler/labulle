//----------------------------------------------------------------------------------------
// WIFI CONNECTION
//----------------------------------------------------------------------------------------

ESP8266WiFiMulti wifiMulti;

boolean connectWifi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  debugPrint(WIFI_SCAN);
  prescanNetworks = listNetwork();
  if (prescanNetworks == "false") {
    debugPrint(WIFI_OUT);
    return false;
  }
  wifiMulti.addAP(wset.ssid, wset.pwd);
  return checkWifi();
}

String listNetwork() {
  String options;
  int n = WiFi.scanNetworks();
  if (n == 0)
    options += "false";
  else {
    for (int i = 0; i < n; ++i) {
      options += WiFi.SSID(i);
      if (i < n - 1) options += ",";
    }
  }
  return options;
}

bool checkWifi() {
  if (WiFi.status() == WL_CONNECTED)
    return true;
    
  debugPrint(WIFI_TRY);
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) {
    debugPrint(WIFI_WAIT);
    if (i++ > CONNECTION_TIMEOUT || WiFi.status() == WL_CONNECT_FAILED) {
      debugPrint(WIFI_FAIL);
      return false;
    }
    delay(1000);
  }
  debugPrint(WIFI_SUCCESS);
  return true;
}

void disconnectWifi() {
  WiFi.disconnect();
}

//----------------------------------------------------------------------------------------
// ACCESS POINT CONFIGURATION
//----------------------------------------------------------------------------------------

#define DNS_PORT 53
DNSServer dnsServer;
IPAddress apIP(192, 168, 100, 1);

void initAccessPoint() {
  debugPrint(AP_CONFIG);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("LABULLE", "PDIG2019");
}

void initDNS() {
  dnsServer.start(DNS_PORT, "setup.labulle.io", apIP);
  debugPrint(DNS_INIT);
}

void updateDNS() {
  dnsServer.processNextRequest();
}

//----------------------------------------------------------------------------------------
// MDNS CONFIGURATION
//----------------------------------------------------------------------------------------

void initMDNS() {
  MDNS.begin("labulle");
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
  debugPrint(MDNS_INIT);
}

void updateMDNS() {
  MDNS.update();
}

//----------------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//----------------------------------------------------------------------------------------

void defaultWifiSettings() {
  strcpy(wset.ssid, "ssid");
  strcpy(wset.pwd, "password");
}

void saveWifiSettings() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(WS_ADDRESS, wset);
  EEPROM.end();
}
