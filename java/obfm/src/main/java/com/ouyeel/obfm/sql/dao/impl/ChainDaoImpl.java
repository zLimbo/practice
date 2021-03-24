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
            Object result =  sqlMapClient.insert("insertTx", paramMap);
            logger.info("result: [{}]", result);
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
        Map<String, String> paramMap = new HashMap<>();
        paramMap.put(ChainConfig.TABLE_NAME, tableName);
        paramMap.put(ChainConfig.REQUEST_SN, requestSn);
        logger.debug("queryTxHashByRequestSn...");
        try {
            List<Map<String, String>> list =
                    sqlMapClient.queryForList("queryTxHashByRequestSn", paramMap);
            Map<String, String> map = list.get(0);
            txHash = map.get(ChainConfig.TX_HASH);
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
        Map<String, String> paramMap = new HashMap<>();
        paramMap.put(ChainConfig.TABLE_NAME, tableName);
        paramMap.put(ChainConfig.TX_HASH, txHash);
        List<Map<String, String>> list;
        logger.debug("queryAllByTxHash...");
        try {
            list = sqlMapClient.queryForList("queryAllByTxHash", paramMap);
        } catch (SQLException e) {
            logger.debug("queryAllByTxHash fail");
            logger.error(e.getMessage());
            e.printStackTrace();
            return null;
        }
        logger.debug("queryAllByTxHash end");
        return list;
    }


    @Override
    public List<Map<String, String>> queryAllByRequestSn(String tableName, String requestSn) {
        logger.debug("queryAllByRequestSn start");
        Map<String, String> paramMap = new HashMap<>();
        paramMap.put(ChainConfig.TABLE_NAME, tableName);
        paramMap.put(ChainConfig.REQUEST_SN, requestSn);
        List<Map<String,String>> list;
        logger.debug("queryAllByRequestSn...");
        try {
            list = sqlMapClient.queryForList("queryAllByRequestSn", paramMap);
        } catch (SQLException e) {
            logger.debug("queryAllByRequestSn fail");
            logger.error(e.getMessage());
            e.printStackTrace();
            return null;
        }
        logger.debug("queryAllByRequestSn end");
        return list;
    }


    @Override
    public List<Map<String, String>> queryCallbackByRequestSn(String tableName, String requestSn) {
        logger.info("queryCallbackByRequestSn start");
        logger.info("queryCallbackByRequestSn tableName:["+tableName+"],requestSn:["+requestSn+"]");
        Map<String, String> paramMap = new HashMap<>();
        paramMap.put(ChainConfig.TABLE_NAME, tableName);
        paramMap.put(ChainConfig.REQUEST_SN, requestSn);
        List<Map<String, String>> list;
        try {
            logger.info("queryCallbackByRequestSn param : [{}]",paramMap);
            list = sqlMapClient.queryForList("queryCallbackByRequestSn", paramMap);
            logger.info("queryByTxHash result:[{}]",list);
        } catch (SQLException e) {
            logger.debug("queryByTxHash fail");
            logger.error(e.getMessage());
            e.printStackTrace();
            return null;
        }
        logger.info("queryByTxHash success");
        logger.debug("queryCallbackByRequestSn end");
        return list;
    }



    /**
     * 状态查询通用接口
     * @param tableName 数据表名
     * @param state 状态参数
     * @return 哈希
     */
    public String queryState(String tableName, String state) {
        Map<String, String> paramMap = new HashMap<>();
        paramMap.put(ChainConfig.TABLE_NAME, tableName);
        paramMap.put(ChainConfig.STATE, state);
        List<Map<String, String>> list = null;
        logger.debug("paramMap: " + paramMap);
        try {
            list = sqlMapClient.queryForList("queryState", paramMap);
        } catch (SQLException e) {
            e.printStackTrace();
            logger.error(e.getMessage());
            return null;
        }
        if (list == null) {
            return null;
        }
        Map<String, String> map = list.get(0);
        String txHash = map.get(ChainConfig.LOWERCASE_STATE);
        return txHash;
    }

    /**
     * 状态回调查询
     * @param tableName
     * @param stateHash
     * @return
     */
    public String queryCallbackState(String tableName, String state, String stateHash) {
        logger.debug("tableName: [{}], stateHash: [{}]", tableName, stateHash);
        Map<String, String> paramMap = new HashMap<>();
        paramMap.put(ChainConfig.TABLE_NAME, tableName);
        paramMap.put(ChainConfig.STATE, state);
        paramMap.put(ChainConfig.STATE_HASH, stateHash);
        List<Map<String, String>> list = null;
        try {
            list = sqlMapClient.queryForList("queryCallbackState", paramMap);
        } catch (SQLException e) {
            e.printStackTrace();
            logger.error(e.getMessage());
            return null;
        }
        if (list == null) {
            return null;
        }
        Map<String, String> map = list.get(0);
        String result = map.get(ChainConfig.LOWERCASE_STATE);
        return result;
    }


    public void test() {
        try {
            sqlMapClient.queryForList("test");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }


//    @Override
//    public String queryCount(String tableName) {
//        logger.debug("queryCount - tableName: [" + tableName + "]");
//        Map<String, String> map = queryForState("queryCount", tableName);
//        if (map == null) {
//            return null;
//        }
//        return map.get("COUNT");
//    }
//
//    @Override
//    public String queryLatestThreeTxDate(String tableName) {
//        logger.debug("queryLatestThreeTxDate - tableName: [" + tableName + "]");
//        Map<String, String> map = queryForState("queryLatestThreeTxDate", tableName);
//        if (map == null) {
//            return null;
//        }
//        return map.get("LATEST_THREE_TX_DATE");
//    }
//
//    @Override
//    public String queryCountAndLatestThreeTxDate(String tableName) {
//        logger.debug("queryCountAndLatestThreeTxDate - tableName: [" + tableName + "]");
//        Map<String, String> map = queryForState("queryCountAndLatestThreeTxDate", tableName);
//        if (map == null) {
//            return null;
//        }
//        return map.get("COUNT_AND_LATEST_THREE_TX_DATE");
//    }
//
//
//    /**
//     * 状态查询通用接口
//     * @param sqlId 查询sql语句id
//     * @param tableName 查询表名
//     * @return 返回相关map
//     */
//    private Map<String, String> queryForState(String sqlId, String tableName) {
//        Map<String, String> paramMap = new HashMap<>();
//        paramMap.put(ChainConfig.TABLE_NAME, tableName);
//        List<Map<String, String>> list = null;
//        try {
//            list = sqlMapClient.queryForList("queryCount", paramMap);
//        } catch (SQLException e) {
//            e.printStackTrace();
//            logger.error(e.getMessage());
//            return null;
//        }
//        if (list == null) {
//            return null;
//        }
//        return list.get(0);
//    }


}
