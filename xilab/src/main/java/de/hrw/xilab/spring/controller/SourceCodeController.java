package de.hrw.xilab.spring.controller;


import org.springframework.core.io.Resource;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;

import java.nio.charset.StandardCharsets;
import de.hrw.xilab.spring.util.IOUtils;

import static de.hrw.xilab.spring.util.IOUtils.*;

@RestApiController("/code")
@CrossOrigin(origins = "http://localhost:3000")
public class SourceCodeController {

    private final String shared_utils;
    private final String shared_lora;

    private final String backend_glue;
    private final String backend_main;

    private final String device_main;
    private final String device_watermeasurement;
    private final String device_xilabgps;

    public SourceCodeController() {
        this.shared_utils = readFromResource("sourcecode/shared/utils.h");
        this.shared_lora = readFromResource("sourcecode/shared/loracom.h");

        this.backend_glue = readFromResource("sourcecode/LoRaGateway/backendglue.h");
        this.backend_main = readFromResource("sourcecode/LoRaGateway/gateway.ino");

        this.device_main = readFromResource("sourcecode/ClientDevice/ClientDevice.ino");
        this.device_watermeasurement = readFromResource("sourcecode/ClientDevice/watermeasurement.ino");
        this.device_xilabgps = readFromResource("sourcecode/ClientDevice/xilabgps.ino");
    }

    @GetMapping(path = "/gateway/backendglue")
    public ResponseEntity<Resource> getBackendGlue() {
        byte[] result = backend_glue.getBytes(StandardCharsets.UTF_8);
        return buildFileDownloadResponseEntity(result, "backendglue.h");
    }

    @GetMapping(path = "/gateway/loracom")
    public ResponseEntity<Resource> getBackendLora() {
        byte[] result = shared_lora.getBytes(StandardCharsets.UTF_8);
        return buildFileDownloadResponseEntity(result, "loracom.h");
    }

    @GetMapping(path = "/gateway/utils")
    public ResponseEntity<Resource> getBackendUtils() {
        byte[] result = shared_utils.getBytes(StandardCharsets.UTF_8);
        return buildFileDownloadResponseEntity(result, "utils.h");
    }

    @GetMapping(path = "/gateway/main")
    public ResponseEntity<Resource> getBackendMain() {
        byte[] result = backend_main.getBytes(StandardCharsets.UTF_8);
        return buildFileDownloadResponseEntity(result, "gateway.ino");
    }



    @GetMapping(path = "/device/main")
    public ResponseEntity<Resource> getDeviceMain() {
        byte[] result = device_main.getBytes(StandardCharsets.UTF_8);
        return IOUtils.buildFileDownloadResponseEntity(result, "ClientDevice.ino");
    }

    @GetMapping(path = "/device/gps")
    public ResponseEntity<Resource> getDeviceGps() {
        byte[] result = device_xilabgps.getBytes(StandardCharsets.UTF_8);
        return IOUtils.buildFileDownloadResponseEntity(result, "xilabgps.ino");
    }

    @GetMapping(path = "/device/measurement")
    public ResponseEntity<Resource> getDeviceWatermeasurement() {
        byte[] result = device_xilabgps.getBytes(StandardCharsets.UTF_8);
        return IOUtils.buildFileDownloadResponseEntity(result, "watermeasurement.ino");
    }

    @GetMapping(path = "/device/utils")
    public ResponseEntity<Resource> getDeviceUtils() {
        byte[] result = shared_utils.getBytes(StandardCharsets.UTF_8);
        return IOUtils.buildFileDownloadResponseEntity(result, "utils.ino");
    }

    @GetMapping(path = "/device/lora")
    public ResponseEntity<Resource> getDeviceLora() {
        byte[] result = shared_lora.getBytes(StandardCharsets.UTF_8);
        return IOUtils.buildFileDownloadResponseEntity(result, "loracom.ino");
    }
}
