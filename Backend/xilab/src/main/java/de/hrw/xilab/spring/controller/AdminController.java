package de.hrw.xilab.spring.controller;


import de.hrw.xilab.spring.model.api.Device;
import de.hrw.xilab.spring.services.DeviceService;
import de.hrw.xilab.spring.util.IOUtils;
import de.hrw.xilab.spring.util.exceptions.GenericObjectException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.core.io.Resource;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.Optional;
import java.util.UUID;

@RestApiController("/admin")
@CrossOrigin(origins = "http://localhost:3000")
public class AdminController {

    private static final Logger LOGGER = LoggerFactory.getLogger(AdminController.class);
    public final DeviceService deviceService;
    private final String code;

    public AdminController(DeviceService deviceService) {

        this.deviceService = deviceService;

        code = IOUtils.readFromResource("sourcecode/ClientDevice/ClientDevice.ino");
    }


    @PostMapping(path = "/new/device")
    public ResponseEntity<Resource> postDevice(@RequestParam Optional<String> name,
                                               @RequestParam Optional<Integer> max,
                                               @RequestParam Optional<Integer> min) {

        Device device = buildDevice(name.orElseThrow(), min.orElseThrow(), max.orElseThrow());
        deviceService.save(device);

        String main = code.replace("$UUID$", device.getDeviceData().getUuid())
                .replace("$MAX$", "" + device.getWaterSensorData().getMax())
                .replace("$MIN$", "" + device.getWaterSensorData().getMin());

        byte[] result = main.getBytes(StandardCharsets.UTF_8);
        return IOUtils.buildFileDownloadResponseEntity(result, "ClientDevice.ino");
    }

    @PostMapping(path = "/new/list")
    public void postDevices(@RequestBody Optional<List<Device>> device) {
        deviceService.saveAll(device.orElseThrow());
    }

    @PatchMapping(path = "/patch")
    public void patchDevice(@RequestBody Optional<Device> device) {
        deviceService.update(device.orElseThrow());
    }


    @DeleteMapping(path = "/remove/device")
    public void deleteDevice(@RequestParam String uuid) {
        deviceService.deleteByUuid(uuid);
    }

    @DeleteMapping(path = "/remove/list")
    public ResponseEntity<List<String>> deleteDeviceList(@RequestBody List<String> uuids) {
        var removed = deviceService.deleteByUuidList(uuids);
        return ResponseEntity.ok(removed);
    }

    @GetMapping(path = "/code/main")
    public ResponseEntity<Resource> getSourceCode(@RequestParam Optional<String> name,
                                                  @RequestParam Optional<Long> max,
                                                  @RequestParam Optional<Long> min) {
        String uuid = UUID.randomUUID().toString();
        String main =code.replace("$MAX$", "" + max.orElseThrow())
                .replace("$MIN$", "" + min.orElseThrow())
                .replace("$UUID$", uuid);

        byte[] result = main.getBytes(StandardCharsets.UTF_8);
        return IOUtils.buildFileDownloadResponseEntity(result, "device.ino");
    }

    private Device buildDevice(String name, int min, int max) {
        Device device = new Device();

        device.getWaterSensorData().setMin(min);
        device.getWaterSensorData().setMax(max);
        device.getDeviceData().setName(name);
        UUID uuid = UUID.randomUUID();
        device.getDeviceData().setUuid(uuid.toString());
        return device;
    }


    @GetMapping(path = "/test")
    public void testIt(@RequestBody Device device) {
        throw new GenericObjectException("Dummy", device);
    }
}
