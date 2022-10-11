#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// API for the VL53L0X sensor
#include <Adafruit_VL53L0X.h> 
// Used for LoRa
#include <SPI.h> 
#include <LoRa.h>

// Different values for each LoRa region
#define LORA_ASIA 433E6
#define LORA_EUROPE 866E6
#define LORA_NA 915E6

// LoRa region of this device, use one from above
#define LORA_REGION LORA_EUROPE

// LoRa Sync word for this device
#define LORA_SYNC_WORD 0xF3

// LoRa Pin layout
#define LORA_PIN_SS 5
#define LORA_PIN_RST 14
#define LORA_PIN_DIO 2

// The following definitions are used as QOL to create
// a better interaction with time values
#define SECOND (1000)
#define MINUTE (60 * SECOND)
#define HOUR (60 * MINUTE)
#define DAY (24 * HOUR)

#define SEND_INTERVAL (5 * MINUTE)

// Controls when the device should measure and notify the server
// For production remove line 42 and 44 uncomment line 43
//#define ACTIVIY_THRESHOLD (5 * MINUTE) 
#define ACTIVIY_THRESHOLD (500)

// This definition defines the total amount of measurements
// until the test collected a good amount of data
#define MAX_MEASURE (100)

typedef int boolean;
#define TRUE 0
#define FALSE 1

boolean lastSend = FALSE;
boolean dataChanged = FALSE;

/**
 * @brief This struct contains information about the watermeasurement
 */
typedef struct water_sensor_data_t{
    /// Maximum capacity
    int max;

    /// Minimum capactiy
    int min;

    /// Current value
    int current;
} water_sensor_data;

/**
 * @brief This struct contains information about the device
 */
typedef struct device_data_t{
    /// Name of the device
	const char* name;

    /// UUID of the device
	const char* id;

    /// Current battery voltage in percent
    unsigned short battery;
} device_data;

/**
 * @brief This sturct contains the location of the device
 */
typedef struct location_data_t{
    /// Longitude of the location
    long longitude;

    /// Langitude of the location
    long latitude;
}location_data;

water_sensor_data* water_data_m;
device_data* device_data_m;
location_data* location_data_m;

// Water sensor API
Adafruit_VL53L0X water_sensor_api_t = Adafruit_VL53L0X();


/// This attribute is used as a time threshold, details inside the main loop
unsigned long next_operation_m = -1;

/**
 * @brief This method is a printf style version of Serial.println
 */
void LOGn(char* format, ...){
    char buffer[1024];
    va_list pargs;
    va_start(pargs, format);
    vsnprintf(buffer, 1024, format, pargs);
    va_end(pargs);
    Serial.println(buffer);
}

/**
 * @brief Initializes the water measurement
 * 
 * @param retries Amount of retries until the initializiation fails, default 10
 * @return When the component is initialized true will be returned.
 */
bool initializeWaterSensor(int retries = 10){
    LOGn("[VL53L0X] Initialize sensor");
    for(int i = 0; i < retries; ++i){
        LOGn("[VL53L0X] Current attempt: %i", i);
        if(water_sensor_api_t.begin()){
            water_sensor_api_t.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);
            LOGn("[VL53L0X] Successful initialized");
            return true;
        }
        delay(500);
    }
    LOGn("[VL53L0X] Cannot initialize sensor");
    return false;
}

/**
 * @brief Initializes the LoRa component
 * This method sets all lora pins and the syncword too. 
 * After setting all attributes the LoRa module is started. 
 * @param retries Amount of retries until the initializiation fails, default 10
 */
void initializeLoRa(int retries = 10){
    LOGn("[LoRa] Initialize module");
    LoRa.setPins(LORA_PIN_SS, LORA_PIN_RST, LORA_PIN_DIO);
    LoRa.setSyncWord(LORA_SYNC_WORD);

    LOGn("[LoRa] Lora Setup");
    LOGn("[LoRa] Pin SS:  %i", LORA_PIN_SS);
    LOGn("[LoRa] Pin RST: %i", LORA_PIN_RST);
    LOGn("[LoRa] Pin DI0: %i", LORA_PIN_DIO);
    LOGn("[LoRa] Region: %i", LORA_REGION);
    LOGn("[LoRa] Syncword: %i", LORA_SYNC_WORD);

    for(int i = 0; i < retries; ++i){
        LOGn("[LoRa] Current attempt: %i", i);
        if(LoRa.begin(LORA_REGION)){
            return;
        }
        delay(500);

    }
    
    LOGn("[LoRa] Cannot initialize LoRa");
}


