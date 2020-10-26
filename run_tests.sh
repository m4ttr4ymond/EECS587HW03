#!/bin/bash

FILENAME="run.out"

g++-10 -fopenmp ./hw3_mattrmd.cpp -o hw3_mattrmd;

echo -e "thread no, max, time" > $FILENAME;


for CORE_NO in 4 10 36
do
	for i in {0..10}
    do
        OUTPUT=`./hw3_mattrmd $CORE_NO 1`;
        echo -e $OUTPUT >> $FILENAME;
        echo -e $OUTPUT;
    done
done