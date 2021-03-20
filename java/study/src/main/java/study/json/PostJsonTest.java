package study.json;

import com.alibaba.fastjson.JSONObject;
import org.apache.http.HttpEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicHeader;
import org.apache.http.protocol.HTTP;
import org.apache.http.util.EntityUtils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

public class PostJsonTest {

    final static String SEND_URL = "http://127.0.0.1:8080/obst/service/S_ST_01";
    final static int THREAD_NUM = 20;
    final static int SEND_NUM = 50;

    static int count = 0;
    static Object lock = new Object();



    static public void main(String[] args) throws InterruptedException {

        while (true) {
            long start = System.currentTimeMillis();
            test();
            long end = System.currentTimeMillis();
            double time = (double)(end - start) / 1000;
            System.out.println("count: " + THREAD_NUM * SEND_NUM +
                    "\tspend time: " + time +
                    "s\ttps: " + (double)(THREAD_NUM * SEND_NUM) / time);
            Thread.sleep(2000);
        }
    }


    static public void test() throws InterruptedException {
//        System.out.println("test start");
        List<Thread> threadList = new ArrayList<>();
        for (int i = 0; i < THREAD_NUM; ++i) {
            Thread thread = new Thread(()->{
//                System.out.println("Current Thread Id: " + Thread.currentThread().getId());
                for (int j = 0; j < SEND_NUM; ++j) {
                    String jsonStr = "{\n" +
                            "    \"accountId\": \"002\",\n" +
                            "    \"systemId\": \"00007817\",\n" +
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
                            "    \"requestSn\": \"f2e4bcac-6f28-47ea-b48d-7eb3e10d5e52\",\n" +
                            "    \"invokeTime\": \"2019-03-05 11:11:11.12\",\n" +
                            "    \"businessId\": \"businessId\",\n" +
                            "    \"keyId\": \"0001\",\n" +
                            "    \"callbackUrl\": \"http://127.0.0.1:8082/callback\",\n" +
                            "    \"tableName\": \"invoice\"\n" +
                            "}";
                    JSONObject json = JSONObject.parseObject(jsonStr);
                    json.put("requestSn", UUID.randomUUID().toString());
                    try {
                        String response = send(SEND_URL, json, "UTF-8");
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
            thread.start();
            threadList.add(thread);
        }

        for (Thread thread: threadList) {
            thread.join();
        }
    }


    static public String send(String url, JSONObject jsonObject, String encoding) throws IOException {
//        System.out.println("====================> [send] start");
        String body = "";

        CloseableHttpClient client = HttpClients.createDefault();
        HttpPost httpPost = new HttpPost(url);

        StringEntity stringEntity = new StringEntity(jsonObject.toString(), "utf-8");
        stringEntity.setContentEncoding(new BasicHeader(HTTP.CONTENT_TYPE, "application/json"));
        httpPost.setEntity(stringEntity);
//        System.out.println("== request url: " + url);

        httpPost.setHeader("Content-type", "application/json");
        httpPost.setHeader("User-Agent", "Mozilla/4.0 (compatible; MSIE 5.0; Windows NT; DigExt)");

        CloseableHttpResponse response = client.execute(httpPost);

        HttpEntity entity = response.getEntity();
        if (entity != null) {
            body = EntityUtils.toString(entity, encoding);
        }
        EntityUtils.consume(entity);
        response.close();
//        System.out.println("== body: " + body);
        //JSONObject returnJson = new JSONObject();
//        System.out.println("====================> [send] end\n");
        return body;
    }
}
