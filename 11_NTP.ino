//----------------------------------------------------------------------------------------
// NETWORK TIME PROTOCOL
//----------------------------------------------------------------------------------------

#define ONE_MINUTE 60
#define ONE_HOUR 3600
#define ONE_DAY 86400

WiFiUDP   ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

//Central European Timezone (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};
TimeChangeRule CET  = {"CET", Last, Sun, Oct, 3, 60};
Timezone       CE(CEST, CET);

bool ST, WT, TST, TWT;
unsigned long startTime, nextStart;

void updateNTP(bool p) {
  if (hasWifi) {
    timeClient.begin();
    timeClient.update();

    startTime = millis() / 1000;

    ST  = CE.utcIsDST(timeClient.getEpochTime());
    TST = CE.utcIsDST(timeClient.getEpochTime() + ONE_DAY);
    WT  = !ST;
    TWT = !TST;

    if (ST) timeClient.setTimeOffset(7200);
    else    timeClient.setTimeOffset(3600);

    if (timeClient.getHours() + gset.sleepTime <= gset.endTime)
      nextStart = startTime + gset.sleepTime * ONE_HOUR;
    else {
      nextStart = startTime + (24 + gset.startTime - timeClient.getHours()) * ONE_HOUR - (timeClient.getMinutes() * ONE_MINUTE) - timeClient.getSeconds();
      if (ST && TWT)
        nextStart += ONE_HOUR;
      if (WT && TST)
        nextStart -= ONE_HOUR;
    }
    timeClient.end();
    if (p) debugPrint(REAL_TIME);
  }
  else {
    startTime = millis() / 1000;
    nextStart = startTime + (gset.sleepTime * ONE_HOUR);
  }
}
