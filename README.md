# BinClock
ESP8266 &amp; WS2812 &amp; NTP combined into a binary clock

You will need 3 stripes of 6 WS2812 LED to represent hours, minutes and seconds.
The WS2812 data input is connected to GPIO 4. Note the WS2812 are 5V while the esp8266 is 3.3V. 
Do not remove the "#define OpenDrain" and provide a pullup resistor.
