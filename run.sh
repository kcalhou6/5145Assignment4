g++ Transform_A4.cpp -o transform -fopenmp #-O3 -march=native -std=gnu++0x
# static scheduling with 1, 2, 4, 8, 16 threads
./transform 100000000 1 0 #Sequential
./transform 100000000 2 0
./transform 100000000 4 0
./transform 100000000 8 0
./transform 100000000 16 0
# dynamic,1 scheduling with  1, 2, 4, 8, 16 threads
./transform 100000000 1 1
./transform 100000000 2 1
./transform 100000000 4 1
./transform 100000000 8 1
./transform 100000000 16 1
# dynamic,1000 scheduling with  1, 2, 4, 8, 16 threads
./transform 100000000 1 2
./transform 100000000 2 2
./transform 100000000 4 2
./transform 100000000 8 2
./transform 100000000 16 2
# dynamic,100000 scheduling with  1, 2, 4, 8, 16 threads
./transform 100000000 1 3
./transform 100000000 2 3
./transform 100000000 4 3
./transform 100000000 8 3
./transform 100000000 16 3