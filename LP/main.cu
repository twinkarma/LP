#include <cuda_runtime.h>
#include "device_launch_parameters.h"
#include "cuda_profiler_api.h"
#include <glm/glm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileIO.h"
#include "Auxilary.h"
#include "lp.h"


////////////////////////////////////
//Main

int main(int argc, const char* argv[])
{

	
	//------------------------------------------
	//handle args
	if (argc != 3) {
		printf("\nIncorrect Number of Arguments!\n");
		printf("Correct Usages/Syntax:\n");
		printf("Argument 1) File-Name -- Name of input file in benchmarks folder. Cannot contain spaces\n");
		printf("Argument 2) Batch-size -- Number of LPs to be solved\n");
		return 1;
	}

	// Run benchmark
	lplibBenchmark(argv[1], atoi(argv[2]));
	
  return 0;
}
