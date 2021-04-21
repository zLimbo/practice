package com.zlimbo.study.json;

import com.alibaba.fastjson.JSONObject;

public class JSONObjectTest {

    public static void main(String[] args) {
        User user = new User("001", "zlimbo", "male", 24);
        JSONObject json = (JSONObject) JSONObject.toJSON(user);
        System.out.println(JSONObject.toJSONString(json, true));
        User user2 = JSONObject.toJavaObject(json, User.class);
        System.out.println(user2);
    }

    public static class User {
        String id;
        String name;
        String sex;
        int age;

        @Override
        public String toString() {
            return "User{" +
                    "id='" + id + '\'' +
                    ", name='" + name + '\'' +
                    ", sex='" + sex + '\'' +
                    ", age=" + age +
                    '}';
        }

        public User(String id, String name, String sex, int age) {
            this.id = id;
            this.name = name;
            this.sex = sex;
            this.age = age;
        }

        public String getId() {
            return id;
        }

        public void setId(String id) {
            this.id = id;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public String getSex() {
            return sex;
        }

        public void setSex(String sex) {
            this.sex = sex;
        }

        public int getAge() {
            return age;
        }

        public void setAge(int age) {
            this.age = age;
        }
    }
}
