#include "catch2/catch2.hpp"
#include "ENRICCamValve.h"
#ifndef PI
#define PI 3.14159265
#endif


TEST_CASE("init gives a pointer to a  initialized device", "[ENRICCamValve]") {
    double rMin = 10;
    double rMax = 15;
    double lenValve = 30;
    double diamValve = 15;
    double Alpha = 0;
    double Gamma = PI / 6;

    ENRICdevice * device = ENRICinitDevice(rMin, rMax, lenValve, diamValve, Alpha, Gamma);
    REQUIRE( device != NULL);

    REQUIRE( device->rMin == rMin);
    REQUIRE( device->rMax == rMax);
    REQUIRE( device->lenValve == lenValve);
    REQUIRE( device->diamValve == diamValve);

    ENRICdelete(device);    
}

TEST_CASE("checkIntegrity returns 1 when device parameters don't respect contrains", "[ENRICCamValve]") {
    double rMin = 10;
    double rMax = 15;
    double lenValve = 30;
    double diamValve = 15;
    double Alpha = 0;
    double Gamma = PI / 6;

    ENRICdevice * device = new(nothrow) ENRICdevice;
    
    device->rMin = rMin;
    device->rMax = rMax;
    device->lenValve = lenValve;
    device->diamValve = diamValve;

    //Parameter <= 0
    device->rMin = -5;
    REQUIRE( ENRICcheckIntegrity(device) == 1 );
    device->rMin = 0;
    REQUIRE( ENRICcheckIntegrity(device) == 1 );
    device->rMin = rMin;

    //Cam constrain
    device->rMin = 6;
    device->rMax = 4;
    REQUIRE( ENRICcheckIntegrity(device) == 2);
    device->rMin = rMin;
    device->rMax = rMax;

    //Lift over valve dimension test
    device->rMax = 10;
    device->rMin = 3;
    device->lenValve = 5;
    REQUIRE( ENRICcheckIntegrity(device) == 3 );
    device->rMin = rMin;
    device->rMax = rMax;
    device->lenValve = lenValve;
    
    ENRICdelete(device);    
}

TEST_CASE("init returns NULL if device parameter don't respect constrain", "[ENRICCamValve]") {
    double rMin = 5;
    double rMax = 15;
    double lenValve = 8;
    double diamValve = 15;
    double Alpha = 0;
    double Gamma = PI / 6;

    ENRICdevice * device = ENRICinitDevice(rMin, rMax, lenValve, diamValve, Alpha, Gamma);
    REQUIRE( device == NULL);
    
    ENRICdelete(device);    
}

TEST_CASE("TEST setrMin when parameters doesn't respect constrains", "[ENRICCamValve]") {
    double rMin = 25;
    double rMax = 40;
    double lenValve = 100;
    double diamValve = 20;
    double Alpha = 0;
    double Gamma = PI / 6;


    ENRICdevice * device = ENRICinitDevice(rMin, rMax, lenValve, diamValve, Alpha, Gamma);

    REQUIRE( ENRICsetrMin(device,39) == 0 );
    REQUIRE( device->rMin == 39 );

    REQUIRE( ENRICsetrMin(device,41) == 1 );
    REQUIRE( device->rMin == 39 );

    ENRICdelete(device);  
}

TEST_CASE("TEST setrMax when parameters doesn't respect constrains", "[ENRICCamValve]") {
    double rMin = 25;
    double rMax = 40;
    double lenValve = 100;
    double diamValve = 20;
    double Alpha = 0;
    double Gamma = PI / 6;

    ENRICdevice * device = ENRICinitDevice(rMin, rMax, lenValve, diamValve, Alpha, Gamma);

    REQUIRE( ENRICsetrMax(device,26) == 0 );
    REQUIRE( device->rMax == 26 );

    REQUIRE( ENRICsetrMax(device,24) == 1 );
    REQUIRE( device->rMax == 26 );

    ENRICdelete(device);  
}

TEST_CASE("TEST lenValve when parameters doesn't respect constrains", "[ENRICCamValve]") {
    double rMin = 25;
    double rMax = 40;
    double lenValve = 100;
    double diamValve = 20;
    double Alpha = 0;
    double Gamma = PI / 6;

    ENRICdevice * device = ENRICinitDevice(rMin, rMax, lenValve, diamValve, Alpha, Gamma);

    REQUIRE( ENRICsetlenValve(device,16) == 0 );
    REQUIRE( device->lenValve == 16 );

    REQUIRE( ENRICsetlenValve(device,14) == 1 );
    REQUIRE( device->lenValve == 16 );

    ENRICdelete(device);  
}
