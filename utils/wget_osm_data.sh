#!/bin/bash

# author: Yitao Li

# script for automating OSM data request (which will probably be useful in the OSM project)
# a detailed error message will be displayed if HTTP request was unsuccessful 

if [ $# -ne 4 ]
then
	echo "Usage: " $0 " <westernmost longtitude> <southernmost latitude> <easternmost longtituderight> <northernmost latitude>" 
	exit 0
fi

if [ `expr $1 \> $3` -eq 1 ]
then
	echo "Error: the westernmost longtitude cannot be greater than the easternnmost longtitude"
	echo "Usage: " $0 " <westernmost longtitude> <southernmost latitude> <easternmost longtituderight> <northernmost latitude>" 
	exit 0
fi

if [ `expr $2 \> $4` -eq 1 ] 
then
	echo "Error: the southernmost latitude cannot be greater than the northernnmost latitude"
	echo "Usage: " $0 " <westernmost longtitude> <southernmost latitude> <easternmost longtituderight> <northernmost latitude>" 
	exit 0
fi

wget -dv http://api.openstreetmap.org/api/0.6/map?bbox=$1,$2,$3,$4 2>&1
