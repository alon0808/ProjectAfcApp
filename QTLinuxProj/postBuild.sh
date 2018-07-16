#!/bin/sh

source "/appConst.sh"

OUTDIR=output/linux/
COMPILE=Bin/
isCpOut=$1
code=0
out=0

echo "$LINE2"
pwd
echo "postBuild copy the compile files to source and project files"

echo $@"+"$*

for i in $@; do
   echo $i
   if [ $i = "code" ]; then
	code=1
   fi
   if [ $i = "out" ]; then
	out=1
   fi
done
echo $code"+"$out

if [ $code = 1 ]; then
	echo "$LINE3"
	echo "cp -ru ${COMPILEDIR}${CODEDIR} ${WORKDIR}"
	cp -ru ${COMPILEDIR}${CODEDIR} ${WORKDIR}
	echo "$LINE3"
	echo cp -ru ${COMPILEDIR}${QTPROJDIR}Handan* ${WORKDIR}${QTPROJDIR}
	cp -ru ${COMPILEDIR}${QTPROJDIR}Handan* ${WORKDIR}${QTPROJDIR}
	echo "$LINE3"
fi

if [ $out = 1 ]; then
        echo "copy some bash files"${COMPILEDIR}${QTPROJDIR}"*.sh"
        cp -ru ${COMPILEDIR}${QTPROJDIR}*.sh ${WORKDIR}${QTPROJDIR}
	echo "copy execute and lib files"${COMPILEDIR}${QTPROJDIR}${COMPILE}lib*.*.*.*.*
	cp -ru ${COMPILEDIR}${QTPROJDIR}${COMPILE}/${APPNAME} ${WORKDIR}../${OUTDIR}
	cp -ru ${COMPILEDIR}${QTPROJDIR}${COMPILE}lib*.*.*.*.* ${WORKDIR}../${OUTDIR}
fi


