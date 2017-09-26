g++ -pg -no-pie Sequential/cvtree-sequential.cpp -o gprof-sequential.out
./gprof-sequential.out data/
gprof gprof-sequential.out gmon.out > profile-sequential.txt
rm gprof-sequential.out && rm gmon.out