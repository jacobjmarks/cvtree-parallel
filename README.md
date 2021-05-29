# cvtree-parallel
**Parallelization of Genome Sequence Comparison**

Completed for academic work - Queensland University of Technology
### [View Report](./assets/9188100%20Report.pdf)

## Source
`cvtree-sequential-original.sh` - Original sequential program. Does not compile nor run in this environment.

`cvtree-sequential.cpp` - Improved sequential program.

`cvtree-parallel.cpp` - Parallelized program using OpenMP.

## Usage

The compiled program (either sequential or parallel) takes a single parameter indicating the data directory containing the genome sequences.*

To quickly compile and run the cvtree on sequences within the `data/` directory, the scripts `sequential.sh` and `parallel.sh` can be used.

These shell scripts run as follows:

#### `sequential.sh`
```
rm -f sequential.out
g++ -O2 cvtree-sequential.cpp -o sequential.out
./sequential.out data/
rm -f sequential.out
```
#### `parallel.sh`
```
rm -f parallel.out
g++ -O2 -fopenmp cvtree-parallel.cpp -o parallel.out
./parallel.out data/
rm -f parallel.out
```

*Please note that a full relative data directory path must be specified, with trailing forward slashes, as seen above.

## Output

The program will output a `.csv` file containing the results of the correlation matrix calculated during execution, for each unique pair of sequences.
