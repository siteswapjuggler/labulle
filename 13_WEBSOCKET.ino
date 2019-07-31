//----------------------------------------------------------------------------------------
// WEB SOCKET
//----------------------------------------------------------------------------------------

WebSocketsServer webSocket = WebSocketsServer(81);

void initWebSocket() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  debugPrint(WS_INIT);
}

void updateWebSocket() {
  webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  String m;
  switch (type) {
    case WStype_DISCONNECTED:
      debugPrint(WS_DISCONNECT);
      break;
    case WStype_CONNECTED:
      debugPrint(WS_CONNECT);
      break;
    case WStype_TEXT:
      char msg = payload[0];
      String val((char*)&payload[1]);
      
      //-------------------------------------------------------------
      // Les actions de l'utilisateur repoussent la mise en veille
      //-------------------------------------------------------------

      switch (msg) {
        case 'A':
        case 'W':
        case 'S':
        case 'E':
        case 'D':
        case 'Z':
        case 'w':
        case 's':
        case 'f':
        case 'l':
          resetStandby();
          break;
      }

      //-------------------------------------------------------------
      // Réagir aux demandes sur le websocket
      //-------------------------------------------------------------
      
      switch (msg) {
        case 'A':
          gset.tempThreshold = val.toFloat();
          debugPrint(WS_TEMP_TH);
          break;
        case 'W':
          gset.soilThreshold = val.toFloat();
          debugPrint(WS_SOIL_TH);
          break;
        case 'S':
          gset.startTime = val.toFloat();
          debugPrint(WS_START_TIME);
          break;
        case 'E':
          gset.endTime = val.toFloat();
          debugPrint(WS_END_TIME);
          break;
        case 'D':
          gset.sleepTime = val.toFloat();
          debugPrint(WS_SLEEP_TIME);
          break;
        case 'U':
          m += "U,";
          m += String(temp, 1) + " °C,";
          m += String(hygro, hygro == 100 ? 0 : 1) + " %,";
          m += String(soil , soil  == 100 ? 0 : 1) + " %,";
          m += (tank.read() ? "OK" : "KO");
          webSocket.sendTXT(num, m);
          break;
        case 'P':
          m += "P,";
          m += String(gset.tempThreshold) + ",";
          m += String(gset.soilThreshold) + ",";
          m += String(gset.startTime) + ",";
          m += String(gset.endTime) + ",";
          m += String(gset.sleepTime);
          webSocket.sendTXT(num, m);
          break;
        case 'T':
          m += "T,";
          m += String(water, DEC) + ",";
          m += String(sset.servo, DEC) + ",";
          m += String(fan, DEC) + ",";
          m += String(light, DEC);
          webSocket.sendTXT(num, m);
          break;
        case 'Z':
          saveSettings();
          debugPrint(GS_SAVE);
          break;
        case 'w':
          water = val.toInt();
          wateringTask = OVERRIDE;
          if (water.rise()) waterOn();
          else if (water.fall()) waterOff();
          debugPrint(WS_TOGGLE_WATER);
          break;
        case 's':
          sset.servo = val.toInt();
          windowsTask = OVERRIDE;
          if (sset.servo.rise()) openWindows();
          else if (sset.servo.fall()) closeWindows();
          debugPrint(WS_TOGGLE_SERVO);
          break;
        case 'f':
          fan = val.toInt();
          aerationTask = OVERRIDE;
          if (fan.rise()) fanOn();
          else if (fan.fall()) fanOff();
          debugPrint(WS_TOGGLE_FAN);
          break;
        case 'l':
          light = val.toInt();
          if (light.rise()) lightOn();
          else if (light.fall()) lightOff();
          debugPrint(WS_TOGGLE_LIGHT);
          break;
      }
      break;
  }
}
