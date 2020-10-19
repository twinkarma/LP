# Random Batch LP Solver

This is a batch linear program solver for Nvidia GPUs, suitable for solving many 2D problems efficiently. 

To test results from paper "Two-Dimensional Batch Linear Programming on the GPU" please see the "Improvements" branch.

## What this project contains

CMake capabilities to generate a build tool such as make or .sln. The GLM library in `include/glm`. The Catch 2 library for running the test cases.

## How to build

The library can be built for all platforms using [CMake](https://cmake.org/).

Use CMake to generate the relevant makefile, VS solution, e.t.c. Useful CMake flags to set would be adding `-arch=sm_xx` to "CMAKE_CUDA_FLAGS". This ensures that the project is built using a suitable architecture. Requires `xx` to be set to at least 30. Therefor ensure the GPU this is ran on is an NVidia graphics card with compute capability of at least 30.

* Executables are located in `bin/[Debug|Release]`.
* Library files are located in `lib/[Debug|Release]`.

## Running tests

After building, run the executable `test_all` in the `bin/[Debug|Release]` to run all test cases.


## Using the library

First include the `lp.h` from the `include` folder.

```
#include "lp.h"
```

Then initialise the library, setting the `numMaxBatches` that indicates the maximum number of independent LP problems (batch) it will try to solve simultaneously, and the `numMaxConstraints` which is the maximum number of constraints for each batch.

```c++
// Initialise the library to be able to solve a maximum of 64 LP problems simultaneously
// with maximum of 4 constraints each
int numMaxBatches = 64;
int numMaxConstraints = 4;
lplibInit(numMaxConstraints, numMaxBatches)
```

The constraints and target for optimisation must then be set for each batch, note that 
the number of constraints can vary batch by batch, hence the need to specify a constraint count. This can be done using the API call `lplibSetBatch()` or through directly getting the internal variables and setting them manually. 

To use the `lplibSetBatch()`:

```c++
// An example of setting the batch at the 0 index position

// Create a constraint object 
float4* constraints = (float4*) malloc(sizeof(float4)* numConstraints);

// Set constraint and optimise variables
constraints[0] = make_float4(-11,4,0,0.9090);
constraints[1] = make_float4(-2,6,0,6);

// In this 0th batch we're only using 2 constraints
int numConstraints = 2;

// Target vector to optimise for
auto target = glm::vec2(1,1);

lplibSetBatch(0, constraints, numConstraints, &target);
```

Alternatively, the internal varibles can be obtained and set manually by calling `lplibGetConstraints()`, `lplibGetConstraintsCount()`, and `lplibGetOptimise()`. The returned variables are CUDA managed memory and can be set on the host or be passed to another CUDA kernal for GPU-GPU transfer:

```c++
//This example sets batches 0 and 1 where numMaxConstraints is 4

//Gets the contstrain and optimise variables
auto constraints = lplibGetConstraints();
auto constraintsCount = lplibGetConstraintsCount();
auto target = lplibGetOptimise();

//Setting the 0th batch with 2 constraints
int batchIndex = 0;

constraints[batchIndex*numMaxConstraints] = make_float4(-11,4,0,0.9090);
constraints[batchIndex*numMaxConstraints+1] = make_float4(-2,6,0,6);
constraintsCount[batchIndex] = 2;
target[batchIndex] = glm::vec2(1,1);


//Setting the 1st batch with 3 constraints
batchIndex = 1;

constraints[batchIndex*numMaxConstraints] = make_float4(-11,4,0,0.9090);
constraints[batchIndex*numMaxConstraints+1] = make_float4(-2,6,0,6);
constraints[batchIndex*numMaxConstraints+2] = make_float4(-5,9,0,11);
constraintsCount[batchIndex] = 3;
target[batchIndex] = glm::vec2(1,0.5);
```

After the constraints and optimisation, solve the LP problems by calling `lplibSolve(numBatches)`. The `numBatches` state the number of batches that are available to be solved, variable number of batches can be specified each time. 

Results of the optimisation can be obtained by calling `lplibGetOutput()` which returns a CUDA managed array of optimised vectors.

```c++
//Solve the batches
    lplibSolve(numBatches);

    //Get the output variable
    auto output = lplibGetOutput();
    
    //Gets the 0th batch's output
    glm::vec2 output0 = output[0];
```

The `lplibClear()` can be called after the library is no longer needed to free up allocated GPU memory.

Working examples are illustrated in the test cases in `tests/test_lp.cpp`.


## Running the `LP` executable

Once an executable has been generated one can check the program is working correctly. This example uses a release mode build and can be done from the command line by running the following command from the base folder directory for this code:

`"./bin/Release/LP.exe" benchmarks/64 1024`

Or for Linux:

`./bin/Release/LP benchmarks/64 1024`

which should provide the output:

`Batch 0          Optimal location is x: 1.265306 y: 0.448980     value of 2.163265`


### Program Output

The program will output timing to the "timings" folder in a timings.txt file. This file contains lines of the form `%1 %2 %3\n` where %1 is the number of constraints (e.g. 64), %2 is the number of batches (e.g. 1024), %3 is the time in milliseconds that the program took.

The solutions to the linear program are printed to console. Access to these values is accessible within the code by both GPU and CPU code in `output[i]` where i ranges over all batches.


### Custom Data

To use custom data, create 3 files of the form *_A.txt, *_B.txt, *_C.txt. Where * is a consistent name for all 3 files. File *_A.txt starts with a line of 2 numbers, the first is the number of constraints within the file (n) and the dimension of the problem (d) (should always be 2). The remainder of the file is (n) lines of (d) numbers. These represent the left hand side of the constraints A1x + A2y <= B. All should be space-separated when on the same line

File *_B.txt contains (n) lines of 1 number. These represent the right hand side of the constraints A1x + A2y <= B. 

File *_C.txt contains (d) lines of 1 number (d=2). These represent the objective function to maximise. Ensure data is written with respects to maximising objective function, and inequalities are of form Ax <= B.

For solving a minimization (e.g. min ax + by ) there is a corresponding maximization function (max -ax - by) that can always be used.
