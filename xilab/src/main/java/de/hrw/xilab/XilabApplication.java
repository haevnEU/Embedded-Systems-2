package de.hrw.xilab;

import de.hrw.xilab.spring.config.RsaKeyProperties;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.properties.EnableConfigurationProperties;


@EnableConfigurationProperties(RsaKeyProperties.class)
@SpringBootApplication
public class XilabApplication {

    public static void main(String[] args) {
        SpringApplication.run(XilabApplication.class, args);
    }

}
