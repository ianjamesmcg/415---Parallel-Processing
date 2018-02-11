#!/bin/bash

TESTS=( "./input/test*" )
PROG="multiplication.x"
TESTS_BASE_DIR="tests"
OUT_FILE="output.txt"
i=1

if [ ! -e "tests" ]; then
	mkdir tests
fi

for TEST in ${TESTS[@]}
do
	CURR_DIR="$PWD"
	DIR="$TESTS_BASE_DIR/$i"
	if [ -e "$DIR" ]; then
		rm -rf "$DIR"
	fi
	mkdir "$DIR"
	pushd "$DIR" &>/dev/null
	
	CMD="$CURR_DIR/$PROG test $CURR_DIR/$TEST"
	$CMD &> $OUT_FILE

	i=$((i+1))
	popd &>/dev/null
done