void setup() { 
    // Setup variables
    water_data_m = (water_sensor_data*) malloc(sizeof(water_sensor_data));
    device_data_m = (device_data*) malloc(sizeof(device_data));
    location_data_m = (location_data*) malloc(sizeof(location_data));

    // Device name e.g. SouthWest001
    device_data_m->name = "Test Device 0001";
    // UUID e.g. 688b7fb2-92d1-49bb-93bc-2212979ad6ca
    device_data_m->id = "688b7fb2-92d1-49bb-93bc-2212979ad6ca";

    // GPS Coordinates
    location_data_m->latitude = 0;
    location_data_m->longitude = 0;

    // Waterlevel, e.g. watertank boundaries
    water_data_m->max = 0;
    water_data_m->min = 0;

    
    Serial.begin(115200);

    initializeWaterSensor();
    initializeLoRa();
}

/**
 * @brief Reads a water level and stores the value inside the water_data_m attribute
 * A range test is used for the measurement, the VL53L0X starts a ranging test.
 * After each test the value is added to a temporary variable. Before assigning
 * a value to the water_data_m attribute the temporary sum is divided by the 
 * amount of meassurements. 
 * NOTE 
 * - The MAX_MEASURE definition controlls how many meassurements are done.
 * - When an invalid sum is yield the VL530X will be reinitialized and the
 *      meassurement is done again.
 */
int readWaterLevel(){
    LOGn("[VL53L0X]Reading water level");
    int sum = 0;
    for(int i = 0; i < MAX_MEASURE; i++){
        VL53L0X_RangingMeasurementData_t measure;
        water_sensor_api_t.rangingTest(&measure, false); 
        if (measure.RangeStatus != 4) { 
            sum += measure.RangeMilliMeter;
        } 
        delay(1);
    }    

    if(sum == 0){
        LOGn("[VL53L0X] Reinitialize sensor");
        initializeWaterSensor();
        readWaterLevel(); 
    }
    
    return sum / MAX_MEASURE;
}

void calculateWaterLevel(int level){
    int min = water_data_m->min;
    int max = water_data_m->max;
// Add calculation

    int tmp = map(level, min, max, 0, 100);

    if(tmp != water_data_m->current){
        water_data_m->current = tmp;
        dataChanged = TRUE;
    }
}

/**
 * @brief Reads the battery voltage and stores the value inside the device_data_m attribute
 * 
 */
void readBattery(){

    int currentVoltageValue = analogRead(AKKU_PIN);
        
   // float volt = (akkuValue / (4096 / 4.2));// * 2;   // gemessenen Wert am Pin umrechnen in volt
    int mappedVoltage = map(currentVoltageValue, 0, 4096, 0, 210);
    float voltage = ((float)mappedVoltage / 100) * 2;
    int voltagePercentage = (voltage / 4.2) * 100;       // volt in % umrechnen
  
    if(voltagePercentage != device_data_m->battery){
        device_data_m->battery = voltagePercentage;  
        dataChanged = TRUE;
    }
    
    if(volt <= 3.3){                          
        esp_deep_sleep_start();        
    }
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
void notifyServer(){
    LOGn("[NOTIFY] Sending data to master device");
    char buffer[1024];
    sprintf(buffer, "{\"device\":{\"name\":\"%s\",\"id\":\"%s\",\"battery\":%d},\"water\":{\"max\":%d,\"min\":%d,\"current\":%d},\"location\":{\"long\":%ld,\"lat\":%ld}}",
    device_data_m->name, device_data_m->id, device_data_m->battery,
    water_data_m->max, water_data_m->min, water_data_m->current,
    location_data_m->longitude, location_data_m->latitude);

#ifdef LORA_ACTIVE    
    size_t bufferSize = strlen(buffer);
    LoRa.beginPacket();
    LoRa.print(buffer);
    LoRa.endPacket();
#else   
    LOGn(buffer);
#endif
}

void loop() {
    int level = readWaterLevel();
    calculateWaterLevel(level);     
    readBattery();


    if(dataChanged == TRUE  || (lastSend + SEND_INTERVAL) < millis()){
        notifyServer();
        dataChanged = FALSE;
        lastSend = millis();
    }
    
    // When a given threshold of time has past the device will be active
    // The threshold can be controlled via the ACTIVIY_TRHESHOLD definition
    // This decision was made to improve the lifetime of each component.
    // It is not necesary to have realtime data instead a "snapshot" of 
    // every X minute is enough. 
    delay(ACTIVIY_THRESHOLD);    
}