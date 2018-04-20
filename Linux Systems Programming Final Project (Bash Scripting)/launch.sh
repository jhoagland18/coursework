#!/bin/bash
check=0 #initialize variables
graph=false
search=false
report=false
        printf "Welcome to Twitterbot! \n m will monitor performance(MUST BE PUT LAST IN COMMAND LINE) \n s will graph sentiment analysis \n n will run no sentiment analysis \n Example entry: s m \n Please enter any number of commands below:"
        read args #read user defined arguments
        echo $args
        printf "Which word would you like to track on Twitter?"
        read word #read user defined target word
while [ $check -eq 0 ] #run until at least one argument is satisfied
do
        for v in $args #iterate through args
        do
                        if [[ $v == "s" ]] #if user activated sentiment analysis, pipe twiiter stream to sentimentAnalysis.sh
                        then
                                python3.5 107twitterbot.py $word | bash sentimentAnalysis.sh &
                                bash graphsentiment.sh
                                check=1
                        elif [[ $v == "n" ]] #if no sentiment analysis, launch script without pipe
                        then
                                python3.5 107twitterbot.py $word &
                                check=1
                        elif [[ $v == "r" ]] #does nothing
                        then
                                report=true
                                echo report is $report
                                check=1
                        elif [[ $v == "m" ]] #launches monitoring bash script
                        then
                                bash usagecheck.sh $args
                                check=1

                        elif [ $check -eq 1 ] #ensure user entered at least one valid command
                        then
                                break
                        else
                                echo Please enter a valid command
                                read args
                        fi
        done
done
