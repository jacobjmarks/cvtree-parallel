rm parallel.out
g++ Parallel/cvtree-parallel.cpp -fopenmp -o parallel.out
./parallel.out data/
rm parallel.out