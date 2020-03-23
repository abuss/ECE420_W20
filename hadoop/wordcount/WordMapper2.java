package wordcount;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class WordMapper2 extends Mapper< Text, Text, IntWritable, Text>
{
  IntWritable frequency = new IntWritable();

  @Override
  public void map(Text key, Text value, Context context)
    throws IOException, InterruptedException 
  {
    int newVal = Integer.parseInt(value.toString());
    frequency.set(newVal);
    context.write(frequency, key);
  }
}