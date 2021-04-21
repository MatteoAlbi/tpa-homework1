
#include "catch2/catch2.hpp"
#include "LBAMTTBiellaManovella.h"

//init, check, delete
TEST_CASE("test init con inizializzazione di parametri che rispettano i vincoli", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lenBiella = 10;
    double wBiella = 2;
    double hPistone = 3;
    double dPistone = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
    REQUIRE( device != NULL );

    REQUIRE( device->dShaft == dShaft );
    REQUIRE( device->stroke == stroke );
    REQUIRE( device->lenBiella == lenBiella );
    REQUIRE( device->wBiella == wBiella );
    REQUIRE( device->hPistone == hPistone );
    REQUIRE( device->dPistone == dPistone );
    REQUIRE( device->angle == 0.0 );
    REQUIRE( device->verse == N );

    LBAMTTdelete(device);    
}

TEST_CASE("test dei diversi codici di errore di checkIntegrity", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lenBiella = 10;
    double wBiella = 2;
    double hPistone = 4;
    double dPistone = 4;

    LBAMTTdevice * device = new(nothrow) LBAMTTdevice;
    
    device->dShaft = dShaft;
    device->stroke = stroke;
    device->lenBiella = lenBiella;
    device->wBiella = wBiella;
    device->hPistone = hPistone;
    device->dPistone = dPistone;
    device->angle = 0;
    device->verse = N;

    //parametro nullo/minore di zero
    device->dShaft = -5;
    REQUIRE( LBAMTTcheckIntegrity(device) == 1 );
    device->dShaft = 0;
    REQUIRE( LBAMTTcheckIntegrity(device) == 1 );
    device->dShaft = dShaft;

    //vincolo lunghezza MANOVELLA
    device->stroke = 6;
    REQUIRE( LBAMTTcheckIntegrity(device) == 2 );
    device->stroke = stroke;

    //vincoli dimensioni PISTONE
    device->hPistone = 2;
    REQUIRE( LBAMTTcheckIntegrity(device) == 3 );
    device->hPistone = hPistone;
    device->dPistone = 2;
    REQUIRE( LBAMTTcheckIntegrity(device) == 3 );
    device->dPistone = dPistone;
    
    //vincolo lunghezza BIELLA
    device->lenBiella = 7;
    REQUIRE( LBAMTTcheckIntegrity(device) == 4 );
    device->lenBiella = lenBiella;

    //vincolo larghezza BIELLA
    device->wBiella = 1;
    REQUIRE( LBAMTTcheckIntegrity(device) == 5 );
    device->wBiella = 6;
    device->stroke = 12;
    device->dPistone = 10;
    device->hPistone = 10;
    REQUIRE( LBAMTTcheckIntegrity(device) == 5 );

    LBAMTTdelete(device);    
}

TEST_CASE("test init passando valori che non rispettano i vincoli", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lenBiella = 7;
    double wBiella = 2;
    double hPistone = 3;
    double dPistone = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
    REQUIRE( device == NULL );
    
    LBAMTTdelete(device);    
}

//set
TEST_CASE("test setDShaft quando il parametro rispetta i vincoli e non", "[LBAMTTBiellaManovella]") {
    double dShaft = 80;
    double stroke = 200;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);

    REQUIRE( LBAMTTsetDShaft(device,81) == 0 );
    REQUIRE( device->dShaft == 81 );

    REQUIRE( LBAMTTsetDShaft(device,50) == 1 );
    REQUIRE( device->dShaft == 81 );

    LBAMTTdelete(device);  
}

TEST_CASE("test setStroke quando il parametro rispetta i vincoli e non", "[LBAMTTBiellaManovella]") {
    double dShaft = 80;
    double stroke = 200;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);

    REQUIRE( LBAMTTsetStroke(device,201) == 0 );
    REQUIRE( device->stroke == 201 );

    REQUIRE( LBAMTTsetStroke(device,50) == 1 );
    REQUIRE( device->stroke == 201 );

    LBAMTTdelete(device);  
}

TEST_CASE("test setLenBiella quando il parametro rispetta i vincoli e non", "[LBAMTTBiellaManovella]") {
    double dShaft = 80;
    double stroke = 200;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);

    REQUIRE( LBAMTTsetLenBiella(device,301) == 0 );
    REQUIRE( device->lenBiella == 301 );

    REQUIRE( LBAMTTsetLenBiella(device,50) == 1 );
    REQUIRE( device->lenBiella == 301 );

    LBAMTTdelete(device);  
}

TEST_CASE("test setWBiella quando il parametro rispetta i vincoli e non", "[LBAMTTBiellaManovella]") {
    double dShaft = 80;
    double stroke = 200;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);

    REQUIRE( LBAMTTsetWBiella(device,61) == 0 );
    REQUIRE( device->wBiella == 61 );

    REQUIRE( LBAMTTsetWBiella(device,100) == 1 );
    REQUIRE( device->wBiella == 61 );

    LBAMTTdelete(device);  
}

TEST_CASE("test setHPistone quando il parametro rispetta i vincoli e non", "[LBAMTTBiellaManovella]") {
    double dShaft = 80;
    double stroke = 200;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
 
    REQUIRE( LBAMTTsetHPistone(device,101) == 0 );
    REQUIRE( device->hPistone == 101 );

    REQUIRE( LBAMTTsetHPistone(device,50) == 1 );
    REQUIRE( device->hPistone == 101 );

    LBAMTTdelete(device);  
}

TEST_CASE("test setDPistone quando il parametro rispetta i vincoli e non", "[LBAMTTBiellaManovella]") {
    double dShaft = 80;
    double stroke = 200;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);

    REQUIRE( LBAMTTsetDPistone(device,101) == 0 );
    REQUIRE( device->dPistone == 101 );

    REQUIRE( LBAMTTsetDPistone(device,50) == 1 );
    REQUIRE( device->dPistone == 101 );

    LBAMTTdelete(device);  
}
