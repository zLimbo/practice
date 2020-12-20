package com.zlimbo.study;

import com.alibaba.fastjson.JSONObject;

import java.util.Map;

public class FieldCaseTransform {


    static public void main(String[] args) {
        String smallHump = "sm4Key";
        String upperUnderline = "SM4_KEY";
        System.out.println(
                String.format("smallHumpToUpperUnderline: %s -> %s",
                        smallHump, smallHumpToUpperUnderline(smallHump)));
        System.out.println(
                String.format("smallHumpToUpperUnderline: %s -> %s",
                        upperUnderline, upperUnderlineToSmallHump(upperUnderline)));

        String upperJsonStr = "{\"INVOKE_TIME\":\"2019-03-05 11:11:11.12\",\"BUSINESS_ID\":\"BUSINESS_ID\",\"TABLE_NAME\":\"invoice\",\"ACCOUNT_ID\":\"00002\",\"CALLBACK_URL\":\"http://127.0.0.1:8082/callback\",\"PRIVATE_KEY\":\"d6c83aee4bfbeb135a2dcef8c803b186d0678a99002b09d3c60c22aca7105005\",\"PUBLIC_KEY\":\"2204404536ab867d9a964bfcc5e6fdaa7d77e509ce5891d38b3ebbb036e5c225994597ea6d0bdff3539fd3062b3943a1c7dd75d173f35101b71298e9f7f08d51\",\"SECRET_KEY\":\"0123456789abcdef\",\"KEY_ID\":\"00006\",\"REQUEST_SN\":\"0e57dfd8-267b-42b9-af82-1a858855b307\",\"SYSTEM_ID\":\"SYSTEM_ID\",\"DATA_INFO\":{\"CONSUMER_TAXES_NO\":\"12100000425006133D\",\"PRICE\":\"1000\",\"PRICE_PLUS_TAXES\":\"1000\",\"TAXES\":\"150\",\"INVOICE_NUMBER\":\"1\",\"INVOICE_NO\":\"3100982170\",\"INVOICE_DATE\":\"2020-10-03\",\"STATEMENT_SHEET\":\"1\",\"INVOICE_TYPE\":\"增值税\",\"SELLER_TAXES_NO\":\"913100003245878130\",\"STATEMENT_WEIGHT\":\"1kg\",\"TIMESTAMPS\":\"1601712721\",\"CONSUMER_NAME\":\"数据学院\",\"SELLER_NAME\":\"华东师大\",\"TAXES_POINT\":\"17%\"}}";
        JSONObject upperJson = JSONObject.parseObject(upperJsonStr);
        JSONObject u2sJson = upperUnderlineToSmallHump(upperJson);
        JSONObject s2uJson = smallHumpToUpperUnderline(u2sJson);
        JSONObject u2sJson2 = upperUnderlineToSmallHump(s2uJson);
//        System.out.println(JSONObject.toJSONString(upperJson, true));
//        System.out.println(JSONObject.toJSONString(u2sJson, true));
//        System.out.println(JSONObject.toJSONString(s2uJson, true));
//        System.out.println(JSONObject.toJSONString(u2sJson2, true));

//        System.out.println(upperJson.equals(s2uJson));
//        System.out.println(u2sJson.equals(u2sJson2));


//        for (String s : sa1) {
//            System.out.println(upperUnderlineToSmallHump(s));
//        }
//        System.out.println("");
//        for (String s : sa2) {
//            System.out.println(upperUnderlineToSmallHump(s));
//        }

//        for (String s: params) {
//            System.out.println("final public static String " + s + " = \"" + upperUnderlineToSmallHump(s) + "\";");
//        }

//        System.out.println(JSONObject.toJSONString(smallHumpToUpperUnderline(JSONObject.parseObject(jsonStr)), true));

        JSONObject json = new JSONObject();
        Map<String, String> map = null;
        json.putAll(map);
    }

    /**
     * 小驼峰转大写下划线
     *
     * @param inString
     * @return
     */
    static public String smallHumpToUpperUnderline(String inString) {
        StringBuilder stringBuilder = new StringBuilder();
        for (int i = 0; i < inString.length(); ++i) {
            char c = inString.charAt(i);
            if ('A' <= c && c <= 'Z') {
                stringBuilder.append('_');
                stringBuilder.append(c);
            } else if ('a' <= c && c <= 'z') {
                stringBuilder.append((char) (c - 'a' + 'A'));
            } else {
                stringBuilder.append(c);
            }
        }
        return stringBuilder.toString();
    }


    /**
     * json的键小驼峰转大写下划线
     *
     * @param inJson
     * @return
     */
    static public JSONObject smallHumpToUpperUnderline(JSONObject inJson) {
        JSONObject outJson = new JSONObject();
        for (String key : inJson.keySet()) {
            Object value = inJson.get(key);
            if (value instanceof JSONObject) {
                value = smallHumpToUpperUnderline((JSONObject) value);
            }
            String newKey = smallHumpToUpperUnderline(key);
            outJson.put(newKey, value);
        }
        return outJson;
    }


    /**
     * 大写下划线转小驼峰
     *
     * @param inString
     * @return
     */
    static public String upperUnderlineToSmallHump(String inString) {
        StringBuilder stringBuilder = new StringBuilder();
        boolean afterUnderline = false;
        for (int i = 0; i < inString.length(); ++i) {
            char c = inString.charAt(i);
            if (c == '_') {
                afterUnderline = true;
                continue;
            }
            if (afterUnderline) {
                stringBuilder.append(c);
                afterUnderline = false;
            } else {
                if ('A' <= c && c <= 'Z') {
                    stringBuilder.append((char) (c - 'A' + 'a'));
                } else {
                    stringBuilder.append(c);
                }
            }
        }
        return stringBuilder.toString();
    }


