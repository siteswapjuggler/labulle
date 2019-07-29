//----------------------------------------------------------------------------------------
// LIBRARIES
//----------------------------------------------------------------------------------------

#include <FS.h>                       // SPIFFS management
#include <EEPROM.h>                   // 1024 byte EEPROM Emulation for ESP8266
#include <ESP8266WiFi.h>              // ESP8266 WiFi Library
#include <ESP8266WiFiMulti.h>         // Wifi Multi Library
#include <DNSServer.h>                // DNS Server management
#include <ESP8266mDNS.h>              // mDNS Hostname management
#include <ESP8266WebServer.h>         // WebServer for configuration and running mode
#include <WebSocketsServer.h>         // Websocket Library
#include <WiFiClientSecure.h>         // Webhook

#include <WiFiUdp.h>                  // Udp Library (NTP dependency)
#include <NTPClient.h>                // Network Time Protocol
#include <Timezone.h>                 // Timezone Management

#include <Ramp.h>                     // Servo interpolation
#include <Bounce2.h>                  // Bounce management
#include <togedge.h>                  // Toggle edge detection
#include <elapsedMillis.h>            // Timers for parallel actions management

#include <DHT.h>                      // Humidity & Temperature
#include <Wire.h>                     // I2C Library
#include <Adafruit_PWMServoDriver.h>  // PWM breakboard
#include <Adafruit_NeoPixel.h>        // Neopixel Lights

//----------------------------------------------------------------------------------------
// PIN CONFIGURATION
//----------------------------------------------------------------------------------------

#define DHT_PIN       D3
#define WATER_PIN     D6
#define FAN_PIN       D5
#define TANK_PIN      D7

#define SERVOA_CLOSE  190
#define SERVOA_OPEN   270
#define SERVOA_PIN    0
#define SERVOB_OPEN   165
#define SERVOB_CLOSE  245
#define SERVOB_PIN    1


#define NEOPIXEL_PIN  D8
#define NEOPIXEL_NUM  24
#define LIGHT_ON      0x404040
#define LIGHT_OFF     0x000000

#define SOIL_PIN      A0

#define DHT_PIN       D3
#define DHT_TYPE      DHT22

#define BLINK_DELAY   1000
#define SENSOR_DELAY  2000

//----------------------------------------------------------------------------------------
// ENUMERATIONS
//----------------------------------------------------------------------------------------

enum debug_msg {
  EEPROM_INIT, WIFI_SCAN, WIFI_OUT, WIFI_TRY, WIFI_WAIT, WIFI_ENDWAIT, WIFI_FAIL,
  WIFI_SUCCESS, WIFI_SETTINGS, AP_CONFIG, DNS_INIT, MDNS_INIT, SERVER_INIT, REAL_TIME,
  WS_INTI, WS_CONNECT, WS_DISCONNECT, WS_TEMP_TH, WS_SOIL_TH, WS_START_TIME,
  WS_END_TIME, WS_SENSOR_UPDATE, WS_INIT, DEVICE_READY, WS_TOGGLE_WATER, WS_TOGGLE_SERVO,
  WS_TOGGLE_FAN, WS_TOGGLE_LIGHT, WS_SLEEP_TIME, GS_SAVE, HTTPS_CONNECT, HTTPS_FAIL,
  FP_OK, FP_KO, IFTTT_OK, SERVO_POS, DEEP_SLEEP, AUTO_WATER_ON, AUTO_WATER_OFF, 
  AUTO_AIR_ON, AUTO_AIR_OFF, AUTO_OPEN, AUTO_CLOSE
};

// Arduino IDE workaround prototype is needed with enum
void debugPrint(debug_msg msg);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

//----------------------------------------------------------------------------------------
// GLOBAL STRUCTURES
//----------------------------------------------------------------------------------------

#define PARAMETER_LENGTH  64

struct _stateSettings {
  togedge servo;
  togedge tankAlarm;
};

struct _wifiSettings {
  char ssid[PARAMETER_LENGTH];
  char pwd[PARAMETER_LENGTH];
};

struct _generalSettings {
  float soilThreshold;
  float tempThreshold;
  float startTime;
  float endTime;
  float sleepTime;
};

//----------------------------------------------------------------------------------------
// TASK MANAGEMENT
//----------------------------------------------------------------------------------------

enum taskState {
  ONGOING, DONE, OVERRIDE, NOT_ASSIGNED
};
