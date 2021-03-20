package com.ouyeel.obfm.sql.callback;


import com.ouyeel.obfm.sql.util.BlockChainCallBackService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.amqp.core.Message;
import org.springframework.beans.factory.annotation.Autowired;

import java.nio.charset.StandardCharsets;

public class CallbackHandle {

    private static final Logger logger = LoggerFactory.getLogger(CallbackHandle.class);

    @Autowired
    private BlockChainCallBackService blockChainCallBackService;

    public void process(String msg) {
        String[] split = msg.split(",");
        blockChainCallBackService.callBack(split[0], split[1], split[2]);
    }

}
