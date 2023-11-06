package de.hrw.xilab.spring.util.exceptions;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.NOT_FOUND)
public class ElementDoesNotExist extends RuntimeException{
    public ElementDoesNotExist(String uuid){
        super("No device with uuid " + uuid + " exist");
    }
}
