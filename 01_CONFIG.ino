//----------------------------------------------------------------------------------------
// USER PARAMETERS
//----------------------------------------------------------------------------------------

#define DEBUG                    // uncomment to get Serial debug
#define ON_TIME              15  // Default time before standby (minutes)
#define FAN_TIME              2  // Fan time (minutes)
#define WATER_TIME            2  // Watering time (minutes)
#define MOTOR_TIME            3  // Motor Moving time (seconds)
#define MONITOR_TIME          2  // Time Before Monitoring Again (minutes)
#define CONNECTION_TIMEOUT   10  // WiFi connection timout (seconds)

//----------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------------------

_wifiSettings wset;
_stateSettings sset;
_generalSettings gset;
String prescanNetworks;

elapsedMillis standbyTimer,secondTicker;                    // General Timer
taskState wateringTask = NOT_ASSIGNED;                      // Tasks states
taskState aerationTask = NOT_ASSIGNED;                      // Tasks states
taskState windowsTask  = NOT_ASSIGNED;                      // Tasks states
elapsedMillis wateringTimer, aerationTimer, windowsTimer;   // Tasks timers


//----------------------------------------------------------------------------------------
// GENERAL FUNCTIONS
//----------------------------------------------------------------------------------------

void deviceReady() {
  standbyTimer = 0;
  secondTicker = 0;
  debugPrint(DEVICE_READY);
}

void resetStandby() {
  standbyTimer = 0;
}

void printSeconds() {
  if (secondTicker >= 1000) {
    Serial.println(ON_TIME*60 - standbyTimer / 1000);
    secondTicker = 0;
  }
}

void deviceReboot() {
  ESP.restart();
}
