#!bin/bash
>timeseries.txt #clear input and output files
>mentions.txt
loops=0;

#graph=true
delay=1
last=0

while true #infinite loop
do
        if [[ $(head -1 mentions.txt) != "" ]] #ensure source file contains data
        then
        current=$(head -1 mentions.txt)
        while [[ $current == "" ]] #we encountered deadlock, so this ensures the value it read from the input file is actually valid
        do
                current=$(head -1 mentions.txt)
        done
                diff=$((current - last))
                last=$current
                echo $loops $diff>>timeseries.txt #outputs number of loops and number of tweets since last loop to timeseries.txt
                loops=$(($loops+1))

                if  [[ $@ == 'g' ]] #graphing
                then
                echo > gnuplot.in
                        echo "set term dumb" >> gnuplot.in
                        echo "set xlabel \"Time ($delay second(s))\"" >> gnuplot.in
                        echo "set ylabel \"Number of Mentions\"" >> gnuplot.in
                        echo "set title \"Mentions over time\"" >> gnuplot.in
                        echo "plot \"/home/root/107Project/timeseries.txt\" using 1:2 with lines" >> gnuplot.in
                gnuplot gnuplot.in
                fi
        fi
echo $current
        sleep $delay
done
