package com.ouyeel.obfm.controller;

import com.alibaba.fastjson.JSONObject;
import com.ouyeel.obfm.sql.ChainManager;
import com.ouyeel.obfm.sql.InvoiceChainManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.UUID;

@RestController
public class InvoiceController {

    static Logger logger = LoggerFactory.getLogger(InvoiceController.class);

    private ChainManager chainManager = new InvoiceChainManager();

    @RequestMapping("/")
    public String hello() {
        return "hello world!";
    }

    /**
     * 业务数据上链存证接口, 将业务信息进行上链存证。不对业务数据进行关联。
     * @param inJson
     * @return
     */
    @RequestMapping(value = "/obst/service/S_ST_01",
            method = RequestMethod.POST, produces = "application/json;charset=UTF-8")
    String upChain(@RequestBody JSONObject inJson) {
        logger.debug("[upChain] start");

        inJson.put("requestSn", UUID.randomUUID().toString());

        String txHash = chainManager.executor(inJson);
        logger.debug("[upChain] end");
        return txHash;
    }

}
