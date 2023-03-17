#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <time.h>                  

/* Configuration of NTP */
#define MY_NTP_SERVER "ch.pool.ntp.org"           
#define MY_TZ "CET-1CEST,M3.5.0,M10.5.0/03"
time_t now;  // this is the epoch
tm tm;       // the structure tm holds time information 

#define PIN        4 
#define NUMPIXELS 18 

#define OpenDrain // use open drain on output pin
//#define EasyVariant // always refresh all led

uint8_t pixels[3*NUMPIXELS];
extern "C" IRAM_ATTR void espShow(uint16_t pin, uint8_t *pixels, uint32_t numBytes, uint8_t type);

const char *ssid = "FSM";
const char *password = "0101010101";


uint32_t sntp_update_delay_MS_rfc_not_less_than_15000 ()
{
  Serial.println("sntp_update_delay_MS");
  return 12 * 60 * 60 * 1000UL; // 12 hours
}

void setup() 
{
  Serial.begin(115200);
  Serial.println("Start");
  Serial.flush();
  delay(500);

  #if defined OpenDrain 
    pinMode(PIN, OUTPUT_OPEN_DRAIN); 
  #else                 
    pinMode(PIN, OUTPUT); 
  #endif
  for (int i=0;i<3*NUMPIXELS;i++) pixels[i] = 1;
  delay(100); // prevent espshow() glitches
  espShow(PIN,pixels,3*NUMPIXELS,true);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected to WiFi");
  Serial.println(WiFi.localIP());

  configTime(MY_TZ, MY_NTP_SERVER); 
}

void fillpix(int pnum, uint8_t bin, bool reverse=false)
{
  for (int i=0;i<18;i++) pixels[pnum+i] = 0;
  
  if (reverse)
  {
    if (bin & (1 << 0)) pixels[pnum+15] = 20;
    if (bin & (1 << 1)) pixels[pnum+12] = 20;
    if (bin & (1 << 2)) pixels[pnum+9]  = 20;
    if (bin & (1 << 3)) pixels[pnum+6]  = 20;
    if (bin & (1 << 4)) pixels[pnum+3]  = 20;
    if (bin & (1 << 5)) pixels[pnum]    = 20; 
  }
  else
  {
    if (bin & (1 << 0)) pixels[pnum]    = 20;
    if (bin & (1 << 1)) pixels[pnum+3]  = 20;
    if (bin & (1 << 2)) pixels[pnum+6]  = 20;
    if (bin & (1 << 3)) pixels[pnum+9]  = 20;
    if (bin & (1 << 4)) pixels[pnum+12] = 20;
    if (bin & (1 << 5)) pixels[pnum+15] = 20; 
  }
}

uint8_t oldhour = 100;
uint8_t oldmin = 100;

void loop() 
{
  uint8_t newhour;
  uint8_t newmin;
  uint8_t newsec;
  bool pixupdate = false;
  
  time(&now);
  localtime_r(&now, &tm);

  newsec = (uint8_t)tm.tm_sec;
  newmin = (uint8_t)tm.tm_min;
  newhour = (uint8_t)tm.tm_hour;

#if defined EasyVariant
  // easy variant
  fillpix(0,newsec);
  fillpix(18,newmin,true);
  fillpix(36,newhour);
  delay(100); // prevent espshow() glitches
  noInterrupts();
  espShow(PIN,pixels,NUMPIXELS*3,true);
  interrupts();
#else 
  // complicated variant
  fillpix(0,newsec);
  if (newmin != oldmin)
  {
    oldmin = newmin; 
    fillpix(18,newmin,true);
    fillpix(36,newhour);
    delay(100); // prevent espshow() glitches
    noInterrupts();
    espShow(PIN,pixels,NUMPIXELS*3,true);
    interrupts();
  }
  else
  {
    delay(100); // prevent espshow() glitches
    espShow(PIN,pixels,NUMPIXELS,true);
  }
#endif
 
  delay(900);
}
