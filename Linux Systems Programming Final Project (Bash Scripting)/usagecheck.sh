#!/bin/bash
PID=$(ps -efl | awk '{if($15~/python/)print $4}') #get python script process ID
cpu=0
while [ 1 -eq 1 ] #infinite loop
do
cpu=$(top -n 1 -p $PID | awk '{if(NR==8)print $10}') #get cpu usage of python process
if [[ $cpu >=80 ]] #if cpu too high, kill python
then
        kill -9 $PID
        bash fireart.sh
        exit 1
fi
sleep 1 #loop delay
done
