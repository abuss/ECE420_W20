#!/usr/bin/python
import sys

#Word Count Example
# input comes from standard input STDIN
for line in sys.stdin:
  line = line.strip().lower() #remove leading and trailing whitespaces
  words = line.split() #split the line into words and returns as a list
  for word in words:
    #write the results to standard output STDOUT
    print("{}\t{}".format(word,1)) #Emit the word
