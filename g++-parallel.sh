rm -f parallel.out
g++ -g -fopenmp cvtree-parallel.cpp -o parallel.out
./parallel.out data/