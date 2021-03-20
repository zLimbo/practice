package com.zlimbo.spingstudy.bean;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class UserService {

    @Autowired
    private UserRepository userRepository;

    public String getData() {
        return userRepository.getData();
    }

    public static void main(String[] args) {
        UserService userService = new UserService();
        String data = userService.getData();

        System.out.println("data: " + data);
    }
}
