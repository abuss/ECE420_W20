import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.*;
import java.util.stream.Collectors.*;
import java.util.function.*;

public class WordCount
{
  public static void main(String[] args) {
    try {
      Map<String, Integer> frequency = Files.readAllLines(Paths.get("Sample.txt")).stream()
              .flatMap(e->Stream.of(e.trim().split("\\s+")))
              .collect(Collectors.groupingBy(Function.identity(), Collectors.summingInt(e -> 1)));
      
      frequency.entrySet().stream()
              .sorted((e1,e2)->-e1.getValue().compareTo(e2.getValue()))
              .limit(15)
              .forEach(System.out::println);        
    }
    catch (Exception e) {
      System.out.println(e);
    }
  }
}
