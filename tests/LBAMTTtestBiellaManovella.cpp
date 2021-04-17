
#include "catch2/catch2.hpp"
#include "LBAMTTBiellaManovella.h"


TEST_CASE("init restituisce un puntatore a device inizializzato", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lenBiella = 10;
    double wBiella = 2;
    double hPistone = 3;
    double dPistone = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
    REQUIRE( device != NULL);

    REQUIRE( device->dShaft == dShaft);
    REQUIRE( device->stroke == stroke);
    REQUIRE( device->lenBiella == lenBiella);
    REQUIRE( device->wBiella == wBiella);
    REQUIRE( device->hPistone == hPistone);
    REQUIRE( device->dPistone == dPistone);
    REQUIRE( device->angle == 0.0);
    REQUIRE( device->verse == N);

    LBAMTTdelete(device);    
}

TEST_CASE("checkIntegrity ritorna 1 quando i parametri del device non rispettano i vincoli", "[LBAMTTBiellaManovella]") {
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
    REQUIRE( LBAMTTcheckIntegrity(device) == 2);
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

    LBAMTTdelete(device);    
}

TEST_CASE("init restituisce NULL si parametri del device non rispettano i vincoli", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lenBiella = 7;
    double wBiella = 2;
    double hPistone = 3;
    double dPistone = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
    REQUIRE( device == NULL);
    
    LBAMTTdelete(device);    
}
/*
TEST_CASE("add should insert element at the start of the list", "[LinkedList]") {
    
    labtpa::LinkedList* ll = labtpa::init();
    labtpa::add(ll,"a");
    REQUIRE( std::string(ll->head->label) == "a");
    labtpa::add(ll,"b");
    REQUIRE( std::string(ll->head->label) == "b");
    labtpa::add(ll,"c");
    REQUIRE( std::string(ll->head->label) == "c"); 
    REQUIRE( labtpa::size(ll) == 3);

    labtpa::destroy(ll);
}

TEST_CASE("to_string should return the string format of the list", "[LinkedList]") {
    
    labtpa::LinkedList* ll = labtpa::init();
    labtpa::add(ll,"a");
    labtpa::add(ll,"b");
    labtpa::add(ll,"c");
    REQUIRE( labtpa::to_string(ll) == std::string("LinkedList: c; b; a; ") );  

    labtpa::destroy(ll);
}

//test case search

TEST_CASE("pop should delete last node", "[LinkedList]") {
    
    labtpa::LinkedList* ll = labtpa::init();
    labtpa::add(ll,"a");
    labtpa::add(ll,"b");
    labtpa::add(ll,"c");
    REQUIRE( labtpa::to_string(ll) == std::string("LinkedList: c; b; a; ") );  

    labtpa::destroy(ll);
}

*/