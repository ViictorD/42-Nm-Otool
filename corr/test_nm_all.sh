#!/bin/sh
	./ft_otool "$@" &> output_his
	otool -t "$@" &> output_real
	diff output_his output_real