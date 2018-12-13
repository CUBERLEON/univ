#define CATCH_CONFIG_MAIN

#include <catch/catch.hpp>
#include <cmath>

#include "obstacle.h"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}

const float EPS = 1e-5;

TEST_CASE("Serialization and deserialization", "[obstacle]") {
    Obstacle t({10.2, 15.6}, 11, 0.2);
    SECTION("serialization") {
        json r = t.serialize();
        REQUIRE( fabs((float)r["pos"]["x"] - 10.2) < EPS );
        REQUIRE( fabs((float)r["pos"]["y"] - 15.6) < EPS );
        REQUIRE( fabs((float)r["radius"] - 11) < EPS );
        REQUIRE( fabs((float)r["factor"] - 0.2) < EPS );
    }
    SECTION("deserialiation") {
        
    }
}