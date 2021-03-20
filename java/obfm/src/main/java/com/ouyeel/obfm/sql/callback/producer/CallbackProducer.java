package com.ouyeel.obfm.sql.callback.producer;

import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class CallbackProducer {


    @Autowired
    private RabbitTemplate rabbitTemplate;

    public void send(String queue, String message) {
        rabbitTemplate.convertAndSend(queue, message);
    }
}
