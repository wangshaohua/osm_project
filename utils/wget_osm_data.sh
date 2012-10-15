#!/bin/bash

# script for automating OSM data request
# a detailed error message will be displayed if HTTP request was unsuccessful 

if [ $# -ne 4 ];
then
	echo "Usage: " $0 " <westernmost longtitude> <southernmost lattitude> <easternmost longtituderight> <northernmost lattitude>" 
	exit 0
fi

wget -dv http://api.openstreetmap.org/api/0.6/map?bbox=$1,$2,$3,$4
