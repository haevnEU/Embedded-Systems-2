package de.hrw.xilab.spring.util.exceptions;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.ALREADY_REPORTED)
public class ElementAlreadyExist extends RuntimeException{
    public ElementAlreadyExist(String uuid){
        super("A device with uuid " + uuid + " already exist");
    }
}
