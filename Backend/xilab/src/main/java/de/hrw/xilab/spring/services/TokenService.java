package de.hrw.xilab.spring.services;

import de.hrw.xilab.spring.model.TokenResult;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.oauth2.jwt.JwtClaimsSet;
import org.springframework.security.oauth2.jwt.JwtEncoder;
import org.springframework.security.oauth2.jwt.JwtEncoderParameters;
import org.springframework.stereotype.Service;

import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.util.stream.Collectors;

@Service
public class TokenService {
    private static final Logger LOGGER = LoggerFactory.getLogger(TokenService.class);
    @Value("${security.expiration}")
    private int expiration;
    private final JwtEncoder jwtEncoder;

    public TokenService(JwtEncoder jwtEncoder) {
        this.jwtEncoder = jwtEncoder;
    }

    public TokenResult generateToken(Authentication authentication) {
        LOGGER.info("New token was requested");
        Instant now = Instant.now();
        Instant expireAt = now.plus(this.expiration, ChronoUnit.DAYS);
        String scope = authentication.getAuthorities().stream()
                .map(GrantedAuthority::getAuthority)
                .collect(Collectors.joining(" "));
        String issuer = "self";
        JwtClaimsSet claimsSet = JwtClaimsSet.builder()
                .issuer(issuer)
                .issuedAt(now)
                .expiresAt(expireAt)
                .claim("scope", scope)
                .build();

        TokenResult result = new TokenResult();
        result.setToken(this.jwtEncoder.encode(JwtEncoderParameters.from(claimsSet)).getTokenValue());
        result.setExpiration(expireAt);
        result.setIssuedAt(now);
        result.setScope(scope);
        result.setIssuer(issuer);

        LOGGER.info("Token details");
        LOGGER.info("Expiration {}", expireAt);
        LOGGER.info("Begin {}", now);
        LOGGER.info("Scope {}", scope);
        LOGGER.info("Issuer {}", issuer);
        return result;
    }

}
