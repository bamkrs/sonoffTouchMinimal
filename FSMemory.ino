/**
 * FSMemory.ino
 * 
 * Created on: 15.08.2018
 *     Author: Benedikt-Alexander Mokroß (benedikt@bhorn.de)
 */

#include <FS.h>
#include "globals.h"
#include <math.h>

inline String getConfigFilename(String key)
{
  return "pcfg_"+key;
}

String getConfigFS(String key)
{
  String filename = getConfigFilename(key);
  if(!SPIFFS.exists(filename)){
    return "";
  }
  File f = SPIFFS.open(filename, "r");
  if (!f) {
    return "";
  }
  char cfgbuf[f.size()+1];
  f.readBytes(cfgbuf,f.size());
  cfgbuf[f.size()] = 0;
  f.close();
  return String(cfgbuf);
}

boolean setConfigFS(String key, String value)
{
  String filename = getConfigFilename(key);
  File f = SPIFFS.open(filename,"w");
  if(!f){
    return false;
  }
  f.print(value);
  f.close();
  SPIFFS.end();
  return true;
}

