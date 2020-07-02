/**
 * LP functions test cases
 */
#include <catch2/catch.hpp>
#include <glm/glm.hpp>
#include <cmath>
#include "lp.h"


TEST_CASE( "LP manually setting constratints", "" ) {

    int numConstraints = 64;
    int numBatches = 1;

    //Initialise lib
    lplibInit(numConstraints, numBatches);

    //Gets the contstrain and optimise variables
    auto constraints = lplibGetConstraints();
    auto constraintsCount = lplibGetConstraintsCount();
    auto optimise = lplibGetOptimise();

    // Set constraint and optimise variables
    constraints[0] = make_float4(-11,4,0,0.9090);
    constraints[1] = make_float4(-2,6,0,6);
    constraintsCount[0] = 2;

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

TEST_CASE( "LP set constraints using API call", "" ) {

    int numConstraints = 64;
    int numBatches = 1;

    //Initialise lib
    lplibInit(numConstraints, numBatches);

    float4* constraints = (float4*) malloc(sizeof(float4)* numConstraints);

    // Set constraint and optimise variables
    constraints[0] = make_float4(-11,4,0,0.9090);
    constraints[1] = make_float4(-2,6,0,6);

    auto target = glm::vec2(1,1);
    lplibSetBatch(0, constraints, numConstraints, &target);

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

    // Runs the benchmarking for various number of constraints in batches of 1024
    REQUIRE( lplibBenchmark("benchmarks/2", 1024) == 0);
    REQUIRE( lplibBenchmark("benchmarks/4", 1024) == 0);
    REQUIRE( lplibBenchmark("benchmarks/64", 1024) == 0);
    REQUIRE( lplibBenchmark("benchmarks/128", 1024) == 0);
    REQUIRE( lplibBenchmark("benchmarks/256", 1024) == 0);

    
}
