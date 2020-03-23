package wordcount;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class WordMapper extends Mapper<LongWritable, Text, Text, IntWritable> 
{
  IntWritable one = new IntWritable(1);
  Text text = new Text();

  @Override
  public void map(LongWritable key, Text value, Context context) 
    throws IOException, InterruptedException 
  {

    String line = value.toString();
    for (String word : line.split("\\W+")) {
        if (word.length() > 0) {
            text.set(word);
            context.write(text, one);
         }
     }
  }
} 