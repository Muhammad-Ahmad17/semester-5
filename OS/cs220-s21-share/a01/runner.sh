#!/bin/bash 
x=1
while [ $x -le 50 ]
do
        (time ./hello2 > /dev/null)  &>> hello2.txt
        (time ./hello  > /dev/null)  &>> hello.txt
  x=$(( $x + 1 ))
done




