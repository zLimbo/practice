package com.zlimbo.study.redis;

import redis.clients.jedis.Jedis;

import java.util.HashMap;

public class Test {

    public static void main(String[] args) {
        Jedis jedis = new Jedis("localhost", 6379, 100000);
//        HashMap<String, String> hashMap = new HashMap<>();
        int i = 0;
        try {
            long time = 0;
            do {
                i++;
                String key = "test" + i;
                String value = i + "";
                long start = System.currentTimeMillis();// 开始毫秒数
                jedis.set(key, value);
//                hashMap.put(key, value);
                long end = System.currentTimeMillis();
                time += end - start;
            } while (time < 1000);
        } finally {// 关闭连接
            jedis.close();
        }
        // 打印1秒内对Redis的操作次数
        System.out.println("redis每秒操作：" + i + "次");
    }
}