    /**
     * json的键大写下划线转小驼峰
     *
     * @param inJson
     * @return
     */
    static public JSONObject upperUnderlineToSmallHump(JSONObject inJson) {
        JSONObject outJson = new JSONObject();
        for (String key : inJson.keySet()) {
            Object value = inJson.get(key);
            if (value instanceof JSONObject) {
                value = upperUnderlineToSmallHump((JSONObject) value);
            }
            String newKey = upperUnderlineToSmallHump(key);
            outJson.put(newKey, value);
        }
        return outJson;
    }


    static String[] sa1 = {
            "RESOURCE_CODE",
            "MANUFACTURER_CODE",
            "MANUFACTURER_NAME",
            "CATEGORY_CODE",
            "CATEGORY_NAME",
            "PRODUCT_CODE",
            "PRODUCT_NAME",
            "SHOP_SIGN",
            "THICKNESS_UPPER",
            "THICKNESS_LOWER",
            "WIDTH_UPPER",
            "WIDTH_LOWER",
            "LENGTH_UPPER",
            "LENGTH_LOWER",
            "GMV",
            "MOQ",
            "ONSALE_DATE_START",
            "ONSALE_DATE_END",
            "ONSALE_PRICE",
            "DELIVERY_DATE",
            "DELIVERY_ADD",
            "TECH_STD",
            "STD_SHOP_SIGN",
            "STD_SPEC",
            "ONSALE_WAY_CODE",
            "ONSALE_WAY_NAME",
            "STATUS_CODE",
            "STATUS_NAME",
            "ABORT_AUCTION_DATE",
            "CONTRACT_DATE",
            "SYSTEM_ID",
            "REQUEST_SN",
            "BLOCK_HEIGHT",
            "TX_HASH",
            "CONTRACT_ADDRESS",
            "ON_CHAIN"
    };

    static String[] sa2 = {
            "CONTRACT_CODE",
            "RESOURCE_CODE",
            "SELLER_CODE",
            "SELLER_NAME",
            "BUYER_CODE",
            "BUYER_NAME",
            "RESOURCE_TYPE_CODE",
            "RESOURCE_TYPE_NAME",
            "DEAL_DATE",
            "DEAL_PRICE",
            "DEAL_WEIGHT",
            "DEAL_PRICE",
            "HAVE_ORDER",
            "HAVE_PAY",
            "CONTRACT_STATUS_CODE",
            "CONTRACT_STATUS_NAME",
            "BREACH_CONTRACT_CODE",
            "BREACH_CONTRACT_NAME",
            "CREATED_DATE",
            "CREATED_USER",
            "UPDATED_DATE",
            "UPDATED_USER",
            "SYSTEM_ID",
            "REQUEST_SN",
            "BLOCK_HEIGHT",
            "TX_HASH",
            "CONTRACT_ADDRESS",
            "ON_CHAIN"
    };

    static String[] params = {
        "SYSTEM_ID",
        "REQUEST_SN",
        "INVOKE_TIME",
        "BUSINESS_ID",
        "CALLBACK_URL",
        "KEY_ID",
        "ACCOUNT_ID",
        "TX_HASH",
        "PRIVATE_KEY",
        "PUBLIC_KEY",
        "SM4_KEY",
        "SM4_IV",
        "DATA_INFO",
        "CONTRACT_ADDRESS",
        "ON_CHAIN",
        "BLOCK_TIME",
        "BLOCK_HEIGHT",
        "TABLE_NAME",
        "SEARCH_REQUEST_SN",
        "ON_CHAIN_SUCCESS",
        "SUCCESS",
        "CODE",
        "MSG"
    };

    static String jsonStr = "{\n" +
            "    \"sm4Iv\": \"sm4Iv\",\n" +
            "    \"systemId\": \"systemId\",\n" +
            "    \"dataInfo\": {\n" +
            "        \"timestamps\": \"1601712721\",\n" +
            "        \"taxesPoint\": \"17%\",\n" +
            "        \"sellerName\": \"华东师大\",\n" +
            "        \"taxes\": \"150\",\n" +
            "        \"invoiceDate\": \"2020-10-03\",\n" +
            "        \"consumerTaxesNo\": \"12100000425006133D\",\n" +
            "        \"sellerTaxesNo\": \"913100003245878130\",\n" +
            "        \"price\": \"1000\",\n" +
            "        \"statementSheet\": \"1\",\n" +
            "        \"pricePlusTaxes\": \"1000\",\n" +
            "        \"invoiceNumber\": \"1\",\n" +
            "        \"invoiceType\": \"增值税\",\n" +
            "        \"statementWeight\": \"1kg\",\n" +
            "        \"invoiceNo\": \"3100982170\",\n" +
            "        \"consumerName\": \"数据学院\"\n" +
            "    },\n" +
            "    \"businessId\": \"businessId\",\n" +
            "    \"keyId\": \"keyId\",\n" +
            "    \"publicKey\": \"publicKey\",\n" +
            "    \"tableName\": \"invoice\",\n" +
            "    \"accountId\": \"accountId\",\n" +
            "    \"privateKey\": \"privateKey\",\n" +
            "    \"sm4Key\": \"sm4Key\",\n" +
            "    \"requestSn\": \"c36aac94-0997-44bf-941f-e10d64daf64a\",\n" +
            "    \"invokeTime\": \"2019-03-05 11:11:11.12\",\n" +
            "    \"callbackUrl\": \"http://127.0.0.1:8082/callback\"\n" +
            "}";
}





