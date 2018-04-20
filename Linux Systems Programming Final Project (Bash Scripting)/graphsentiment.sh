#!bin/bash
>positiveTimeseries.txt #clears positive and negative timeseries text files to plot new graph
>negativeTimeseries.txt

delay=1
last=0

while true //infinite loop
do
if [[ $(head -1 positiveTimeseries.txt) != "" ]] #ensure data source files contain data
then

        echo > gnuplot.in #write gnuplot commands to input file to execute later
                echo "set term dumb" >> gnuplot.in
                echo "set xlabel \"Time ($delay second(s))\"" >> gnuplot.in
                echo "set ylabel \"Number of Positive/Negative mentions\"" >> gnuplot.in
                echo "set title \"Sentiment over time\"" >> gnuplot.in
                #echo "set linestyle 1 lc 2" >> gnuplot.in
                echo "plot \"/home/root/107Project/negativeTimeseries.txt\" using 1:2 with lines, \"/home/root/107Project/positiveTimeseries.txt\" using 1:2 with lines" >> gnuplot.in
        gnuplot gnuplot.in
        sleep $delay

fi
done
