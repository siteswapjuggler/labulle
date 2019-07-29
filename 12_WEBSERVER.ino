//----------------------------------------------------------------------------------------
// WEB SERVER
//----------------------------------------------------------------------------------------

ESP8266WebServer server(80);

void initServer() {
  SPIFFS.begin();
  server.on("/", handleRoot);
  server.onNotFound(handleAny);
  server.on("/setConfig", setConfig);
  server.on("/prescan", prescanNetwork);
  server.on("/reboot", deviceReboot);
  server.on("/scan", scanNetwork);
  server.begin();
  debugPrint(SERVER_INIT);
}

void updateServer() {
  server.handleClient();
}

//----------------------------------------------------------------------------------------
// RESSOURCE HANDLERS
//----------------------------------------------------------------------------------------

void handleAny() {
  if (!handleFileRead(server.uri()))
    handleError();
}

void handleRoot() {
  String root = !hasWifi ? "/wifi.html" : "/config.html";
  if (!handleFileRead(root))
    handleError();
}

void handleError() {
  if (!handleFileRead("/404.html"))
    server.send(404, F("text/plain"), F("404: Not Found"));
}

//----------------------------------------------------------------------------------------
// SERVER API
//----------------------------------------------------------------------------------------

void prescanNetwork() {
  server.send(200, "text/plain", prescanNetworks);
}

void scanNetwork() {
  String options = listNetwork();
  server.send(200, "text/plain", options);
}

void setConfig() {
  server.arg("ssid").toCharArray(wset.ssid, PARAMETER_LENGTH);
  server.arg("pwd").toCharArray(wset.pwd, PARAMETER_LENGTH);
  saveWifiSettings();
  debugPrint(WIFI_SETTINGS);
  if (!handleFileRead("/reboot.html")) handleError();
}

//----------------------------------------------------------------------------------------
// FILE SYSTEM HANDLERS
//----------------------------------------------------------------------------------------

bool handleFileRead(String path) {
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

String getContentType(String filename) {
  if (filename.endsWith(".html"))      return "text/html";
  else if (filename.endsWith(".htm"))  return "text/html";
  else if (filename.endsWith(".css"))  return "text/css";
  else if (filename.endsWith(".js"))   return "application/javascript";
  else if (filename.endsWith(".png"))  return "image/png";
  else if (filename.endsWith(".gif"))  return "image/gif";
  else if (filename.endsWith(".jpg"))  return "image/jpeg";
  else if (filename.endsWith(".ico"))  return "image/x-icon";
  else if (filename.endsWith(".xml"))  return "text/xml";
  else if (filename.endsWith(".pdf"))  return "application/x-pdf";
  else if (filename.endsWith(".zip"))  return "application/x-zip";
  else if (filename.endsWith(".gz"))   return "application/x-gzip";
  return "text/plain";
}
