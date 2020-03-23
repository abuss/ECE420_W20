package wordcount;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;   
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
import org.apache.hadoop.mapreduce.lib.jobcontrol.ControlledJob;
import org.apache.hadoop.mapreduce.lib.jobcontrol.JobControl;

import wordcount.WordMapper;

public class WordCount extends Configured implements Tool 
{

  public int run(String[] args) throws Exception 
  {
    // ---------------------------------------------------------------
    // Flow job control
    JobControl jobControl = new JobControl("WordCount jobChain"); 
    
    // First Job configuration
    Configuration conf1 = getConf();

    Job job1 = Job.getInstance(conf1);  
    job1.setJarByClass(WordCount.class);
    job1.setJobName("Word Count");

    FileInputFormat.setInputPaths(job1, new Path(args[0]));
    FileOutputFormat.setOutputPath(job1, new Path(args[1] + "/temp"));

    job1.setMapperClass(WordMapper.class);
    job1.setReducerClass(SumReducer.class);
    job1.setCombinerClass(SumReducer.class);

    job1.setOutputKeyClass(Text.class);
    job1.setOutputValueClass(IntWritable.class);

    ControlledJob controlledJob1 = new ControlledJob(conf1);
    controlledJob1.setJob(job1);

    jobControl.addJob(controlledJob1);

    // ---------------------------------------------------------------
    // Second Job configuration
    Configuration conf2 = getConf();

    Job job2 = Job.getInstance(conf2);
    job2.setJarByClass(WordCount.class);
    job2.setJobName("Word Invert");

    FileInputFormat.setInputPaths(job2, new Path(args[1] + "/temp"));
    FileOutputFormat.setOutputPath(job2, new Path(args[1] + "/final"));

    job2.setMapperClass(WordMapper2.class);
    job2.setReducerClass(SumReducer2.class);
    job2.setCombinerClass(SumReducer2.class);

    job2.setOutputKeyClass(IntWritable.class);
    job2.setOutputValueClass(Text.class);
    job2.setInputFormatClass(KeyValueTextInputFormat.class);

    // Change to a descending sorting order
    job2.setSortComparatorClass(IntComparator.class);
    
    ControlledJob controlledJob2 = new ControlledJob(conf2);
    controlledJob2.setJob(job2);

    // ---------------------------------------------------------------
    // make job2 dependent on job1
    controlledJob2.addDependingJob(controlledJob1); 
    // add the job to the job control
    jobControl.addJob(controlledJob2);

    // ---------------------------------------------------------------
    // A thread is needed to wait until all jobs are finished
    Thread jobControlThread = new Thread(jobControl);
    jobControlThread.start();

    while (!jobControl.allFinished()) {
        System.out.println("Jobs in waiting state: " + jobControl.getWaitingJobList().size());  
        System.out.println("Jobs in ready state: " + jobControl.getReadyJobsList().size());
        System.out.println("Jobs in running state: " + jobControl.getRunningJobList().size());
        System.out.println("Jobs in success state: " + jobControl.getSuccessfulJobList().size());
        System.out.println("Jobs in failed state: " + jobControl.getFailedJobList().size());
    try {
          Thread.sleep(5000);
        } catch (Exception e) {

        }
    } 

    System.exit(0);  
    return (job1.waitForCompletion(true) ? 0 : 1);
  }

  public static void main(String[] args) throws Exception { 
  int exitCode = ToolRunner.run(new WordCount(), args);  
  System.exit(exitCode);
  }

}