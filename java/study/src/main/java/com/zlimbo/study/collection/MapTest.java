package com.zlimbo.study.collection;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class MapTest {

    public static void main(String[] args) {
        Map<String, String> map = new HashMap<>();
        System.out.println(map.put("1", "1"));
        System.out.println(map.put("1", "2"));
        Objects.hash("");
    }
}
