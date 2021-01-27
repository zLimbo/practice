package com.zlimbo.study.thread;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class ThreadPoolTest {

    static final ExecutorService threadPool = new ThreadPoolExecutor(
            10,
            20,
            1000,
            TimeUnit.MILLISECONDS,
            new LinkedBlockingDeque<Runnable>()
    );

    static Map<Integer, Integer> map = new HashMap<>();

    static public void main(String[] args) {


        for (int i = 0; i < 1000; ++i) {
            Thread thread = new Thread(() -> {
                System.out.println("thread id: " + Thread.currentThread().getId());
                int key = (int) (Thread.currentThread().getId() / 5);
                synchronized (map) {
                    if (map.containsKey(key))
                    map.put(key, map.get(key) + 1);
                }
            });
            threadPool.execute(thread);
        }

        for (Map.Entry<Integer, Integer> entry: map.entrySet()) {
            System.out.println(entry.getKey() + " : " + entry.getValue());
        }
    }
}
