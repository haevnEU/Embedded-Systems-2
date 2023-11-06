#ifndef _XILAB_GPS_H_
#define _XILAB_GPS_H_

#include <TinyGPS.h>
#include <SPI.h>
#include<Arduino.h>

namespace xilab{

    enum GPS_Results{
        OK = 400,
        NOT_INITIALZED = 401,
        LOCATED = 402,
        NOT_YET_ALLOWED = 403
    };
    class GPS{
        public:
            static GPS& getInstance(){
                static GPS instance;
                return instance;
            }
        private:
            GPS() : Serial1(1){}

            bool initialized_m = false;

            float latitude_m;
            float longitude_m;

            long lastLocation = 0;
            long locationOffset = 500;

            HardwareSerial Serial1;
            TinyGPS gps;

        public:

            bool isInitialized(){
                return initialized_m;
            }

            GPS_Results init(){
                Serial1.begin(9600,SERIAL_8N1,14,13);

                initialized_m = true;
                return GPS_Results::OK;
            } 

            GPS_Results locate(float& longitude, float& latitude){
                if(!isLocationAllowed()){
                    return NOT_YET_ALLOWED;
                }
                while (Serial1.available()){
                    gps.encode(Serial1.read());
                }

                  gps.f_get_position(&latitude_m, &longitude_m);
                    longitude = longitude_m;
                    latitude = latitude_m;
                lastLocation = millis();
                
                LOGn("============================================");
                LOGn("[GPS       ] %f %f", latitude_m, longitude_m);
                LOGn("============================================");
                return GPS_Results::LOCATED;
            }

            bool isLocationAllowed(){
                return (lastLocation + locationOffset) > millis();
            }

            float getLongitude(){
                return longitude_m;
            }

            float getLatitude(){
                return latitude_m;
            }
    };
} // namespace xilab

#endif // _XILAB_GPS_H_
