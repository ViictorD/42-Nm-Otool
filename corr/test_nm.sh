#!/bin/sh
for file in "$@"
do
	echo "$file"
	./ft_otool "$file" &> output_his
	otool -t "$file" &> output_real
	diff output_his output_real
	echo "$file"
done