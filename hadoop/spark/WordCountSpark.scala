val lines = sc.textFile("Sample.txt")

val words = lines.flatMap(_.split("\\s+"))

val wc = words.map(w => (w, 1)).reduceByKey(_ + _)

wc.saveAsTextFile("Sample.count")
