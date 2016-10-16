#g++ Transform_A4.cpp -o transform -fopenmp -O3 -march=native -std=gnu++0x
## static scheduling with 1, 2, 4, 8, 16 threads
#for t in 1 2 4 8 16; do ./transform 100000000 $t 0; done
## dynamic,1 scheduling with  1, 2, 4, 8, 16 threads
#for t in 1 2 4 8 16; do ./transform 100000000 $t 1 1; done
## dynamic,1000 scheduling with  1, 2, 4, 8, 16 threads
#for t in 1 2 4 8 16; do ./transform 100000000 $t 1 1000; done
## dynamic,100000 scheduling with  1, 2, 4, 8, 16 threads
#for t in 1 2 4 8 16; do ./transform 100000000 $t 1 100000; done

#g++ Reduce_A4.cpp -o reduce -fopenmp -O3 -march=native -std=gnu++0x
## static scheduling with 1, 2, 4, 8, 16 threads
#for t in 1 2 4 8 16; do ./reduce 100000000 $t 0; done
## dynamic,1 scheduling with  1, 2, 4, 8, 16 threads
#for t in 1 2 4 8 16; do ./reduce 100000000 $t 1 1; done
## dynamic,1000 scheduling with  1, 2, 4, 8, 16 threads
#for t in 1 2 4 8 16; do ./reduce 100000000 $t 1 1000; done
## dynamic,100000 scheduling with  1, 2, 4, 8, 16 threads
#for t in 1 2 4 8 16; do ./reduce 100000000 $t 1 100000; done

#g++ NumericalIntegration_A4.cpp -o integrate -fopenmp -O3 -march=native -std=gnu++0x
## dynamic,1 scheduling with 1, 2, 4, 8, 16 threads (x^2 from 1 to 10 over 10^3 points and 10 intensity)
#for t in 1 2 4 8 16; do ./integrate 1 10 1000 0 10 $t 1; done
## dynamic,1 scheduling with 1, 2, 4, 8, 16 threads (x^2 from 1 to 10 over 10^3 points and 1000 intensity)
#for t in 1 2 4 8 16; do ./integrate 1 10 1000 0 1000 $t 1; done
## dynamic,1000 scheduling with 1, 2, 4, 8, 16 threads (x^2 from 1 to 10 over 10^3 points and 10 intensity)
#for t in 1 2 4 8 16; do ./integrate 1 10 1000 0 10 $t 1000; done
## dynamic,1000 scheduling with  1, 2, 4, 8, 16 threads (x^2 from 1 to 10 over 10^3 points and 1000 intensity)
#for t in 1 2 4 8 16; do ./integrate 1 10 1000 0 1000 $t 1000; done
## dynamic,1 scheduling with 1, 2, 4, 8, 16 threads (x^2 from 1 to 10 over 10^9 points and 10 intensity)
#for t in 1 2 4 8 16; do ./integrate 1 10 1000000000 0 10 $t 1; done

## dynamic,1 scheduling with 1, 2, 4, 8, 16 threads (x^2 from 1 to 10 over 10^9 points and 1000 intensity)
#for t in 1 2 4 8 16; do ./integrate 1 10 1000000000 0 1000 $t 1; done
## dynamic,1000 scheduling with 1, 2, 4, 8, 16 threads (x^2 from 1 to 10 over 10^9 points and 10 intensity)
#for t in 1 2 4 8 16; do ./integrate 1 10 1000000000 0 10 $t 1000; done
## dynamic,1000 scheduling with  1, 2, 4, 8, 16 threads (x^2 from 1 to 10 over 10^9 points and 1000 intensity)
#for t in 1 2 4 8 16; do ./integrate 1 10 1000000000 0 1000 $t 1000; done

g++ FindFirst_A4.cpp -o first -fopenmp -O3 -march=native -std=gnu++0x
##Time complexity n
#for t in 1 2 4 8 16; do ./first 100000000 0 $t 0; done
#Time complexity pos with 1, 2, 4, 8, 16 threads
for t in 1 2 4 8 16; do ./first 100000000 70234 $t 1; done

g++ PrefixSum_A4.cpp -o prefix -fopenmp -O3 -march=native -std=gnu++0x
# prefix sum of array length 10^9 for 1 2 4 8 16 threads
for to in 1 2 4 8 16; do ./prefix 1000000000 $t; done
