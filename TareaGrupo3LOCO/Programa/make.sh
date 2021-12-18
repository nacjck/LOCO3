rm -rf obj
make
./main -s 5 -c 1 "kodim20.pgm" "xd.bin" #kodim20.pgm
./main -d "xd.bin" "res.pgm"
