g++ -pg -no-pie -fopenmp Parallel/cvtree-parallel.cpp -o gprof-parallel.out
./gprof-parallel.out data/
gprof gprof-parallel.out gmon.out > profile-parallel.txt
rm gprof-parallel.out && rm gmon.out