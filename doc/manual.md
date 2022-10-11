# Device Manual
## **Modules**
### **Water**
#### Constants
Constant                        | Human Readable     | Description 
--------------------------------|--------------------|--
SCK_PIN                         | SCK                | System Clock Pin
SCL_PIN                         | SCL                | Data pin
WATER_READ_TIMEOUT              | Read timeout       | Timeout for sensor readings
WATER_MEASUREMENT_TIMING_BUDGET | Timing budget      | 
WATER_SIGNAL_RATE_LIMIT         | Signal rate limit  | 
DISTANCE_CALIBRATION_VALUE      | Calibration        | Calibrated value reference
AMOUNT_MEASUREMENTS             | Amount measurements| More measurements increases the precision of the measurements, but it also increases the reading time
---
### **LoRa**
#### Constants

Constant | Human Readable | Description 
---------------|----------|--
LORA_PIN_SS    | Pin SS   | 
LORA_PIN_RST   | Pin RST  |
LORA_PIN_DIO   | Pin DI0  |
LORA_REGION    | Region   | Region of the module
LORA_SYNC_WORD | Syncword | Word, which is used to differentiate the LoRa communication

#### Region

Region          | Code 
----------------|--
Europe          | 866E6
North America   | 915E6
Asia            | 433E6

---
---
## **Errors**
### **Sent Data**
In case of an error the following JSON document is sent to the server.
```
{
    "error_code" : code,
    "device" : {
        "name" : name,
        "id" : UUID,
        "battery": battery level
    },
    "location" : {
        "long" : longitude,
        "lat" : latiude
    }
}
```

### **Codes**
Code       | Description | Solution
-----------|-------------|---------
0xDE (222) | The device battery has not enough charge to maintain the operation. | Recharge the device battery

---
---