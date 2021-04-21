package com.zlimbo.study.io;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.Map;

public class Main {

    public void test() {
    }
    public static void main(String[] args) throws IOException {

        String homePath = System.getenv("HOMEPATH");
        OutputStream outputStream = new FileOutputStream(".zcat");

        outputStream.write("zcat".getBytes(StandardCharsets.UTF_8));

        File f = new File("%HOME%");

        System.out.println(f.getAbsolutePath());

        for (Map.Entry<String, String> entry: System.getenv().entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }

        String str = "zLimbo";
        byte[] data1 = str.getBytes(StandardCharsets.UTF_8);
        byte[] data2 = new byte[1024];
        for (int i = 0; i < data1.length; ++i) {
            data2[i] = data1[i];
        }
        String str2 = new String(data2, StandardCharsets.UTF_8);
        System.out.println("str2: " + str2);

    }

    static void printFiles(File[] files) {
        System.out.println("==========");
        if (files != null) {
            for (File f : files) {
                System.out.println(f);
            }
        }
        System.out.println("==========");
    }
}


