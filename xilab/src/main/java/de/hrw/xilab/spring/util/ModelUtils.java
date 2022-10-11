package de.hrw.xilab.spring.util;

import de.hrw.xilab.spring.model.api.Device;
import de.hrw.xilab.spring.model.api.DeviceData;
import de.hrw.xilab.spring.model.api.LocationData;
import de.hrw.xilab.spring.model.api.WaterSensorData;
import de.hrw.xilab.spring.model.wrapper.DeviceWrapper;

public class ModelUtils {
    private ModelUtils(){}
    public static Device toDeviceFromDeviceWrapper(DeviceWrapper wrapperDevice) {
        var device = new Device();
        DeviceData deviceData = new DeviceData();
        LocationData locationData = new LocationData();
        WaterSensorData waterSensorData = new WaterSensorData();

        deviceData.setBattery(wrapperDevice.getBattery());
        deviceData.setName(wrapperDevice.getName());
        deviceData.setUuid(wrapperDevice.getUuid());

        locationData.setLatitude(wrapperDevice.getLatitude());
        locationData.setLongitude(wrapperDevice.getLongitude());

        waterSensorData.setMin(wrapperDevice.getMin());
        waterSensorData.setMax(wrapperDevice.getMax());
        waterSensorData.setCurrent(wrapperDevice.getCurrent());

        device.setDeviceData(deviceData);
        device.setLocationData(locationData);
        device.setWaterSensorData(waterSensorData);
        return device;
    }

    public static DeviceWrapper toDeviceWrapperFromDevice(Device device) {
        DeviceWrapper ref = new DeviceWrapper();

        ref.setName(device.getDeviceData().getName());
        ref.setUuid(device.getDeviceData().getUuid());
        ref.setBattery(device.getDeviceData().getBattery());

        ref.setLatitude(device.getLocationData().getLatitude());
        ref.setLongitude(device.getLocationData().getLongitude());

        ref.setMax(device.getWaterSensorData().getMax());
        ref.setMin(device.getWaterSensorData().getMin());
        ref.setCurrent(device.getWaterSensorData().getCurrent());
        return ref;
    }
}
