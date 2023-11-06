package de.hrw.xilab.spring.model;

import lombok.Data;

import java.time.Instant;

@Data
public class TokenResult {
    private String token;
    private Instant issuedAt;
    private Instant expiration;
    private String scope;
    private String issuer;
}
