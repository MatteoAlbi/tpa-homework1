#include "catch2/catch2.hpp"
#include "LBAMTTcadSVG.h"

TEST_CASE("test rectSVG in caso di errore", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTrectSVG(0, 0, 0, 0, "black") == "");
}

TEST_CASE("test circleSVG in caso di errore", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTcircleSVG(0, 0, 0, "black") == "");
}

TEST_CASE("test lineSVG in caso di errore", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTlineSVG(0, 0, 0, 0) == "");
    REQUIRE(LBAMTTlineSVG(0, 0, 0, 1, 0) == "");
}

TEST_CASE("test arcSVG in caso di errore", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTarcSVG(0, 0, 0.5, 10, 20, 1) == "");
    REQUIRE(LBAMTTarcSVG(0, 0, 10, 10, 20, 0) == "");
    REQUIRE(LBAMTTarcSVG(0, 0, 10, 10, 20, 11) == "");
    REQUIRE(LBAMTTarcSVG(0, 0, 10, 20, 20, 1) == "");
}

TEST_CASE("test quoteDistSVG in caso di errore", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTquoteDistSVG(0, 0, 0, 0, 10, 5) == "");
    REQUIRE(LBAMTTquoteDistSVG(0, 0, 0, 10, 10, 0.5) == "");
    REQUIRE(LBAMTTquoteDistSVG(0, 0, 0, 10, 10, 20) == "");
}