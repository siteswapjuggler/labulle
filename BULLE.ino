/*----------------------------------------------------------------------------------------
   BULLE - SERRE CONNECTÉE

   Un projet réalisé dans le cadre du Pacours Digital 2018/2019

   Co-création des participants et des encadrants du Dôme, de l'E2C Normandie,
   de la Mission Locale Caen-la-Mer Calavados Centre et d'Orange Solidarité.

   !ATTENTION!

   ESPCore 2.5.2 
   Carte: Wemos D1 R2 & Mini
   CPU Frequency: 80 MHz
   FlashSize: 4M(2M SPIFFS)
   IwIP Variant : v2 Lower Memory (no features)
  ----------------------------------------------------------------------------------------*/

bool hasWifi = false;

void setup() {
  // HARDWARE SETUP
  initDebug();
  initEEPROM();
  initDevice();

  // WIFI CONNECTION
  hasWifi = connectWifi();
  updateNTP(true);

  // SERVICES & ACCESS POINT
  initAccessPoint();
  initWebSocket();
  initServer();
  initDNS();
  initMDNS();

  // READY TO START
  deviceReady();
}

void loop() {
  //hasWifi = checkWifi(); // Reconnection en cas de besoin
  updateDNS();           // Important pour le maintien du DNS
  updateMDNS();          // Important pour le maintien du MSDNS
  updateServer();        // Gestion des requêtes du serveur
  updateWebSocket();     // Gestion des requêtes sur le websocket

  yield();               // Traitement anticipé des tâches Wifi

  updateBlink();         // Une veilleuse pour s'assurer du fonctionnement
  updateSensor();        // Continuer à mettre à jour les capteurs
  updateServos();        // Au cas où les servos bouges
  updateTaskAssign();    // On assigne les tâche à faire
  updateTaskTimers();    // On mets à jours les timers de tâche

  yield();               // Traitement anticipé des tâches Wifi

  //printSeconds();
  updateDeepSleep();
}
