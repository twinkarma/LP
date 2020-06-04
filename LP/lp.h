#include <string>
#include <cuda_runtime.h>
#include <glm/glm.hpp>

/**
 * @brief Initialise or re-initialise the library. Pre-allocates CUDA memory for solver.
 * 
 * @param numMaxConstraints Maximum number of constrains per batch.
 * @param numMaxBatches Maximum number of batches that can be solved at once.
 */
void lplibInit(int numMaxConstraints, int numMaxBatches);

/**
 * @brief Solves linear program for all batches.
 * 
 * 
 * @param numBatches 
 */
void lplibSolve(int numBatches);

/**
 * @brief Clear all solver variables.
 * 
 */
void lplibClear();

/**
 * @brief Runs benchmarking on the library
 * 
 * @param fileName Path to the benchmark file e.g. benchmark/1 , the function will load benchmark/1_A.txt, 1_B.txt and 1_C.txt to be used in the benchmarking.
 * @param numBatches Number of batches to run. Values from the benchmark files will be duplicated across batches.
 * @return int Function status, 0 is successful
 */
int lplibBenchmark(std::string fileName, int numBatches);


float4* lplibGetConstraints();
glm::vec2* lplibGetOptimise();
glm::vec2* lplibGetOutput();