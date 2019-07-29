//----------------------------------------------------------------------------------------
// CONNECTED DEVICES
//----------------------------------------------------------------------------------------

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_NUM, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_PWMServoDriver pwm   = Adafruit_PWMServoDriver();

rampInt servoA;
rampInt servoB;

Bounce tank;                                  // Tank debounce
bool blinkState;
float temp, hygro, soil;                      // Sensors Value
togedge light, fan, water;                    // Toggle Value


elapsedMillis blinkTimer = BLINK_DELAY;
elapsedMillis sensorTimer = SENSOR_DELAY;     // Slowing down sensor acquisition

//----------------------------------------------------------------------------------------
// SETUP FUNCTION
//----------------------------------------------------------------------------------------

void initDevice() {
  // DHT
  dht.begin();

  // Neopixels
  strip.begin();
  strip.show();

  // Servo Motors
  pwm.begin();
  pwm.setPWMFreq(60);
  servoLastPosition();

  // Extender Configuration
  pinMode(WATER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TANK_PIN, INPUT_PULLUP);
  tank.attach(TANK_PIN);
  tank.interval(250);

  waterOff();
  fanOff();
  updateSensor();
}

//----------------------------------------------------------------------------------------
// SETUP FUNCTION
//----------------------------------------------------------------------------------------

void updateBlink() {
  if (blinkTimer >= BLINK_DELAY) {
    blinkState = blinkState ? false : true;
    digitalWrite(LED_BUILTIN, blinkState);

    uint32_t c = LIGHT_OFF;
    if (blinkState) c = LIGHT_BLINK;
    else if (light) c = LIGHT_ON;
    for (int i = 0; i < 4; i++) {
      strip.setPixelColor(i * 6, c);
    }
    strip.show();
    blinkTimer = 0;
  }
}

void blinkOff() {
  digitalWrite(LED_BUILTIN, HIGH);
}

//----------------------------------------------------------------------------------------
// GENERAL SENSORS
//----------------------------------------------------------------------------------------

void updateSensor() {
  //Gestion du réservoir
  tank.update();
  if (tank.fell()) {
    sendTrigger();
  }
  else if (tank.rose() && sset.tankAlarm == true) {
    sset.tankAlarm = false;
    saveStateSettings();
  }

  //Mise à jour des capteurs
  if (sensorTimer >= SENSOR_DELAY) {
    int sensor = analogRead(SOIL_PIN);
    soil = constrain(sensor, 35, 725);
    soil = 100.*(soil - 35.) / 725.;
    temp  = dht.readTemperature();
    hygro = dht.readHumidity();
    sensorTimer = 0;
  }
}

//----------------------------------------------------------------------------------------
// LIGHT FUNCTION
//----------------------------------------------------------------------------------------

void lightOn() {
  changeLight(LIGHT_ON);
}

void lightOff() {
  changeLight(LIGHT_OFF);
}

void changeLight(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

//----------------------------------------------------------------------------------------
// RELAY FUNCTION
//----------------------------------------------------------------------------------------

void waterOn() {
  digitalWrite(WATER_PIN, LOW);
}

void waterOff() {
  digitalWrite(WATER_PIN, HIGH);
}

void fanOn() {
  digitalWrite(FAN_PIN, LOW);
}

void fanOff() {
  digitalWrite(FAN_PIN, HIGH);
}

//----------------------------------------------------------------------------------------
// SERVO FUNCTION
//----------------------------------------------------------------------------------------

bool servoSleep;

void attachServos() {
  if (servoSleep) {
    pwm.wakeup();
    servoSleep = false;
  }
}

void detachServos() {
  if (!servoSleep) {
    pwm.sleep();
    servoSleep = true;
  }
}

void servoLastPosition() {
  attachServos();
  servoA.go(sset.servo ? SERVOA_OPEN : SERVOA_CLOSE);
  servoB.go(sset.servo ? SERVOB_OPEN : SERVOB_CLOSE);
  servoA.go(sset.servo ? SERVOA_OPEN : SERVOA_CLOSE, MOTOR_TIME * 1000);
  servoB.go(sset.servo ? SERVOB_OPEN : SERVOB_CLOSE, MOTOR_TIME * 1000);
  debugPrint(SERVO_POS);

}

void openWindows() {
  attachServos();
  saveStateSettings();
  servoA.go(SERVOA_OPEN, MOTOR_TIME * 1000);
  servoB.go(SERVOB_OPEN, MOTOR_TIME * 1000);
}

void closeWindows() {
  attachServos();
  saveStateSettings();
  servoA.go(SERVOA_CLOSE, MOTOR_TIME * 1000);
  servoB.go(SERVOB_CLOSE, MOTOR_TIME * 1000);
}

void updateServos() {
  if (servoA.isRunning() || servoB.isRunning()) {
    pwm.setPWM(SERVOA_PIN, 0, servoA.update());
    pwm.setPWM(SERVOB_PIN, 0, servoB.update());
  }
  else {
    detachServos();
  }
}
