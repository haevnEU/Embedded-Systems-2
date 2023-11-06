package de.hrw.xilab.spring.model.api;

import lombok.Data;

@Data
public class Device {
    private DeviceData deviceData = new DeviceData();
    private LocationData locationData = new LocationData();
    private WaterSensorData waterSensorData = new WaterSensorData();


}
