package study.annotation.range;

public class Person {
    @Range(min=1, max=20)
    private String name;

    @Range(max=10)
    private String city;

    public Person(String name, String city) {
        this.name = name;
        this.city = city;
    }

}
