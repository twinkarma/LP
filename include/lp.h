#include <string>
#include <cuda_runtime.h>
#include <glm/glm.hpp>

/**
 *  Initialise or re-initialise the library. Pre-allocates CUDA memory for solver.
 * 
 * @param numMaxConstraints Maximum number of constrains per batch.
 * @param numMaxBatches Maximum number of batches that can be solved at once.
 */
void lplibInit(int numMaxConstraints, int numMaxBatches);


/**
 *  Sets constraints and optimise at the batchIndex position
 * 
 * @param batchIndex Index of batch, must be < numMaxBatches
 * @param constraintsArray The constraints array
 * @param constraintsCount Number of constraints for this batch index, must be < numMaxConstraints
 * @param target The target vector to optimise for
 */
void lplibSetBatch(unsigned int batchIndex, float4* constraintsArray,unsigned int numConstraints, glm::vec2* target);

/**
 *  Solves linear program for all batches.
 * 
 * 
 * @param numBatches 
 */
void lplibSolve(int numBatches);

/**
 *  Clear all solver variables.
 * 
 */
void lplibClear();

/**
 *  Runs benchmarking on the library
 * 
 * @param fileName Path to the benchmark file e.g. benchmark/1 , the function will load benchmark/1_A.txt, 1_B.txt and 1_C.txt to be used in the benchmarking.
 * @param numBatches Number of batches to run. Values from the benchmark files will be duplicated across batches.
 * @return int Function status, 0 is successful
 */
int lplibBenchmark(std::string fileName, int numBatches);

/**
 * Gets the CUDA managed float4 constraints array for setting outside API or passing
 * to external functions.
 * 
 * Each constraint is a half-plane line representing the area where optimisation is bounded
 * 
 * @return float4* 
 */
float4* lplibGetConstraints();

/**
 *  Gets the CUDA managed int constraints count array for setting outside API or passing
 * to external functions.
 * 
 * The number of constraints for the batch index. Each bach can have a 
 * variable number of constraints with a maximum of numMaxBatches
 * set when called from lplibInit()
 * 
 * @return int* 
 */
int* lplibGetConstraintsCount();

/**
 *  Gets the CUDA managed vec2 optimise array for setting outside API or passing
 * to external functions.
 * 
 * The optimise array represents the 'desired' target output before optimisation.
 * 
 * @return glm::vec2* 
 */
glm::vec2* lplibGetOptimise();

/**
 *  Gets the CUDA managed vec2 output array for setting outside API or passing
 * to external functions.
 * 
 * Output represents the optimised target after constraints are applied.
 * 
 * @return glm::vec2* 
 */
glm::vec2* lplibGetOutput();