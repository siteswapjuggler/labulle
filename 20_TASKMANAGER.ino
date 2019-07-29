//----------------------------------------------------------------------------------------
// AUTOMATED TASK MANAGEMENT
//----------------------------------------------------------------------------------------

void updateTaskAssign() {
  if (soil <= gset.soilThreshold && wateringTask == NOT_ASSIGNED) {
    water = 1;
    wateringTimer = 0;
    wateringTask  = ONGOING;
    waterOn();
    debugPrint(AUTO_WATER_ON);
  }

  if (temp >= gset.tempThreshold && aerationTask == NOT_ASSIGNED) {
    aerationTask = ONGOING;
    fan = 1;
    aerationTimer = 0;
    fanOn();
    debugPrint(AUTO_AIR_ON);
  }
  if (windowsTask == NOT_ASSIGNED) {
    if (temp >= gset.tempThreshold) {
      sset.servo = 1;
      if (sset.servo.rise()) {
        windowsTask  = ONGOING;
        windowsTimer = 0;
        openWindows();
        debugPrint(AUTO_OPEN);
      }
    }
    else {
      sset.servo = 0;
      if (sset.servo.fall()) {
        windowsTask  = ONGOING;
        windowsTimer = 0;
        closeWindows();
        debugPrint(AUTO_CLOSE);
      }
    }
  }
}

void updateTaskTimers() {
  if (wateringTask == ONGOING && wateringTimer >= WATER_TIME * 60000) {
    wateringTask = DONE;
    wateringTimer = 0;
    water = 0;
    waterOff();
    debugPrint(AUTO_WATER_OFF);
  }
  else if (wateringTask == DONE && wateringTimer >= WATER_TIME * 60000) {
    wateringTask = NOT_ASSIGNED;
    wateringTimer = 0;
  }
  
  if (aerationTask == ONGOING && aerationTimer >= FAN_TIME * 60000) {
    aerationTask = DONE;
    aerationTimer = 0;
    fan = 0;
    fanOff();
    debugPrint(AUTO_AIR_OFF);
  }
  else if (aerationTask == DONE && aerationTimer >= MONITOR_TIME * 60000) {
    aerationTask = NOT_ASSIGNED;
    aerationTimer = 0;
  }
  
  if (windowsTask == ONGOING && windowsTimer >= MOTOR_TIME * 1000) {
    windowsTask = DONE; 
    windowsTimer = 0;
  }
  else if (windowsTask == DONE && windowsTimer >= MONITOR_TIME * 60000) {
    windowsTask = NOT_ASSIGNED;
    windowsTimer = 0;
  }
}

bool isEveryThingDone() {
  // Arrosage et aération ne sont pas en cours, delay de mise en veille dépassé, mouvement des servos finis
  return ((wateringTask != ONGOING) && (aerationTask != ONGOING) && (standbyTimer >= ON_TIME * 60000) && servoA.isFinished() && servoB.isFinished());
}

//----------------------------------------------------------------------------------------
// TAKE A NAP AFTER WORK
//----------------------------------------------------------------------------------------

long sleepTime ;

void updateDeepSleep() {
  if (isEveryThingDone() && gset.sleepTime > 0) {
    sleepTime = nextStart - millis()/1000;
    if (sleepTime < 0) {
      updateNTP(false);
      sleepTime = nextStart - millis()/1000;
    }
    debugPrint(DEEP_SLEEP);
    changeLight(LIGHT_OFF);
    digitalWrite(LED_BUILTIN, HIGH);
    WiFi.disconnect();
    SPIFFS.end();
    ESP.deepSleep(sleepTime * 1000000);
  }
}
