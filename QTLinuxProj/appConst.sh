#!/bin/sh
LINE1="========================================="
LINE2="-----------------------------------------"
LINE3="*****************************************"

APPDIR="/mnt/disk0/ltyapp/"
HOSTIP="192.168.0.88"
APPNAME="HandanLinux"
COMPILEDIR="/home/aaa/Desktop/HandanWorkSpace/"
WORKDIR="/mnt/hgfs/afcapp/ProjectAfcApp/"
CODEDIR="code/"
QTPROJDIR="QTLinuxProj/"


function num_yu()
{
	bit1=`echo "obase=2;$1" | bc`
    bit2=`echo "obase=2;$2" | bc`
    len1=${#bit1}
    len2=${#bit2}
    if [ "$len1" -gt "$len2" ]
    then
        len=$len1
        bit2=`printf "%0${len}d" $bit2`
        #echo $bit2
    elif [ "$len1" -lt "$len2" ]
    then
        len=$len2
        bit1=`printf "%0${len}d" $bit1`
        #echo $bit1
    else
        len=$len1
    fi
 
    for ((i=1;i<=len;i++))
    do
        b1=${bit1:0-$i:1}
        b2=${bit2:0-$i:1}
        if [ $b1 -eq 1 ] && [ $b2 -eq 1 ]
        then
            b=1
        else
            b=0
        fi
        result=`echo "$b""$result"`
    done
    result=`echo "obase=10;ibase=2;$result" | bc`
    echo $result
}
