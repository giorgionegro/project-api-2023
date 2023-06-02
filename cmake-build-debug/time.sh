#time ./api execution time

# Path: cmake-build-debug\time.sh
#time ./api execution time

start=$(date +%s.%N)
./api
dur=$(echo "$(date +%s.%N) - $start" | bc)
printf "Execution time: %.6f seconds" $dur
printf "\n"