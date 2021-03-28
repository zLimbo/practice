package com.ouyeel.obfm.sql.util;

import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Component;

@Component
public class ApplicationContextUtil implements ApplicationContextAware {

    private static ApplicationContext applicationContext;


    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        ApplicationContextUtil.applicationContext = applicationContext;
    }

    public static Object getBean(Class clazz){
        return applicationContext.getBean(clazz);
    }

    public static ApplicationContext getSpringContext() {
        return applicationContext;
    }
}