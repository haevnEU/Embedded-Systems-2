package de.hrw.xilab.spring.util.exceptions;

public class GenericObjectException extends RuntimeException{
    private final transient Object data;

    public GenericObjectException(String message, Object data){
        super(message);
        this.data = data;
    }

    public GenericObjectException(Throwable throwable, Object data){
        super(throwable);
        this.data = data;
    }

    public Object getData() {
        return data;
    }
}
