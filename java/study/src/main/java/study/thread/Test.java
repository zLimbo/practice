package study.thread;

public class Test {

    public static void main(String[] args) {
        StackTraceElement[] cause = Thread.currentThread().getStackTrace();
        for (StackTraceElement se: cause) {
            System.out.println(se.getClassName());
            System.out.println(se.getMethodName());
        }
    }
}
