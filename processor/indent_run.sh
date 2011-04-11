#!/bin/sh
# Verification program to check C++ Beautifiers 'bcpp', 'indent' or cb
############################################################
# Copyright 
# The copyright policy is GNU/GPL.
# Author: Al Dev (Alavoor Vasudevan) alavoor[AT]yahoo.com
############################################################
run_beautify_now()
{
	FNAME=$1
	echo "FNAME" ${FNAME}
	if [ ! -f ${FNAME} ]; then
		echo "Error: The file["${FNAME}"] does not exist!!. Aborting now ...."
		exit
	fi
	aa=`basename $PRGM`
	echo "Running, ["$aa"] on ["${FNAME}"]"
	echo ${PRGM} ${TMP_CPPFILE}.cpp
	${PRGM} ${FNAME}
}
########## Main of program begins here ##################3
#PRGM=/usr/bin/bcpp
#PRGM=/usr/bin/cb
PRGM='astyle --style=gnu'
COMPILER=/usr/bin/g++
TMP_FILE=beautify.tmp
TMP_CPPFILE=beautify-tmp_cppfile
#echo "Enter the C++ file name <default is *.cpp> : "
#read ans
if [ "$ans" = "" -o "$ans" = " " ]; then
	ans="ALL"
else
	FILENAME=$ans
fi

if [ "$ans" != "ALL" ]; then
	run_beautify_now ${FILENAME}
else
	ls *.cpp |
	while read FILENAME 
	do
		run_beautify_now ${FILENAME}
		echo ""
	done
	ls *.h |
	while read FILENAME 
	do
		run_beautify_now ${FILENAME}
		echo ""
	done	
fi

