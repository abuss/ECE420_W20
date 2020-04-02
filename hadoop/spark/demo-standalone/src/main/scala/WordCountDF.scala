/* WordCounfDF.scala */
/**
* Word Count version using DataFrame
*/

import org.apache.spark.sql.SparkSession

object WordCountDF {
  def main(args: Array[String]) {

    val spark = SparkSession.builder()
                    .appName("WordCount-df")
                    .getOrCreate()

    val lines = spark.read.textFile("hdfs://mozgo:9000/Sample.txt")
    val wordsDF = lines.explode("value","word")((line: String) => line.toLowerCase.split("\\W+"))
    val wordCountDF = wordsDF.groupBy("word").count()
    val wordCountSorted = wordCountDF.sort(wordCountDF("count").desc)
    wordCountSorted.write.csv("hdfs://mozgo:9000/Sample-out.csv")
    wordCountSorted.show()
    println("=== DONE ===")
  }
}