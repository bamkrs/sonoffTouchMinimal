# sonoffTouchMinimal
Minimal firmware for the Sonoff Touch Switches with WeMo. Escape the cloud, no app needed, no home calling.

## Dependencies

* [ESP8266 Arduino](https://github.com/esp8266/Arduino) (GPL License)
* [WiFiManager](https://github.com/tzapu/WiFiManager) by tzapu (MIT License)
* [fauxmoesp](https://bitbucket.org/xoseperez/fauxmoesp) by Xose Pérez (MIT License)

## How It Works

Have a look at tzapu's [WiFiManager - How It Works](https://github.com/tzapu/WiFiManager#how-it-works) for the first startup.
You just have to enter your WiFi credentials, a name for the switch and the LED-Mode which is one of the following:

* 'Normal' - Normal Mode: WiFi-LED on if relais is on
* 'Inv' - Inverted Mode: WiFi-LED on if relais if off
* 'Off' - WiFi-LED always off
* 'On' - WiFi-LED always on
* 'WiFi' - WiFi-LED off, slow blinking (1s) if Wifi connection is lost. 

If the WiFi-LED blinks fast (250ms), I have bad news for you: your internal SPI-Flash seems to be broken. 

## How Do I Flash My Sonoff Touch?

Have a look at the [Sonoff-Tasmota projects Wiki on flashing the Sonoff Touch](https://github.com/arendst/Sonoff-Tasmota/wiki/Sonoff-Touch). The Tasmota project is a great resource for everything Sonoff! Please pay attention to the [Sonoff-Tasmota projects Hardware Preparation](https://github.com/arendst/Sonoff-Tasmota/wiki/Hardware-Preparation) and don't kill yourself by working on your touch while connected to AC!

FYI: GPIO-0 is connected to the unlabled IC to the left of the PSF-A/ESP8285 on the Tasmota images. You can easily attach a hook to the top left pin instead of fiddling around on the PSF-A/ESP8285. 

## Disclaimer
I'm definetly not responsible to any hardware damage or personal damage. Use at your own risk!