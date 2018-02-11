TESTS=( "./p_input/p*" )
PROG="multiplication.x"
TESTS_BASE_DIR="p_out"
OUT_FILE="output.txt"
i=1

if [ ! -e "p_out" ]; then
	mkdir p_out
fi

for TEST in ${TESTS[@]}
do
	CURR_DIR="$PWD"
	DIR="$TESTS_BASE_DIR/$i"
	if [ -e "$DIR" ]; then
		rm -rf "$DIR"
	fi
	mkdir "$DIR"

	pushd &>/dev/null

	while IFS= read -r line
	do		

		CMD="$CURR_DIR/$PROG perf $line"
		$CMD &>> $DIR/$OUT_FILE

	done <"$TEST"

	
	popd &>/dev/null
	i=$((i+1))
done
