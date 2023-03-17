# BinClock
ESP8266 &amp; WS2812 &amp; NTP combined into a binary clock

You will need 3 stripes of 6 WS2812 LED to represent hours, minutes and seconds.
The WS2812 data input is connected to GPIO 4. Note the WS2812 are 5V while the esp8266 is 3.3V. 
Use the "#define OpenDrain" option and provide a pullup resistor, or run the ws2812 at 3.3V.
Or, on your own risk, just connect gpio4 to din and hope the world will not explode. 

Change the wifi credentials to your wifi. This:

const char *ssid = "FSM";
const char *password = "0101010101";

Change the ntp server to your location. This:

#define MY_NTP_SERVER "ch.pool.ntp.org"   

Change the ntp time zone and winter/summer time. This:

#define MY_TZ "CET-1CEST,M3.5.0,M10.5.0/03"

The ESP will sync to NTP every 12 hours and use the internal timer in-between.

