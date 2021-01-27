package com.zlimbo.study.thread;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.PriorityBlockingQueue;


public class BlockingQueueTest {

    static class Obj implements Comparable {

        int val;
        public Obj(int val) {
            this.val = val;
        }

        @Override
        public int compareTo(Object o) {
            Obj rhs = (Obj)o;
            if (this.val < rhs.val) {
                return -1;
            } else if (this.val > rhs.val) {
                return 1;
            }
            return 0;
        }
    }


    public static void main(String[] args) throws InterruptedException {
        BlockingQueue<Obj> objQueue = new PriorityBlockingQueue<>();
        for (int i = 0; i < 100; ++i) {
            objQueue.put(new Obj(i));
        }
        new Thread(() -> {
            System.out.println("size: " + objQueue.size());
            int i = 0;
            while (true) {
                System.out.print(i++ + ": ");
                Obj obj = null;
                try {
                    obj = objQueue.take();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println(obj.val);
            }
        }).start();

    }

}
