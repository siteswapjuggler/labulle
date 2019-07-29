//----------------------------------------------------------------------------------------
// EEPROM PARAMETER
//----------------------------------------------------------------------------------------

#define EEPROM_SIZE       1024
#define WS_ADDRESS        0x000   // WiFi Settings
#define GS_ADDRESS        0x100   // General Settings
#define ST_ADDRESS        0x200   // State Settings

//----------------------------------------------------------------------------------------
// GENERAL FUNCTIONS
//----------------------------------------------------------------------------------------

void initEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(WS_ADDRESS, wset);
  EEPROM.get(GS_ADDRESS, gset);
  EEPROM.get(ST_ADDRESS, sset);
  EEPROM.end();
  debugPrint(EEPROM_INIT);
}

//----------------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//----------------------------------------------------------------------------------------

void defaultSettings() {
  gset.soilThreshold = 65.;
  gset.tempThreshold = 30.;
  gset.startTime = 8.;
  gset.endTime = 20.;
  gset.sleepTime = 0.;
}

void saveSettings() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(GS_ADDRESS, gset);
  EEPROM.end();
}

void defaultSateSettings() {
  sset.servo = false;
  sset.tankAlarm = false;
}

void saveStateSettings() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(ST_ADDRESS, sset);
  EEPROM.end();
}
