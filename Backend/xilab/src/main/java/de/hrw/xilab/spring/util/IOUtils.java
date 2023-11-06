package de.hrw.xilab.spring.util;

import org.springframework.core.io.ByteArrayResource;
import org.springframework.core.io.Resource;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.util.ResourceUtils;

import java.io.*;
import java.util.stream.Collectors;

public final class IOUtils {
    private IOUtils(){}

    public static String readFromResource(String file){
        String result = "";

        try{
            File target = ResourceUtils.getFile("classpath:" + file);
            try(BufferedReader rd = new BufferedReader(new InputStreamReader(new FileInputStream(target)))){
                result = rd.lines().collect(Collectors.joining("\n"));
            }catch (Exception ex) {
                ex.printStackTrace();
            }
        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
        }
        return result;
    }


    public static ResponseEntity<Resource> buildFileDownloadResponseEntity(byte[] data, String filename){

        ByteArrayResource resource = new ByteArrayResource(data);

        return ResponseEntity.ok()
                .header(HttpHeaders.CONTENT_DISPOSITION, "attachment;filename=\"" + filename + "\"")
                .contentLength(data.length)
                .contentType(MediaType.APPLICATION_OCTET_STREAM)
                .body(resource);
    }

}
