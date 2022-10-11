package de.hrw.xilab.spring.services;

import de.hrw.xilab.spring.model.api.Device;
import de.hrw.xilab.spring.model.wrapper.DeviceWrapper;
import de.hrw.xilab.spring.repository.DeviceRepository;
import de.hrw.xilab.spring.util.ModelUtils;
import de.hrw.xilab.spring.util.exceptions.ElementAlreadyExist;
import de.hrw.xilab.spring.util.exceptions.ElementDoesNotExist;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.function.Predicate;

import static de.hrw.xilab.spring.util.ModelUtils.toDeviceWrapperFromDevice;

@Service
public class DeviceService {

    private static final Logger LOGGER = LoggerFactory.getLogger(DeviceService.class);

    private final DeviceRepository repository;

    public DeviceService(DeviceRepository repository) {
        this.repository = repository;
    }


    public void save(Device device) {
        var exists = repository.findByUuid(device.getDeviceData().getUuid()).orElse(null);
        if(null != exists){
            LOGGER.info("The device with the given UUID {} already exists", device.getDeviceData().getUuid());
            throw new ElementAlreadyExist(device.getDeviceData().getUuid());
        }
        repository.save(toDeviceWrapperFromDevice(device));
    }

    public void saveAll(List<Device> devices) {
        devices.forEach(this::save);
    }


    public List<Device> findAll() {
        return repository.findAll().stream()
                .map(ModelUtils::toDeviceFromDeviceWrapper)
                .toList();
    }

    public Device findByUuid(String uuid) {
        var device = repository.findByUuid(uuid).orElse(null);
        if(null == device){
            LOGGER.info("The device with the given UUID {} does not exist", uuid);
            throw new ElementDoesNotExist(uuid);
        }
        return ModelUtils.toDeviceFromDeviceWrapper(device);
    }

    public List<Device> findAllByBatteryBelow(double battery) {
        return repository.findAll().stream()
                .filter(deviceWrapper -> deviceWrapper.getBattery() <= battery)
                .map(ModelUtils::toDeviceFromDeviceWrapper)
                .toList();
    }

    public List<Device> findAllByMinimumWaterReached() {
        return repository.findAll().stream()
                .filter(deviceWrapper -> deviceWrapper.getMin() >= deviceWrapper.getCurrent())
                .map(ModelUtils::toDeviceFromDeviceWrapper)
                .toList();
    }


    public List<String> deleteByUuidList(List<String> uuids) {
        List<String> removed = new ArrayList<>();
        uuids.forEach(uuid -> {
            try {
                deleteByUuid(uuid);
                removed.add(uuid);
            } catch (Exception ignored) {
                LOGGER.info("The device with the given UUID {} does not exists", uuid);
            }
        });
        return removed;
    }

    public void deleteByUuid(String uuid) {
        var device = repository.findByUuid(uuid).orElse(null);
        if(null == device){
            LOGGER.info("The device with the given UUID {} does not exist", uuid);
            throw new ElementDoesNotExist(uuid);
        }
        repository.deleteById(device.getId());
    }


    public void update(Device device) {
        String uuid = device.getDeviceData().getUuid();
        findByUuid(uuid);
        deleteByUuid(uuid);
        save(device);
    }

    public void update(String uuid, Optional<Integer> battery, Optional<Integer> waterlevel, Optional<Double> longitude, Optional<Double> latitude) {
        var dataset = repository.findByUuid(uuid).orElse(null);
        if(null == dataset){
            LOGGER.info("The device with the given UUID {} does not exist", uuid);
            throw new ElementDoesNotExist(uuid);
        }
        battery.ifPresent(dataset::setBattery);
        waterlevel.ifPresent(dataset::setCurrent);
        longitude.ifPresent(dataset::setLongitude);
        latitude.ifPresent(dataset::setLatitude);
        repository.save(dataset);
    }








    public List<Device> filter(Optional<String> name,
                               Optional<Long> battery,
                               Optional<Double> latitude,
                               Optional<Double> longitude,
                               Optional<Long> min,
                               Optional<Long> max,
                               Optional<Long> current) {
        Predicate<DeviceWrapper> nameFilter = device -> true;
        Predicate<DeviceWrapper> batteryFilter = device -> true;
        Predicate<DeviceWrapper> latitudeFilter = device -> true;
        Predicate<DeviceWrapper> longitudeFilter = device -> true;
        Predicate<DeviceWrapper> minFilter = device -> true;
        Predicate<DeviceWrapper> maxFilter = device -> true;
        Predicate<DeviceWrapper> currentFilter = device -> true;

        if (name.isPresent()) {
            nameFilter = device -> device.getName().contains(name.get());
        }
        if (battery.isPresent()) {
            batteryFilter = device -> device.getBattery() <= battery.get();
        }
        if (latitude.isPresent()) {
            latitudeFilter = device -> device.getLatitude() == latitude.get();
        }
        if (longitude.isPresent()) {
            longitudeFilter = device -> device.getLongitude() == longitude.get();
        }
        if (max.isPresent()) {
            maxFilter = device -> device.getMax() == max.get();
        }
        if (min.isPresent()) {
            minFilter = device -> device.getMin() == min.get();
        }
        if (current.isPresent()) {
            currentFilter = device -> device.getCurrent() == current.get();
        }

        return repository.findAll().stream()
                .filter(nameFilter)
                .filter(batteryFilter)
                .filter(longitudeFilter)
                .filter(latitudeFilter)
                .filter(minFilter)
                .filter(maxFilter)
                .filter(currentFilter)
                .map(ModelUtils::toDeviceFromDeviceWrapper).toList();
    }

}
