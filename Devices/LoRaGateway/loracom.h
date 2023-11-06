#ifndef LORA_COM_H_
#define LORA_COM_H_

#include "utils.h"
#include <SPI.h>
#include <LoRa.h>
#include "configuration.h"


typedef void (*lora_callback)(String data);

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
                    lora_callback lora_callback_s = nullptr;

                public:
                    loraResults init(lora_callback callback){
                        LOGn("[LORA MODULE ] Initialize lora module.");
                        if(nullptr == callback){
                            LOGn("[LORA MODULE ] A nullptr was provided as callback");
                            return loraResults::NOT_INITIALIZED;
                        }
                        lora_callback_s = callback;
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
            
                    loraResults poll(){
                        if(!initialized){
                            return loraResults::NOT_INITIALIZED;
                        }
                        int packetSize = LoRa.parsePacket();
                        if (packetSize) {  
                            String received = "";
                            while (LoRa.available()) {
                                received += (char)LoRa.read();
                            }
                            if(nullptr != lora_callback_s){
                                lora_callback_s(received);
                            }
                        }

                        return loraResults::OK;
                    }
            };
        }
    }
}


#endif // LORA_COM_H_