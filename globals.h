/**
 * globals.h
 * 
 * Created on: 15.08.2018
 *     Author: Benedikt-Alexander Mokro?? (benedikt@bhorn.de)
 */

#ifndef GLOBALS_H
#define GLOBALS_H
#include <WiFiManager.h>

/**
 * WiFi Config
 */
#define AP_PASSWORD "Antwort42"

/**
 * Misc 
 */
#define UTC_TIMEZONE 2
#define DEFAULT_NAME_SWITCHNAME "Lichtschalter"

/**
 * Don't touch anything below this line!
 */
#define CODE_VERSION "v1.0"

struct configPortalHolder {
  WiFiManagerParameter* switchName;
  WiFiManagerParameter* ledMode;
} configParameter;


String getChipIdHex()
{
  return WiFi.macAddress().substring(9,11)+  // xx:xx:xx:XX:xx:xx
         WiFi.macAddress().substring(12,14)+ // xx:xx:xx:xx:XX:xx
         WiFi.macAddress().substring(15,17); // xx:xx:xx:xx:xx:XX
}

String getChipShortIdHex()
{
  return WiFi.macAddress().substring(15,17); // xx:xx:xx:xx:xx:XX
}

#endif

