package de.hrw.xilab.spring.model.wrapper;

import lombok.Data;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;

import javax.persistence.*;

@EnableAutoConfiguration
@Data
@Entity
@Table(name = "DeviceWrapper")
public class DeviceWrapper {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    private String name;
    private String uuid;
    private int battery;
    private double latitude;
    private double longitude;
    private int max;
    private int min;
    private int current;
}
