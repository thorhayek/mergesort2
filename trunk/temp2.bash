#!/bin/bash

echo "Starting the test" > mergeheap.log

# loop for number of read/writes to be done on the streams
for N in 10000 100000 1000000 10000000 100000000
do
		##generate random number in a file
		./fillrandom $N myfile.bin

		##run mergesort, heapsort and notedown the time
		let MSTART=$(date +%s)
		./mergeheapsort $N myfile.bin 1
		let MEND=$(date +%s)
		let MTOTALTIME=$(($MEND - $MSTART))
		echo "MergeSort: $N $MTOTALTIME" >> mergeheap.log

		rm myfile.bin.out

		let START=$(date +%s)
		./mergeheapsort $N myfile.bin 0
		let END=$(date +%s)
		let TOTALTIME=$(($END - $START))
		echo "HeapSort: $N $TOTALTIME" >> mergeheap.log
done
#echo $N
