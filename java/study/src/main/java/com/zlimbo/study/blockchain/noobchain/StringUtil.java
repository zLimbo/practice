package com.zlimbo.study.blockchain.noobchain;

//  链接: https://www.cnblogs.com/helloworld2018/p/9011369.html
//  创建数字签名
//  熟悉加密算法的朋友们，Java方式可以实现的加密方式有很多，
//  例如BASE、MD、RSA、SHA等等，我在这里选用了SHA256这种加密方式，
//  SHA（Secure Hash Algorithm）安全散列算法，
//  这种算法的特点是数据的少量更改会在Hash值中产生不可预知的大量更改，
//  hash值用作表示大量数据的固定大小的唯一值，而SHA256算法的hash值大小为256位。
//  之所以选用SHA256是因为它的大小正合适，一方面产生重复hash值的可能性很小，
//  另一方面在区块链实际应用过程中，有可能会产生大量的区块，而使得信息量很大，那么256位的大小就比较恰当了。
//
//  下面我创建了一个StringUtil方法来方便调用SHA256算法

import java.security.MessageDigest;

public class StringUtil {
    //Applies Sha256 to a string and returns the result.
    public static String applySha256(String input){
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            //Applies sha256 to our input,
            byte[] hash = digest.digest(input.getBytes("UTF-8"));
            StringBuffer hexString = new StringBuffer(); // This will contain hash as hexidecimal
            for (int i = 0; i < hash.length; i++) {
                String hex = Integer.toHexString(0xff & hash[i]);
                if(hex.length() == 1) hexString.append('0');
                hexString.append(hex);
            }
            return hexString.toString();
        }
        catch(Exception e) {
            throw new RuntimeException(e);
        }
    }
}