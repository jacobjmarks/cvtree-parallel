rm -f parallel.out
icc -g -fopenmp cvtree-parallel.cpp -o parallel.out
./parallel.out data/