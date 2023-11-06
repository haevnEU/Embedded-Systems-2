
#define DEBUG
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "backendglue.h"
#include "loracom.h"

xilab::network::http::BackendGlue backend;
xilab::network::lora::LoRaNetwork network = xilab::network::lora::LoRaNetwork::getInstance();

void LOGn(const char *fmt, ...);

void lora_data_received(String data){
    String json(data);
    int result = xilab::utils::JsonUtil::getInstance().parse(data);  
    if(result == xilab::utils::JsonResults::OK){
      int charge = xilab::utils::JsonUtil::getInstance().extractInteger(data, "device", "charge");
      String uuid = xilab::utils::JsonUtil::getInstance().extractString(data, "device", "UUID");
      int value = xilab::utils::JsonUtil::getInstance().extractInteger(data, "water", "current");
      float longitude = xilab::utils::JsonUtil::getInstance().extractFloat(data, "coordinates", "longitude");
      float latitude = xilab::utils::JsonUtil::getInstance().extractFloat(data, "coordinates", "latitude");
      backend.send(uuid, value, charge, longitude, latitude);
    }else{
      LOGn("[ROOT        ](data rec) Parsing yields %i for document \"%s\"", result, json);
    }
}
void setup() {
  Serial.begin(115200);
  LOGn("[SETUP       ](setup  ) Start setup.");
  backend.init();
  network.init(&lora_data_received);
  LOGn("[SETUP       ](setup  ) Setup done.");
}

void loop(){
  network.poll();
}
