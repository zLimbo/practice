package study.string;

public class StringTest {

    static public void main(String[] args) {

        String s = "a\nb\nc\n";
        String s2 = s.replace("\n", "x");
        System.out.println(s);
        System.out.println(s2);

    }
}
