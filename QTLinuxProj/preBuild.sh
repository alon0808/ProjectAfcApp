#!/bin/sh

source "/appConst.sh"

echo "$LINE2"
pwd
echo "perBuild copy the source and project files to compile directory"

echo "$LINE3"
echo "cp -ru ${WORKDIR}${CODEDIR} ${COMPILEDIR}"
cp -ru ${WORKDIR}${CODEDIR} ${COMPILEDIR}
echo "$LINE3"
echo cp -ru ${WORKDIR}${QTPROJDIR}Handan* ${COMPILEDIR}${QTPROJDIR}
cp -ru ${WORKDIR}${QTPROJDIR}Handan* ${COMPILEDIR}${QTPROJDIR}
echo "$LINE3"
#echo "copy some bash files"
#cp -ru ${WORKDIR}${QTPROJDIR}*.sh ${COMPILEDIR}${QTPROJDIR}

