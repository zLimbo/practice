package study.digest;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.UUID;

public class Test {

    static public void main(String[] args) throws NoSuchAlgorithmException {

        String data = UUID.randomUUID().toString();

        MessageDigest sha256 = MessageDigest.getInstance("SHA-256");

        sha256.update(data.getBytes(StandardCharsets.UTF_8));

        byte[] digest = sha256.digest();

        String hexChar = "0123456789abcdef";
        StringBuffer stringBuffer = new StringBuffer("0x");
        for (byte x: digest) {
            int y = x & 0xff;
            stringBuffer.append(hexChar.charAt(y >>> 4));
            stringBuffer.append(hexChar.charAt(0xf & y));
        }

        String out = stringBuffer.toString();

        System.out.println(out);

    }
}
