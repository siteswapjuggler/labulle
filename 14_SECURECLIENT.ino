//----------------------------------------------------------------------------------------
// HTTPS REQUEST
//----------------------------------------------------------------------------------------

const char* host = "maker.ifttt.com";
const int   httpsPort = 443;
const char  fingerprint[] = "AA 75 CB 41 2E D5 F9 97 FF 5D A0 8B 7D AC 12 21 08 4B 00 8C";
String url = "/trigger/emptyTank/with/key/kC_z0un3Cw-1rSXr2IPRj7JF0UN327HT54Aif-mnD56";

void sendTrigger() {
  if (hasWifi && sset.tankAlarm == false) {
    WiFiClientSecure client;
    debugPrint(HTTPS_CONNECT);

    client.setFingerprint(fingerprint);
    if (!client.connect(host, httpsPort)) {
      debugPrint(HTTPS_FAIL);
      return;
    }
/*
    if (client.verify(fingerprint, host))
      debugPrint(FP_OK);
    else
      debugPrint(FP_KO);
*/

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: LaBulle\r\n" +
                 "Connection: close\r\n\r\n");

    sset.tankAlarm = true;
    saveStateSettings;
    
    debugPrint(IFTTT_OK);
  }
}
