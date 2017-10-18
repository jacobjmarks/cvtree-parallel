rm -f sequential.out
g++ -O2 cvtree-sequential.cpp -o sequential.out
./sequential.out data/
rm -f sequential.out