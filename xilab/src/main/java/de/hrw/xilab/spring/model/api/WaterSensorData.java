package de.hrw.xilab.spring.model.api;

import lombok.Data;

@Data
public class WaterSensorData {
    private int max = 0;
    private int min = 0;
    private int current = 0;
}
