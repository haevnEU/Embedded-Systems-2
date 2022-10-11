# XILAB API
## **/admin**
### **/new/device**
Register a new device into the database
Operation | Description
--|--
**Method** | *POST*
**Authenticaton** |required
**Param** | name, Name of the device
**Param** | max, Maximum of water capacity
**Param** | min, Minimum of water capacity
**Body** | none
**Return** | [Device](#Device)
**Response** | 200 OK
--- 

### **/new/list**
DEPRECATED

Register a list of devices into the database
Operation | Description
--|--
**Method** | *POST*
**Authenticaton** |required
**Param** | none
**Body** | [DeviceArray](#DeviceArray)
**Return** | none
**Response** | 200 OK
---

### **/patch**
Updates an existing device 
Operation | Description
--|--
**Method** | *PATCH*
**Authenticaton** |required
**Param** | none
**Body** | [Device](#Device)
**Return** | none
**Response** | 200 OK
--- 

### **/remove/device**
Removes a device from the database 
Operation | Description
--|--
**Method** | *DELETE*
**Authenticaton** |required
**Param** | UUID (e.g. `688b7fb2-92d1-49cb-0011-4212979ad6cf`)
**Body** | none
**Return** | none
**Response** | 200 OK
---

### **/remove/list**
Removes a device from the database 
Operation | Description
--|--
**Method** | *DELETE*
**Authenticaton** |required
**Param** | none
**Body** | [UUIDarray](#UUIDarray)
**Return** | [UUIDarray](#UUIDarray)
**Response** | 200 OK
--- 

### **/remove/list**
Request the main source code for given parameter
Operation | Description
--|--
**Method** | *Get*
**Authenticaton** | optional
**Param** | name, Name of the device
**Param** | longitude, longitude of the device
**Param** | latitude, latitude of the device
**Body** | none
**Return** | source filee
**Response** | 200 OK
--- 

### **/remove/list**
Request the lora source code
Operation | Description
--|--
**Method** | *Get*
**Authenticaton** | optional
**Param** | none
**Body** | none
**Return** | source filee
**Response** | 200 OK
--- 

### **/remove/list**
Request the water measurement source code
Operation | Description
--|--
**Method** | *Get*
**Authenticaton** | optional
**Param** | none
**Body** | none
**Return** | source filee
**Response** | 200 OK
--- 
---

## **/device**
### **/{uuid}**
Requests information about a given device
Operation | Description
--|--
**Method** | *GET*
**Authenticaton** | optional
**Param** | none
**Path** | UUID of the device
**Body** | none
**Return** | [Device](#Device)
**Response** | 200 OK
---
### **/{uuid}**
Updates information about a given device
Operation | Description
--|--
**Method** | *PATCH*
**Authenticaton** | required
**Param** | battery, new battery charge value, Optional
**Param** | waterlevel, new water level, Optional
**Param** | longitude, new longitude, Optional
**Param** | latitude, new latitude, Optional
**Path** | UUID of the device
**Body** | none
**Return** | none
**Response** | 200 OK
---
### **/all**
Requests information about all registered devices
Operation | Description
--|--
**Method** | *GET*
**Authenticaton** | optional
**Param** | none
**Path** | none
**Body** | none
**Return** | [DeviceArray](#DeviceArray)
**Response** | 200 OK
---
### **/battery**
Requests all registered devices where the battery charge is below a given percentage
Operation | Description
--|--
**Method** | *GET*
**Authenticaton** | optional
**Param** | value (Battery charge in percentage between 0 and 100)
**Body** | none
**Return** | [DeviceArray](#DeviceArray)
**Response** | 200 OK
---
### **/minimum**
Requests all registered devices where the minimum water level is reached
Operation | Description
--|--
**Method** | *GET*
**Authenticaton** | optional
**Param** | none
**Body** | none
**Return** | [DeviceArray](#DeviceArray)
**Response** | 200 OK
---
### **/filter**
Applies custom filtering. Providing a param applies the param to the filter chain, filtering is an and operation.

Example 1: `current=500&battery=30` Yields all devices with 500ml water level and less than 30% battery charge  

Example 2: `name=south&battery=30` Yields all devices with south inside the name and less than 30% battery charge  
Operation | Description
--|--
**Method** | *GET*
**Authenticaton** | optional
**Param** | name (Name of the device typical a string value)
**Param** | battery (Minimum battery charge )
**Param** | latitude (Latitude of the device)
**Param** | longitude (Longitude of the device)
**Param** | min (Minimum water level)
**Param** | max (Maximum water level)
**Param** | current (Current water level)
**Body** | none
**Return** | [DeviceArray](#DeviceArray)
**Response** | 200 OK
---
---


## **/token**
### **/create**
Requests a new token which can be used for authorization
Operation | Description
--|--
**Method** | *GET*
**Authenticaton** | Basic with username and password
**Param** | none
**Body** | none
**Return** | [TokenArray](#TokenArray)
**Response** | 200 OK
---
---

## **Schemas**
### **Device** 
```json
{
    "deviceData":{
        "name":"Test Device 1001",
        "uuid":"688b7fb2-92d1-49cb-0011-4212979ad6cf",
        "battery":100
    },
    "waterSensorData":{
        "max":250,
        "min":575,
        "current":200
    },
    "locationData":{
        "longitude":6.14,
        "latitude":51.3
    }
}
```

### **DeviceArray**
```json
[    
    {
        "deviceData":{
            "name":"Test Device 1001",
            "uuid":"688b7fb2-92d1-49cb-0011-4212979ad6cf",
            "battery":100
        },
        "waterSensorData":{
            "max":250,
            "min":575,
            "current":200
        },
        "locationData":{
            "longitude":6.14,
            "latitude":51.3
        }
    },    
    {
        "deviceData":{
            "name":"Test Device 1000",
            "uuid":"77886655-92d1-49cb-0000-4212979ad6cf",
            "battery":100
        },
        "waterSensorData":{
            "max":250,
            "min":575,
            "current":200
        },
        "locationData":{
            "longitude":6.14,
            "latitude":51.3
        }
    }
]
```

### **TokenArray**
```json
{   "token": "JWT TOKEN",
    "issuedAt": "2022-09-24T17:40:58.001148Z",
    "expiration": "2022-09-25T17:40:58.001148Z",
    "scope": "read", 
    "issuer": "self" 
}
```

### **UUIDarray**
```json
["688b7fb2-92d1-49cb-0011-4212979ad6cf", "77886655-92d1-49cb-0000-4212979ad6cf"]
```
