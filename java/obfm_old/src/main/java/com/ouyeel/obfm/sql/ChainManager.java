package com.ouyeel.obfm.sql;

import com.alibaba.fastjson.JSONObject;
//import com.baosight.iplat4j.core.ioc.spring.PlatApplicationContext;
//import com.baosight.obmp.chain.sql.basic.CommonConstant;
//import com.baosight.obmp.chain.sql.basic.ExceptionCode;
//import com.baosight.obmp.chain.sql.callback.producer.CallbackProducer;
//import com.baosight.obmp.chain.sql.config.ChainConfig;
//import com.baosight.obmp.chain.sql.dao.impl.ChainDaoImpl;
//import com.baosight.obmp.chain.sql.exception.BusinessException;
//import com.baosight.obmp.chain.sql.util.ApplicationContextUtil;
//import com.baosight.obmp.chain.sql.util.BlockChainBasicService;
//import com.baosight.obmp.chain.sql.util.BusinessExecutors;
//import com.baosight.obmp.chain.sql.util.CommonUtils;
import com.ouyeel.obfm.sql.basic.CommonConstant;
import com.ouyeel.obfm.sql.basic.ExceptionCode;
import com.ouyeel.obfm.sql.callback.producer.CallbackProducer;
import com.ouyeel.obfm.sql.config.ChainConfig;
import com.ouyeel.obfm.sql.dao.impl.ChainDaoImpl;
import com.ouyeel.obfm.sql.exception.BusinessException;
import com.ouyeel.obfm.sql.util.ApplicationContextUtil;
import com.ouyeel.obfm.sql.util.BlockChainBasicService;
import com.ouyeel.obfm.sql.util.BusinessExecutors;
import com.ouyeel.obfm.sql.util.CommonUtils;
import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.TimeUnit;

/**
 * CITA SQL  数据管理组件上链工具封装
 * <p>
 * 子类继承后，实现initTableName方法，设置业务表名
 */
public abstract class ChainManager {

    private static final Logger logger = LoggerFactory.getLogger(ChainManager.class);

//    //上链结果通知间隔
//    private static final String CALLBACK_SEND_SLEEP = PlatApplicationContext.getProperty("eplat.obmp.dr.sc.callback.send.sleep");

    //上链结果通知间隔
    private static final String CALLBACK_SEND_SLEEP = "3000";

    private BlockChainBasicService blockChainBasicService;
    private ChainDaoImpl commonDao;
    private CallbackProducer producer;

    private String tableName;

    public abstract void initTableName();

    public void setTableName(String tableName) {
        this.tableName = tableName;
    }

    //初始化参数
    private void init() {
//        this.blockChainBasicService = (BlockChainBasicService) ApplicationContextUtil.getBean(BlockChainBasicService.class);
//        this.commonDao = (ChainDaoImpl) ApplicationContextUtil.getBean(ChainDaoImpl.class);
//        this.producer = (CallbackProducer) ApplicationContextUtil.getBean(CallbackProducer.class);
        this.blockChainBasicService = new BlockChainBasicService();
        this.commonDao = new ChainDaoImpl();
        this.producer = new CallbackProducer();

        initTableName();
    }

    public String executor(JSONObject inJson) {

        init();

        checkParam(inJson);

        checkRequestSn(inJson.getString(ChainConfig.LOWERCASE_REQUEST_SN));

        return store(inJson);
    }

    /**
     * 参数校验
     * 如果有callbackUrl,判断callbackServiceId是否为空
     *
     * @param inJson
     */
    private void checkParam(JSONObject inJson) {
        String callbackUrl = inJson.getString(ChainConfig.LOWERCASE_CALLBACK_URL);
        if (StringUtils.isNotBlank(callbackUrl)) {
            String callbackServiceId = inJson.getString(ChainConfig.CALLBAKC_SERVICEID);
            if (StringUtils.isBlank(callbackServiceId)) {
                throw new BusinessException(ExceptionCode.CALLBAKC_SERVICE_ID_NOT_FOUND);
            }
        }

    }

    private String store(JSONObject inJson) {
        logger.info("inJson: [{}]", inJson.toJSONString());

        //获取区块链账户，公私钥
        Map<String, String> paramMap = blockChainBasicService.obtainKey(
                inJson.getString(ChainConfig.LOWERCASE_ACCOUNT_ID),
                inJson.getString(ChainConfig.LOWERCASE_KEY_ID));

        //构造上链参数
        buildInsertTxParam(paramMap, inJson);
        paramMap.put(ChainConfig.TABLE_NAME, this.tableName);
        paramMap.put(ChainConfig.INVOKE_TIME, CommonUtils.longToDateString(paramMap.get(ChainConfig.INVOKE_TIME)));
        logger.info("上链参数：[{}]", JSONObject.toJSONString(paramMap));

        //上链
        boolean insertResult = commonDao.insertTx(paramMap);
        if (!insertResult) {
            logger.error("insertTx error");
            throw new BusinessException(ExceptionCode.UP_TX_FAIL);
        }

        if (StringUtils.isNotBlank(paramMap.get(ChainConfig.CALLBACK_URL))) {
            //上链结果通知入队
            callbackSendQueue(inJson, paramMap);
        }
        //查询交易hash
        String txHash = commonDao.queryTxHashByRequestSn(
                this.tableName,
                paramMap.get(ChainConfig.REQUEST_SN));
        logger.info("insertTx query txHash : [{}]", txHash);

        return txHash;
    }

    /**
     * 校验requestSn是否重复
     *
     * @param requestSn
     */
    private void checkRequestSn(String requestSn) {
        List<Map<String, String>> maps = commonDao.queryAllByRequestSn(
                this.tableName,
                requestSn);
        if (CollectionUtils.isNotEmpty(maps)) {
            throw new BusinessException(ExceptionCode.REQUEST_SN_EXISTS);
        }
    }

    private void callbackSendQueue(JSONObject inJson, Map<String, String> paramMap) {
        StringBuilder sb = new StringBuilder();
                sb.append(inJson.get(ChainConfig.CALLBAKC_SERVICEID))
                .append(",")
                .append(paramMap.get(ChainConfig.TABLE_NAME))
                .append(",")
                .append(paramMap.get(ChainConfig.REQUEST_SN));
        try {
            BusinessExecutors.getThreadPool().submit(() -> {
                try {
                    TimeUnit.MILLISECONDS.sleep(Long.valueOf(CALLBACK_SEND_SLEEP));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                producer.send(CommonConstant.AGENT_STORAGE_QUEUE, sb.toString());
            });
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("cita sql 线程池阻塞，入队信息：[{}]", sb.toString());
        }
    }

    private void buildInsertTxParam(Map<String, String> map, JSONObject inJson) {
        Set<String> strings = inJson.keySet();
        Iterator<String> iterator = strings.iterator();
        while (iterator.hasNext()) {
            String next = iterator.next();
            if (next.equals(ChainConfig.LOWERCASE_DATA_INFO)) {
                JSONObject dataInfoObject = inJson.getJSONObject(next);
                JSONObject afterDataInfo = CommonUtils.smallHumpToUpperUnderline(dataInfoObject);
                map.put(ChainConfig.DATA_INFO, JSONObject.toJSONString(afterDataInfo));
            } else if (ChainConfig.UP_CHAIN_PARAM_LOWERCASE.contains(next)) {
                map.put(CommonUtils.smallHumpToUpperUnderline(next), inJson.getString(next));
            }
        }
    }
}
