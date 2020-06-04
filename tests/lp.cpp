#include <catch2/catch.hpp>
#include "lp.h"
#include <cmath>

TEST_CASE( "LP", "[LP]" ) {

    int numConstraints = 2;
    int numBatches = 1;

    //Initialise lib
    lplibInit(numConstraints, numBatches);

    //Gets the contstrain and optimise variables
    auto constraints = lplibGetConstraints();
    auto optimise = lplibGetOptimise();

    // Set constraint and optimise variables
    constraints[0] = make_float4(-11,4,0,0.9090);
    constraints[1] = make_float4(-2,6,0,6);

    optimise[0] = glm::vec2(1,1);

    //Error if too many batches requested
    REQUIRE_THROWS(lplibSolve(numBatches+1));

    //Solve the batches
    lplibSolve(numBatches);

    //Get the output variable
    auto output = lplibGetOutput();
    float epsilon = 0.001;

    //Check output value
    REQUIRE( output[0].x == Approx(1.93107f).epsilon(0.001));
    REQUIRE( output[0].y == Approx(0.20679f).epsilon(0.001));

    //Clear lib
    lplibClear();

    
}


TEST_CASE( "Benchmarking", "[benchmarking]" ) {

    
    REQUIRE( lplibBenchmark("benchmarks/2", 1) == 0);

    
}
