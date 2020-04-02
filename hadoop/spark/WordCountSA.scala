import org.apache.spark.SparkContext
// import org.apache.spark.sql.SparkSession
import org.apache.spark.SparkContext._
import org.apache.spark._

object WordCountSA {
  def main(args: Array[String]) {
    val sc = new SparkContext( "local", "WordCountSA", "/home/abuss/local/spark", Seq(), Map())  
    val file = sc.textFile(args(0))
    file.flatMap(_.toLowerCase.split("\\s+"))
        .map(word => (word, 1))
        .reduceByKey(_ + _)
        .saveAsTextFile(args(1))

    println("==== DONE ====")
  }
}
