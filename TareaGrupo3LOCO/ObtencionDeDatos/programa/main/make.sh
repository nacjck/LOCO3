rm -rf obj
make
./main -s 5 -c 0 "kodim07.pgm" "xd.bin" #kodim20.pgm
./main -d "xd.bin" "res.pgm"
