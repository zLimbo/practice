package com.zlimbo.study.jdbc;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;

public class ConnectPostgreSQL {

    public static void main(String[] args) {
//        String host = "192.168.192.137";
//        String port = "5432";
//        String db = "fisco";
//        String user = "root";
//        String passwd = "admin";

//        Connection connection = null;

//        String url = "jdbc:postgresql://" +
//                host + ":" +
//                port + "/" +
//                db +
//                "?useSSL=false" +
//                "&useUnicode=true" +
//                "&characterEncoding=UTF8" +
//                "&serverTimezone=GMT" +
//                "&allowPublicKeyRetrieval=true";

        //        try {
//            Properties info = new Properties();
//            info.setProperty("user","root");
//            info.setProperty("password","admin");
//            Connection con = DriverManager.getConnection(url,info);
////            connection = DriverManager.getConnection(
////                    url, user, passwd);
//        } catch (Exception e) {
//            System.out.println("Connect error!");
//            return;
//        }
//        System.out.println("Connect success!");

        Connection connection = null;
        String url = "jdbc:postgresql://192.168.192.137:5432/fisco";
        String username = "root";
        String password = "admin";
        try {
            Class.forName("org.postgresql.Driver").newInstance();
            connection = DriverManager.getConnection(url, username, password);
        } catch (InstantiationException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return;
        }
        System.out.println("success");

    }
}
