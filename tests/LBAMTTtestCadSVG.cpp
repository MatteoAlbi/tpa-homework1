#include "catch2/catch2.hpp"
#include "LBAMTTcadSVG.h"

TEST_CASE("test rectSVG when error occures", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTrectSVG(0, 0, 0, 0, "black") == "");
}

TEST_CASE("test circleSVG when error occures", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTcircleSVG(0, 0, 0, "black") == "");
}

TEST_CASE("test lineSVG when error occures", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTlineSVG(0, 0, 0, 0) == "");
    REQUIRE(LBAMTTlineSVG(0, 0, 0, 1, 0) == "");
}

TEST_CASE("test arcSVG when error occures", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTarcSVG(0, 0, 0.5, 10, 20, 1) == "");
    REQUIRE(LBAMTTarcSVG(0, 0, 10, 10, 20, 0) == "");
    REQUIRE(LBAMTTarcSVG(0, 0, 10, 10, 20, 11) == "");
    REQUIRE(LBAMTTarcSVG(0, 0, 10, 20, 20, 1) == "");
}

TEST_CASE("test quoteDistSVG when error occures", "[LBAMTTcadSVG]"){
    REQUIRE(LBAMTTquoteDistSVG(0, 0, 0, 0, 10, 5) == "");
    REQUIRE(LBAMTTquoteDistSVG(0, 0, 0, 10, 10, 0.5) == "");
    REQUIRE(LBAMTTquoteDistSVG(0, 0, 0, 10, 10, 20) == "");
}

TEST_CASE("test quoteAngleSVG when error occures", "[LBAMTTcadSVG]"){
    //REQUIRE(LBAMTTquoteAngleSVG(0, 0, 0, 0, 10, 5) == "");
    REQUIRE(LBAMTTquoteAngleSVG(0, 0, 0, 10, 10, 0.5) == "");
    REQUIRE(LBAMTTquoteAngleSVG(0, 0, 0, 10, 10, 20) == "");
}