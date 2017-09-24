# cvtree-parallel
**Parallelization of Genome Sequence Comparison**

Completed for academic work - Queensland University of Technology

## Usage
The compiled program take a single parameter indicating the data directory containing the genome sequences.

To quickly compile and run the cvtree on sequences within the `data/` directory, `sequential.sh` and `parallel.sh` can be used.

These shell scripts simply run the following commands:

```
rm [sequential|parallel].out
g++ [Sequential|Parallel]/improved.cpp -o [sequential|parallel].out
./[sequential|parallel].out data/
```
