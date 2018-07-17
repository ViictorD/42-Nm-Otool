#!/bin/sh
for file in "$@"
do
	echo "$file"
	./ft_nm "$file" &> output_his
	nm "$file" &> output_real
	diff output_his output_real
	echo "$file"
done
