
// TODO ADD GPS AND WATER MEASUREMENT

#define DEBUG
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "watermeasurement.h"
#include "loracom.h"
#include "xilabgps.h"

#define BATTERY_PIN 34
#define MAX_VOLTAGE (4.2)

// The following definitions are used as QOL to create
// a better interaction with time values
#define SECOND (1000)
#define MINUTE (60 * SECOND)
#define HOUR (60 * MINUTE)
#define DAY (24 * HOUR)

// Controls when the device should measure and notify the server
// For production remove line 24 - 25 and uncomment line 26
//#define ACTIVITY_THRESHOLD (5 * MINUTE) 
#define ACTIVITY_THRESHOLD (500)


typedef struct Dataset_t{
    /// UUID of the device
	const char* uuid;

    /// Current battery voltage in percent
    unsigned short battery;

    /// Longitude of the location
    float longitude;

    /// Latitude of the location
    float latitude;

    WaterSensorData water_data;
} Dataset;

Dataset data;
xilab::GPS gps = xilab::GPS::getInstance();

xilab::network::lora::LoRaNetwork network = xilab::network::lora::LoRaNetwork::getInstance();

void setup() { 

    data.battery = 0;
    data.uuid = "";
    data.latitude = 0;
    data.longitude = 0;
    data.water_data.max = 0;
    data.water_data.min = 0;
    data.water_data.current = 0;

    Serial.begin(115200);

    network.init();
    gps.init(); 
    initialize_water_sensor_module();
}

/**
 * @brief Measures the voltage supplied by the voltage divider.
 * After the measurement the measured value is mapped to the battery capacity
 * Finally the value is transformed into a percentage space and returned.
 * 
 */
int measure_battery(){
    LOGn("[BATTERY] Read battery");
    float current_battery_voltage = analogRead(BATTERY_PIN);
    float mapped_voltage = map(current_battery_voltage, 0, 4096, 0, 210);
    float actual_volt = (mapped_voltage / 100) * 2;
    float voltagePercentage = (actual_volt / MAX_VOLTAGE) * 100; 
    
    return voltagePercentage;  
}

/**
 * @brief Sends a json document to the LoRa receiver.
 * The JSON object contains of the following elements
 * - device: Device information
 *      - name: Name of the device
 *      - id: UUID of the device
 *      - battery: current battery value
 * - water: Information about the water value
 *      - min: Minimum of water 
 *      - max: Maximum water capacity
 *      - current: Current water level 
 * - location: GPS location of this device
 *      - long: Longitude
 *      - lat: Latitude
 */
void send_data(){
    LOGn("[NOTIFY] Sending data to master device");
    char buffer[255] = {0};
                      
    sprintf(buffer, "{\"device\":{\"UUID\":\"%s\",\"charge\":%d},\"water\":{\"current\":%d},\"location\":{\"longitude\":%ld,\"longitude\":%ld}}",
    data.uuid, data.battery, data.water_data.current,
    data.longitude, data.latitude);
    network.send(String(buffer));
}

/**
 * @brief Broadcast an error code
 *  
 * @param code Code associated with the message, ranging from 0 to 255.
 */
void send_code(int code){
    char buffer[255] = {0};
    sprintf(buffer, "{\"error_code\":%i,\"device\":{\"UUID\":\"%s\",\"charge\":%d},\"location\":{\"longitude\":%ld,\"latitude\":%ld}}",
    code,
    data.uuid, data.battery,
    data.longitude, data.latitude);
    network.send(buffer);
}

void loop() {
    
   // data.battery = measure_battery();
    data.water_data.current = read_water_level();
    gps.locate(data.longitude, data.latitude);
    send_data();
    
    // To ensure an under-voltage protection the esp will go into the deepsleep
    // The master device is notified 10 times with an urgent message
    if(false && data.battery  <= 3.3){  
        data.battery = 0;
        for(int i = 0; i < 10; ++i){
            send_code(0xDE);
            delay(1000);
        }
        //esp_deep_sleep_start();
    }
    delay(ACTIVITY_THRESHOLD);   
    
}