package com.ouyeel.obfm.sql.callback;

//import com.baosight.obmp.chain.sql.basic.CommonConstant;
import com.ouyeel.obfm.sql.basic.CommonConstant;
import org.springframework.amqp.core.Queue;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class RabbitConfig {

//
//    @Bean
//    public CustomExchange delayExchange() {
//        Map<String, Object> args = new HashMap<>();
//        args.put("x-delayed-type", "direct");
//        return new CustomExchange("agent_stroage_exchange", "x-delayed-message",true, false,args);
//    }

    //队列 AgentStorageQueue
    @Bean
    public Queue agentStorageQueue() {
        // durable:是否持久化,默认是false,持久化队列：会被存储在磁盘上，当消息代理重启时仍然存在，暂存队列：当前连接有效
        // exclusive:默认也是false，只能被当前创建的连接使用，而且当连接关闭后队列即被删除。此参考优先级高于durable
        // autoDelete:是否自动删除，当没有生产者或者消费者使用此队列，该队列会自动删除。
        //   return new Queue("TestDirectQueue",true,true,false);

        //一般设置一下队列的持久化就好,其余两个就是默认false
        return new Queue(CommonConstant.AGENT_STORAGE_QUEUE,true);
    }

//    @Bean
//    public Binding binding() {
//        return BindingBuilder.bind(agentStorageQueue()).to(delayExchange()).with(CommonConstant.AGENT_STORAGE_QUEUE).noargs();
//    }

}
