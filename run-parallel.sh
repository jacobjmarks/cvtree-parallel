rm -f parallel.out
g++ -O2 -fopenmp cvtree-parallel.cpp -o parallel.out
./parallel.out data/