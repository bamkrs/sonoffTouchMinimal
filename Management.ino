/**
 * Management.ino
 * 
 * Created on: 15.08.2018
 *     Author: Benedikt-Alexander Mokroß (benedikt@bhorn.de)
 */

#include <FS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "globals.h"

const char* switchNameAnnotation = "Name of switch";
const char* switchNameParameter = "PARAMETER_NAME_SWITCHNAME";
const char* ledModeAnnotation = "Led Mode";
const char* ledModeParameter = "PARAMETER_NAME_LEDMODE";

bool wifiManagerSetup()
{  
  if(!SPIFFS.begin()){
    return false;
  }
  String nameSwitchNameStr = getConfigFS("sw");
  String defaultNameSwitchNameStr = nameSwitchNameStr;
  defaultNameSwitchNameStr += getChipShortIdHex();
  if(nameSwitchNameStr.length() > 0)
    configParameter.switchName = new WiFiManagerParameter(switchNameParameter, switchNameAnnotation, nameSwitchNameStr.c_str(), 64);
  else
    configParameter.switchName = new WiFiManagerParameter(switchNameParameter, switchNameAnnotation, defaultNameSwitchNameStr.c_str(), 64);

  String ledModeStr = getConfigFS("lm");
  if(ledModeStr.length() > 0)
    configParameter.ledMode = new WiFiManagerParameter(ledModeParameter, ledModeAnnotation, ledModeStr.c_str(), 8);
  else
    configParameter.ledMode = new WiFiManagerParameter(ledModeParameter, ledModeAnnotation, "WiFi", 8);
  SPIFFS.end();
  return true;
}

bool wifiManagerPersist()
{
  if(!SPIFFS.begin()){
    return false;
  }
  bool p1 = setConfigFS("sw", configParameter.switchName->getValue());
  bool p2 = setConfigFS("lm", configParameter.ledMode->getValue());
  SPIFFS.end();
  return p1 && p2;
}

void wifiManagerTellSettings()
{
  Serial.print(F("Switch Name:\t\t"));
  Serial.println(configParameter.switchName->getValue());

  Serial.print(F("Led Mode:\t\t"));
  Serial.println(configParameter.ledMode->getValue());
}

bool wifiManager()
{
  WiFiManager wifiManager;
  wifiManagerSetup();
  
  WiFiManagerParameter custom_text_p1(("<p>Switch Name"));
  wifiManager.addParameter(&custom_text_p1);
  wifiManager.addParameter(configParameter.switchName);

  WiFiManagerParameter custom_text_p2(("</p><p>Led Mode ('Normal' = WiFi-Led on if relais on,'Inv' = WiFi-Led on if relais off, 'WiFi' = WiFi-Led blinks if no WiFi, 'Off', 'On')"));
  wifiManager.addParameter(&custom_text_p2);
  wifiManager.addParameter(configParameter.ledMode);

  WiFiManagerParameter custom_text_end(("</p>"));
  wifiManager.addParameter(&custom_text_end);
  
  String apid = String("Switch_")+getChipIdHex();
  wifiManager.autoConnect(apid.c_str(),AP_PASSWORD);

  wifiManagerPersist();

  wifiManagerTellSettings();
  return true;
}

void wifiConnect()
{  
  Serial.print("Connecting to AP");
  if(wifiManager())
    Serial.println("WiFi connected");  
}


