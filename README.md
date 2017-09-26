# cvtree-parallel
**Parallelization of Genome Sequence Comparison**

Completed for academic work - Queensland University of Technology

## Usage
The compiled program take a single parameter indicating the data directory containing the genome sequences.

To quickly compile and run the cvtree on sequences within the `data/` directory, `sequential.sh` and `parallel.sh` can be used.

These shell scripts run the following commands:

`sequential.sh`
```
rm sequential.out
g++ Sequential/cvtree-sequential.cpp -o sequential.out
./sequential.out data/
```

`parallel.sh`
```
rm parallel.out
g++ Parallel/cvtree-parallel.cpp -fopenmp -o parallel.out
./parallel.out data/
```