#!bin/bash
>positiveTimeseries.txt #wipe or create timeseries data text file
>negativeTimeseries.txt
readarray -t positiveWords < positivewords.txt #load positive words as an array

readarray -t negativeWords < negativewords.txt #load negative words as an array

#echo "${positiveWords[@]}"
#echo $negativeWords

positiveMentions=0
negativeMentions=0
writeTime=$(date +%s);
loop=0
interval=1
writeTime=$(($writeTime+$interval))
#echo $writeTime
while read line #read tweet stream
do
        for i in "${positiveWords[@]}" #iterate through all positive words in array and check if contained in tweet
        do
                if [[ $line == *$i* ]]
                then
                        positiveMentions=$(($positiveMentions+1))
                        #echo $line
                        #echo "Pos:" $positiveMentions
                fi
        done
        for i in "${negativeWords[@]}" #iterate through all negative words in array and check if contained in tweet
        do
                if [[ $line == *$i* ]]
                then
                        negativeMentions=$(($negativeMentions+1))
                        #echo $line
                        #echo "Neg:" $negativeMentions
                fi
        done
        if [[ $(date +%s) > $writeTime ]] #since we couldn't sleep due to streaming tweets in, the script sets a write time and then checks if current time is greater than write time to limit disc I/O
        then
        loop=$(($loop+1))
        echo $loop $positiveMentions >> positiveTimeseries.txt
        echo $loop $negativeMentions >> negativeTimeseries.txt
        positiveMentions=0
        negativeMentions=0
        time=$(date +%s) 
        writeTime=$(($time+$interval))#set next write time
        fi
done
