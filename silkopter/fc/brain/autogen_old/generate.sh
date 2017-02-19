#!/bin/bash

for i in *.json
do
	printf $i
	printf "\n"
  	../../../autojsoncxx/autojsoncxx.py -i $i
done
