#!/bin/bash

echo "Starting the test" > output.log

# loop for number of read/writes to be done on the streams
for N in 10000 100000 1000000 10000000 100000000
do
	# loop for streams to be opened simultaneously
	for K in 1 2 4 8 16 32 64 128 510 1020
	do
		for B in 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432 67108864 134217728 268435456 536870912 1073741824 2147483648 4294967296
		do
		let START=$(date +%s)
#echo $N $K $B >> output.log
#OUTPUT=$(./a.out $N $K $B)
#		bash -c './a.out $K $B $N 2>> output.log' >>output.log  2>&1
		./a.out $K $B $N
		let END=$(date +%s)
		let TOTALTIME=$(($END - $START))
		echo "$N $K $B $TOTALTIME" >> output.log
		rm *.bin
		done
	done
#echo $N
done
