package study.annotation.range;



public class RangeCheckTest {

    @org.junit.Test
    public void rangeCheckTest() throws IllegalAccessException {
        Person person = new Person("zLimbo", "Shanghai");
        RangeCheck.check(person);
    }

}