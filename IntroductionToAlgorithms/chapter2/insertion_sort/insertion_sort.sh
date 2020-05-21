#!/bin/bash

#it is wrong code
#

function insertion_sort()
{
    args=$@
    i=0
    for item in $args
    do
        j=$(expr $i - 1)
        if [ $i -gt 0 ]; then
            if [ $j -ge 0 ]; then
                if [ ${args[${j}]} -gt ${item} ];then
                    n=$(expr $j + 1)
                    ${args[$n]} = ${args[$j]}
                fi
                j=$(expr $j - 1)
            fi
        fi
        i=$(expr $i + 1)
    done
    echo ${args}
}

insertion_sort 1 3 4 2 9 7 6