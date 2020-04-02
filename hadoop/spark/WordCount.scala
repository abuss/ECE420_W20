import scala.io.Source

object WordCount {
  def main(args: Array[String]) {
    val counts = Source.fromFile("./Sample.txt")
                  .getLines()
                  .flatMap(_.split("\\W+"))
                  .toList
                  .groupBy((word: String) => word)
                  .mapValues(_.length)
    println(counts)
  }
}

