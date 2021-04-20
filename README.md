# parallel-sso

Parallel Shark Smell Optimization algorithm implementation using MPI (Message Passing Interface). This implementation is based on the algorithm described in the original [research article]( https://doi.org/10.1002/cplx.21634).

## Prerequisites

- C compiler, standard library and related tools
- MPICH library (version 3.3.x)

## Build

Use the following command to build the application:

```sh
make
```

## Run

The user must provide two command line arguments:
- NP: population size
- TC: test case

For example, if the user wishes to run the application on 4 processors with a population size of 30 solution vectors on test case number 1, the following command should be used:

```sh
mpirun -n 4 ./sso 10 1
```

It is possible to get a list of valid test cases by running the application with no arguments.

## License

MIT
