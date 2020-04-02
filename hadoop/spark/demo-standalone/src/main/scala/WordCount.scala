/* WordCount.scala */
/**
* Word Count version using RDD
*/

import org.apache.spark.SparkContext
import org.apache.spark.SparkConf

object WordCount {
  def main(args: Array[String]) {

    val sparkConf = new SparkConf().setAppName("Word Count").setMaster("local[*]")
    val sc = new SparkContext(sparkConf)

    val lines = sc.textFile("hdfs://mozgo:9000/Sample.txt")
    val words = lines.flatMap(_.toLowerCase.split("\\W+"))
    val wc = words.map(w => (w, 1))
                  .reduceByKey(_ + _)
                  .sortBy(_._2,false)
    wc.saveAsTextFile("hdfs://mozgo:9000/Sample.count")

    wc.foreach(w => println(w.toString))

    println("=== DONE ===")
  }
}