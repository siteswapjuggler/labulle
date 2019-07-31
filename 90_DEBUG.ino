//----------------------------------------------------------------------------------------
// DEBUG FUNCTIONS
//----------------------------------------------------------------------------------------

void initDebug() {
#ifdef DEBUG
  Serial.begin(115200);    // Boot-Mode standard speed
  Serial.println();
#endif
}

//----------------------------------------------------------------------------------------
// DEBUG MESSAGES
//----------------------------------------------------------------------------------------

void debugPrint(debug_msg msg) {
#ifdef DEBUG
  switch (msg) {
    case EEPROM_INIT:
    case WIFI_SCAN:
    case WIFI_FAIL:
    case WIFI_SUCCESS:
    case WIFI_SETTINGS:
    case AP_CONFIG:
    case DEVICE_READY:
    case DEEP_SLEEP:
      Serial.println();
  }

  switch (msg) {
    case EEPROM_INIT:
      Serial.println(F("BOOT\t> Chargement des données depuis l'EEPROM"));
      break;
    case SERVO_POS:
      Serial.println(F("BOOT\t> Mise en position des moteurs"));
      break;
    case WIFI_SCAN:
      Serial.println(F("WIFI\t> Recherche des réseaux WiFi"));
      break;
    case WIFI_OUT:
      Serial.println(F("WIFI\t> Aucun réseau WiFi à portée"));
      break;
    case WIFI_TRY:
      Serial.print(F("WIFI\t> Connexion au réseau \""));
      Serial.print(wset.ssid);
      Serial.print(F("\""));
      break;
    case WIFI_WAIT:
      Serial.print(".");
      break;
    case WIFI_FAIL:
      Serial.println(F("WIFI\t> Échec de la connexion WiFi"));
      break;
    case WIFI_SUCCESS:
      Serial.print(F("WIFI\t> Connexion WiFi réussie, votre adresse IP est le "));
      Serial.println(WiFi.localIP());
      break;
    case WIFI_SETTINGS:
      Serial.print(F("WIFI\t> Réseau enregistré : "));
      Serial.println(wset.ssid);
      Serial.print(F("WIFI\t> Mot de passe : "));
      Serial.println(wset.pwd);
      break;
    case AP_CONFIG:
      Serial.println(F("WIFI\t> Création du point d'accès\tOK"));
      break;
    case DNS_INIT:
      Serial.println(F("WIFI\t> Configuration du routage DNS\tOK"));
      break;
    case MDNS_INIT:
      Serial.println(F("WIFI\t> Configuration du routage mDNS\tOK"));
      break;
    case WS_INIT:
      Serial.println(F("WIFI\t> Configuration du WebSocket\tOK"));
      break;
    case SERVER_INIT:
      Serial.println(F("WIFI\t> Configuration du serveur Web\tOK"));
      break;
    case DEVICE_READY:
      Serial.println(F("SYS\t> Connectez vous au point d'accès \"LABULLE\" puis sur"));
      Serial.println(F("\t  \"setup.labulle.io\" pour configurer la serre.\n"));
      break;
    case REAL_TIME:
      Serial.print(F("WIFI\t> Connexion NTP réussie, il est "));
      Serial.println(timeClient.getFormattedTime());
      break;
    case WS_CONNECT:
      Serial.println(F("WS\t> Websocket connecté"));
      break;
    case WS_DISCONNECT:
      Serial.println(F("WS\t> Websocket déconnecté"));
      break;
    case WS_TEMP_TH:
      Serial.print(F("WS\t> Seuil d'aération : "));
      Serial.print(gset.tempThreshold, 1);
      Serial.println(" °C");
      break;
    case WS_SOIL_TH:
      Serial.print(F("WS\t> Seuil d'arrosage : "));
      Serial.print(gset.soilThreshold, 1);
      Serial.println(" %");
      break;
    case WS_START_TIME:
      Serial.print(F("WS\t> Heure de démarrage : "));
      printTime(gset.startTime);
      break;
    case WS_SLEEP_TIME:
      if (gset.sleepTime) {
        Serial.print(F("WS\t> Cycle d'allumage : "));
        printTime(gset.sleepTime);
      }
      else {
        Serial.print(F("WS\t> Cycle d'allumage désactivé"));
      }
      break;
    case WS_END_TIME:
      Serial.print(F("WS\t> Heure d'extinction: "));
      printTime(gset.endTime);
      break;
    case WS_TOGGLE_WATER:
      Serial.print(F("WS\t> Arrosage "));
      Serial.println(water ? F("allumé") : F("éteint"));
      break;
    case WS_TOGGLE_SERVO:
      Serial.print(F("WS\t> Fenêtres "));
      Serial.println(sset.servo ? F("ouvertes") : F("fermées"));
      break;
    case WS_TOGGLE_FAN:
      Serial.print(F("WS\t> Ventilation "));
      Serial.println(fan ? F("allumée") : F("éteinte"));
      break;
    case WS_TOGGLE_LIGHT:
      Serial.print(F("WS\t> Lumière "));
      Serial.println(light ? F("allumée") : F("éteinte"));
      break;
    case GS_SAVE:
      Serial.println(F("WS\t> Paramètres sauvegardés"));
      break;
    case HTTPS_CONNECT:
      Serial.print(F("HTTP\t> Connexion à "));
      Serial.println(host);
      break;
    case HTTPS_FAIL:
      Serial.println(F("HTTPS\t> Échec de la connexion"));
      break;
    case FP_OK:
      Serial.println(F("HTTPS\t> Certificat conforme"));
      break;
    case FP_KO:
      Serial.println(F("HTTPS\t> Certificat non conforme"));
      break;
    case IFTTT_OK:
      Serial.println(F("HTTPS\t> Événement IFTTT envoyé"));
      break;
    case DEEP_SLEEP:
      Serial.print(F("SYS\t> C'est l'heure de la sieste, au dodo pour "));
      Serial.print(nextStart/60);
      Serial.println(F(" minute(s)"));
      break;
    case AUTO_WATER_ON:
      Serial.println(F("AUTO\t> Arrosage automatique activé"));
      break;
    case AUTO_WATER_OFF:
      Serial.println(F("AUTO\t> Arrosage automatique terminé"));
      break;
    case AUTO_AIR_ON:
      Serial.println(F("AUTO\t> Aération automatique activée"));
      break;
    case AUTO_AIR_OFF:
      Serial.println(F("AUTO\t> Aération automatique terminée"));
      break;
    case AUTO_OPEN:
      Serial.println(F("AUTO\t> Ouverture des fenêtres"));
      break;
    case AUTO_CLOSE:
      Serial.println(F("AUTO\t> Fermeture des fenêtres"));
      break;
  }
  Serial.flush();
#endif
}

//----------------------------------------------------------------------------------------
// HELPERS
//----------------------------------------------------------------------------------------

void printTime(float t) {
  int hours = floor(t);
  int minutes = (t - floor(t)) * 60.;
  if (hours < 10) Serial.print("0");
  Serial.print(hours);
  Serial.print("h");
  if (minutes < 10) Serial.print("0");
  Serial.println(minutes);
}
