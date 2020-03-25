#!/usr/bin/python
import sys

# input comes from standard input STDIN
for line in sys.stdin:
  line = line.strip() 
  word,count = line.split()
  print("{}\t{}".format(int(count),word)) 