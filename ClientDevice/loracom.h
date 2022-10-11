#ifndef  _XILAB_LORA_COM_H_
#define  _XILAB_LORA_COM_H_
#include <SPI.h>
#include <LoRa.h>

#include "utils.h"
#include "configuration.h"


namespace xilab{
    namespace network{
        namespace lora{
            enum loraResults{
                OK = 0,
                NOT_INITIALIZED = 201,
            };

            class LoRaNetwork{
                public:
                    static LoRaNetwork& getInstance(){
                        static LoRaNetwork instance;
                        return instance;
                    }
                private:
                    bool initialized = false;
                public:
                    loraResults init(){
                        LOGn("[LORA MODULE ] Initialize lora module.");
                        SPI.begin(5, 19, 27, 18);
                        LoRa.setPins(configuration::lora::PIN_SS, configuration::lora::PIN_RST, configuration::lora::PIN_DIO0);
                    
                        bool loraStarted = false;
                        for(int i = 0; i < configuration::retries::LORA && !loraStarted; i++){
                            delay(500);
                            loraStarted = LoRa.begin(configuration::lora::REGION);   
                        }
                        
                        if(!loraStarted){
                            LOGn("[LORA MODULE ] Lora module not initialized.");
                            return loraResults::NOT_INITIALIZED;
                        }
                        
                        LOGn("[LORA MODULE ] Lora module initialized.");
                        initialized = true;
                        return  loraResults::OK;
                    }

                  void send(String data){
                        LOGn("Sending %s", data.c_str());
                        LoRa.beginPacket();
                        LoRa.print(data);
                        LoRa.endPacket();
                  }
              };
        } // namespace lora
    } // namespace network
} // namespace xilab




#endif //  _XILAB_LORA_COM_H_