package de.hrw.xilab.spring.controller;


import de.hrw.xilab.spring.model.TokenResult;
import de.hrw.xilab.spring.services.DeviceService;
import de.hrw.xilab.spring.services.TokenService;
import org.springframework.security.core.Authentication;
import org.springframework.web.bind.annotation.*;

@RestApiController("/token")
@CrossOrigin(origins = "http://localhost:3000")
public class TokenController {

    public final DeviceService deviceService;
    private final TokenService tokenService;

    public TokenController(DeviceService deviceService, TokenService tokenService) {
        this.deviceService = deviceService;
        this.tokenService = tokenService;
    }

    @GetMapping(path = "/create")
    public TokenResult token(Authentication authentication) {
        return tokenService.generateToken(authentication);
    }

    @GetMapping(path = "/renew")
    public TokenResult renew(Authentication authentication) {
        return tokenService.generateToken(authentication);
    }
}
