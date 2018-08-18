#include <FS.h>
#include <ESP8266WiFi.h>
#include <fauxmoESP.h>
#include "globals.h"

fauxmoESP fauxmo;

#define PIN_BUTTON 0
#define PIN_LED 13
#define PIN_RELAIS 12

enum LEDMODE {
  LED_NORMAL,
  LED_INV,
  LED_OFF,
  LED_ON,
  LED_WIFI,
};

int switchState = 0;
int ledMode = LED_NORMAL;
int ledState = 0;
int lastPinState = 0;
int stateWiFi = 0;
unsigned long timeCounter = 0;

void setSwitchState(int id, int state){
  Serial.print(F("Setting relais to "));
  Serial.println(state);
  digitalWrite(PIN_RELAIS, state);
  switchState = state;
  switch(ledMode){
    case LED_INV:
      // LED -> Active Low..?
      ledState = state == 1 ? 1 : 0;
      digitalWrite(PIN_LED, ledState); 
      break;
    case LED_NORMAL:
      ledState = state == 1 ? 0 : 1;
      digitalWrite(PIN_LED, ledState); 
      break;
    case LED_OFF:
      digitalWrite(PIN_LED, 0);
      break;
    case LED_ON:
      digitalWrite(PIN_LED, 1);
      break;
  }
}

void setup() {

  Serial.begin(74880);

  // Check if the SPI-FS is working
  if (SPIFFS.begin()) {
    SPIFFS.end();
  }
  else
  {
    // Cant mount memory. PANIC-Blinky...
    pinMode(PIN_LED, OUTPUT);
    bool ledOn = true;
    while(1)
    {
      digitalWrite(PIN_LED,ledOn);
      ledOn = !ledOn;
      delay(250);
    }
  }

  // SPI-FS good, print some usable data.
  Serial.println(F("Mounted FS"));
  Serial.println(CODE_VERSION);
  Serial.println(WiFi.macAddress());

  // Conenct to WiFi
  wifiConnect();

  // Set Pinmodes
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELAIS, OUTPUT);

  // Tell Fauxmo our configured Name.
  fauxmo.addDevice(configParameter.switchName->getValue());
  fauxmo.enable(true);

  // Set the LED-Mode
  if(strcmp(configParameter.ledMode->getValue(), "Inv") == 0){
    ledMode = LED_INV; // WiFi-LED on if relais off
    digitalWrite(PIN_LED, 1);
  } else if(strcmp(configParameter.ledMode->getValue(), "Off") == 0){
    ledMode = LED_OFF; // WiFi-LED always off
    digitalWrite(PIN_LED, 1); // WiFi-LED is active low
  } else if(strcmp(configParameter.ledMode->getValue(), "On") == 0){
    ledMode = LED_ON; // WiFi-LED always on
    digitalWrite(PIN_LED, 0); // WiFi-LED is active low
  } else if(strcmp(configParameter.ledMode->getValue(), "WiFi") == 0){
    ledMode = LED_WIFI; // WiFi-LED off, blinks when no WiFi
    digitalWrite(PIN_LED, 1);
  } else { // We regard every other input as "Normal", WiFi-LED on if relais on
    ledMode = LED_NORMAL;
    digitalWrite(PIN_LED, 1);
  }

  setSwitchState(0, 0);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state) {
    setSwitchState(device_id, state ? 1 : 0);
    switch(device_id) { // Not needed since there is only one WeMo device emulated. However, might be handy if further stuff should be done.
      case 0:
        return switchState = state;
    }
  });
  fauxmo.onGetState([](unsigned char device_id, const char * device_name) {
    switch(device_id) { // Not needed since there is only one WeMo device emulated. However, might be handy if further stuff should be done.
      case 0:
        return switchState == 1;
    }
    return false;
  });
}


void loop() {
    fauxmo.handle();
    // Poll the Button
    if(ledMode == LED_WIFI){
      stateWiFi = WiFi.status();
      if(stateWiFi != WL_CONNECTED){
        unsigned long currMillis = millis();
        if(currMillis - timeCounter > 1000){
          ledState = ledState == 1 ? 0 : 1;
          timeCounter = millis();  
        }
      }
    }
    int pinState = digitalRead(PIN_BUTTON);
    if(lastPinState == 0 && pinState == 1){
      setSwitchState(0, switchState == 0 ? 1 : 0);
    }
    lastPinState = pinState;
}
