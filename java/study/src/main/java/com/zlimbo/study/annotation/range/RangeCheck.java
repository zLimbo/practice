package com.zlimbo.study.annotation.range;

import java.lang.reflect.Field;

public class RangeCheck {

    public static void check(Object obj) throws IllegalAccessException {
        for (Field field: obj.getClass().getDeclaredFields()) {
            field.setAccessible(true);
            Range range = field.getAnnotation(Range.class);
            if (range != null) {
                Object value = field.get(obj);
                if (value instanceof String) {
                    String s = (String) value;
                    if (s.length() < range.min() || s.length() > range.max()) {
                        throw new IllegalAccessException("Invalid field: " + field.getName());
                    }
                }
            }
        }
    }
}
