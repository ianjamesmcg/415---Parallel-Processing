TESTS=( "./p_input/p*" )
PROG="multiplication-tau.x"
TESTS_BASE_DIR="tau_out"
OUT_FILE="tau_output.txt"
i=1

if [ ! -e "tau_out" ]; then
	mkdir tau_out
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
	IFS=$'\n'
	for LINE in $(cat $TEST); do

		mpiexec -np 1 tau_exec $CURR_DIR/$PROG perf $LINE\
		 &>> $DIR/$OUT_FILE

		cd MULTI__P_WALL_CLOCK_TIME
		pprof profile.0.0.0 &>> $CURR_DIR/$DIR/$OUT_FILE
		cd ..
		cd MULTI__PAPI_L1_DCM
		pprof profile.0.0.0 &>> $CURR_DIR/$DIR/$OUT_FILE
		cd ..
		cd MULTI__PAPI_L2_TCM
		pprof profile.0.0.0 &>> $CURR_DIR/$DIR/$OUT_FILE
		cd ..
		cd MULTI__PAPI_L3_TCM
		pprof profile.0.0.0 &>> $CURR_DIR/$DIR/$OUT_FILE
		cd ..
		echo "

%			
%
%

				" >> $CURR_DIR/$DIR/$OUT_FILE
	done

	popd &>/dev/null
	
	i=$((i+1))
done
