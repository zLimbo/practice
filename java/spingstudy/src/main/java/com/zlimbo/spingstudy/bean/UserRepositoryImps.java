package com.zlimbo.spingstudy.bean;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class UserRepositoryImps implements UserRepository {

    @Bean
    public String getData() {
        return "UserRepositoryImps";
    }
}
