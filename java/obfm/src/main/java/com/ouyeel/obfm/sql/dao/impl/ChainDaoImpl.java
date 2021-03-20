package com.ouyeel.obfm.sql.dao.impl;


//import com.baosight.obmp.chain.sql.config.ChainConfig;
//import com.baosight.obmp.chain.sql.dao.IChainDao;
import com.ibatis.common.resources.Resources;
import com.ibatis.sqlmap.client.SqlMapClient;
import com.ibatis.sqlmap.client.SqlMapClientBuilder;
import com.ouyeel.obfm.sql.config.ChainConfig;
import com.ouyeel.obfm.sql.dao.IChainDao;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import java.io.Reader;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
public class ChainDaoImpl implements IChainDao {
    final static Logger logger = LoggerFactory.getLogger(ChainDaoImpl.class);

    /**
     * ibatis 连接
     */
    static SqlMapClient sqlMapClient = null;


    static {
        logger.debug("ibatis SqlMapConfig ...");
        try {
            Reader reader = Resources.getResourceAsReader("SqlMapConfig.xml");
            sqlMapClient = SqlMapClientBuilder.buildSqlMapClient(reader);
            reader.close();
            logger.debug("ibatis SqlMapConfig success");
        } catch (Exception e) {
            logger.debug("ibatis SqlMapConfig error");
            logger.error(e.getMessage());
            e.printStackTrace();
        }
    }

    @Override
    public SqlMapClient getSqlMapClient() {
        return sqlMapClient;
    }


    @Override
    public boolean insertTx(Map<String, String> paramMap) {
        logger.info("insertTx start paramMap: [{}]", paramMap);
        try {
            Object insertResult =  sqlMapClient.insert("insertTx", paramMap);
            logger.info("insertResult: [{}]", insertResult);
        } catch (SQLException e) {
            logger.error("insertTx fail  [{}]",e.getMessage());
            e.printStackTrace();
            return false;
        }
        return true;
    }


    @Override
    public String queryTxHashByRequestSn(String tableName, String requestSn) {
        logger.debug("queryTxHashByRequestSn start");
        String txHash = null;
        Map<String, String> queryParamMap = new HashMap<>();
        queryParamMap.put(ChainConfig.TABLE_NAME, tableName);
        queryParamMap.put(ChainConfig.REQUEST_SN, requestSn);
        logger.debug("queryTxHashByRequestSn...");
        try {
            List<Map<String, String>> queryResulList =
                    sqlMapClient.queryForList("queryTxHashByRequestSn", queryParamMap);
            Map<String, String> queryResultMap = queryResulList.get(0);
            txHash = queryResultMap.get(ChainConfig.TX_HASH);
            logger.debug("txHash: [{}]", txHash);
        } catch (SQLException e) {
            logger.debug("queryTxHashByRequestSn fail");
        }
        logger.debug("queryByRequestSn success");
        logger.debug("queryTxHashByRequestSn end");
        return txHash;
    }


    @Override
    public List<Map<String, String>> queryAllByTxHash(String tableName, String txHash) {
        logger.debug("queryAllByTxHash start");
        Map<String, String> queryParamMap = new HashMap<>();
        queryParamMap.put(ChainConfig.TABLE_NAME, tableName);
        queryParamMap.put(ChainConfig.TX_HASH, txHash);
        List<Map<String, String>> queryResulList = null;
        logger.debug("queryAllByTxHash...");
        try {
            queryResulList = sqlMapClient.queryForList("queryAllByTxHash", queryParamMap);
        } catch (SQLException e) {
            logger.debug("queryAllByTxHash fail");
            logger.error(e.getMessage());
            e.printStackTrace();
            return null;
        }
        logger.debug("queryAllByTxHash end");
        return queryResulList;
    }


    @Override
    public List<Map<String, String>> queryAllByRequestSn(String tableName, String requestSn) {
        logger.debug("queryAllByRequestSn start");
        Map<String, String> queryParamMap = new HashMap<>();
        queryParamMap.put(ChainConfig.TABLE_NAME, tableName);
        queryParamMap.put(ChainConfig.REQUEST_SN, requestSn);
        List<Map<String,String>> queryResulList = null;
        logger.debug("queryAllByRequestSn...");
        try {
            queryResulList = sqlMapClient.queryForList("queryAllByRequestSn", queryParamMap);
        } catch (SQLException e) {
            logger.debug("queryAllByRequestSn fail");
            logger.error(e.getMessage());
            e.printStackTrace();
            return null;
        }
        logger.debug("queryAllByRequestSn end");
        return queryResulList;
    }


    @Override
    public List<Map<String, String>> queryCallbackByRequestSn(String tableName, String requestSn) {
        logger.info("queryCallbackByRequestSn start");
        logger.info("queryCallbackByRequestSn tableName:["+tableName+"],requestSn:["+requestSn+"]");
        Map<String, String> queryParamMap = new HashMap<>();
        queryParamMap.put(ChainConfig.TABLE_NAME, tableName);
        queryParamMap.put(ChainConfig.REQUEST_SN, requestSn);
        List<Map<String, String>> queryResulList = null;
        logger.debug("queryByTxHash...");
        try {
            logger.info("queryCallbackByRequestSn param : [{}]",queryParamMap);
            queryResulList = sqlMapClient.queryForList("queryCallbackByRequestSn", queryParamMap);
            logger.info("queryByTxHash result:[{}]",queryResulList);
        } catch (SQLException e) {
            logger.debug("queryByTxHash fail");
            logger.error(e.getMessage());
            e.printStackTrace();
            return null;
        }
        logger.info("queryByTxHash success");
        logger.debug("queryCallbackByRequestSn end");
        return queryResulList;
    }



}
