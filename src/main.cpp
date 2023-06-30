#include <Arduino.h>
#include <sunset.h>
#include <WiFi.h>
#include <time.h>

/* Setup your wifi setings here */
#define WIFI_SSID "YOUR_SSID_HERE"
#define WIFI_PASS "YOUR_PASS_HERE"
#define NTP_SERVER "pool.ntp.org"

/* This is set to Glen Cove NY */
#define LATITUDE        40.867779
#define LONGITUDE       -73.639763
#define DST_OFFSET      -4

SunSet sun;

void WiFiSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.printf("\nConnected with IP: %s\n",WiFi.localIP().toString());
}

bool SetClockFromNTP() {
  Serial.println("Attempting to Set Time");
  configTime(DST_OFFSET * 60 * 60, 0, NTP_SERVER);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("ERROR: Failed to obtain time");
    return false;
  }
  Serial.println(&timeinfo, "Time Set To: %A, %B %d %Y %H:%M:%S");
  return true;
}

void setup()
{
  // Start up Serial
  Serial.begin(115200);

  // Setup WiFi to connect to station
  WiFiSetup();

  // Set the RTC From NTP Server
  bool bClockSet = false;
  while (!bClockSet) { // Keep Trying to Set the clock from NTP until it's succesfull
    bClockSet = SetClockFromNTP();
  }

  sun.setPosition(LATITUDE, LONGITUDE, DST_OFFSET);

  static int currentDay = 32;
  int sunrise;
  int sunset;
  
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  sun.setCurrentDate(timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday);

  sunrise = static_cast<int>(sun.calcSunrise());
  sunset = static_cast<int>(sun.calcSunset());
  Serial.printf("Sunrise at %d:%dam, Sunset at %d:%dpm\n", (sunrise/60), (sunrise%60), (sunset/60), (sunset%60));
}

void loop()
{
  // Here you will do things based on sunset and sunrise
    
}