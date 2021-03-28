package com.ouyeel.obfm.sql.callback.consumer;

//import com.baosight.obmp.chain.sql.util.BlockChainCallBackService;
import com.ouyeel.obfm.sql.util.BlockChainCallBackService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.nio.charset.StandardCharsets;

@Component
public class CallbackConsumer {

    private static final Logger logger = LoggerFactory.getLogger(CallbackConsumer.class);

    @Autowired
    private BlockChainCallBackService blockChainCallBackService;


    @RabbitListener(queues = "${agent.storage.queue}")
    public void process(Message str) {

        logger.info("consumer msg start");
        String msg = new String(str.getBody(), StandardCharsets.UTF_8);
        logger.info("consumer msg :" + msg);
        String[] split = msg.split(",");
        blockChainCallBackService.callBack(split[0], split[1], split[2]);
    }
}
