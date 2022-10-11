package de.hrw.xilab.spring.model.api;

import lombok.Data;

@Data
public class DeviceData {
    private String name = "";
    private String uuid = "";
    private int battery = 0;
}
