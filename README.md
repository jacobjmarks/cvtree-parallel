# cvtree-parallel
**Parallelization of Genome Sequence Comparison**

Ongoing academic work - Queensland University of Technology

## Usage

The compiled program take a single parameter indicating the data directory containing the genome sequences.

To quickly compile and run the cvtree on sequences within the `data/` directory, the scripts `sequential.sh` and `parallel.sh` can be used.

These shell scripts run as follows:

#### `sequential.sh`
```
rm -f sequential.out
g++ -O2 cvtree-sequential.cpp -o sequential.out
./sequential.out data/
rm -f parallel.out
```
#### `parallel.sh`
```
rm -f parallel.out
g++ -O2 -fopenmp cvtree-parallel.cpp -o parallel.out
./parallel.out data/
rm -f parallel.out
```
