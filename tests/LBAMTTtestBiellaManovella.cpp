
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

    LBAMTTdelete(device);    
}

TEST_CASE("test dei diversi codici di errore di checkIntegrity", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTcheckIntegrity(NULL) == 1);
    
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

    //parametro nullo/minore di zero
    device->dShaft = -5;
    REQUIRE( LBAMTTcheckIntegrity(device) == -1 );
    device->dShaft = 0;
    REQUIRE( LBAMTTcheckIntegrity(device) == -1 );
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
    device->lenBiella = 20;
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
}

TEST_CASE("test delete", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lenBiella = 10;
    double wBiella = 2;
    double hPistone = 4;
    double dPistone = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
    REQUIRE( device != NULL );
    REQUIRE( LBAMTTdelete(device) == 0 );

    dShaft = 0;
    device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
    REQUIRE( device == NULL );
    REQUIRE( LBAMTTdelete(device) == 1 );    
}

//set
TEST_CASE("test setDShaft in caso di errore e di processo avvenuto con successo", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTsetDShaft(NULL, 10) == 1);
    
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

TEST_CASE("test setStroke in caso di errore e di processo avvenuto con successo", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTsetStroke(NULL, 10) == 1);
    
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

TEST_CASE("test setLenBiella in caso di errore e di processo avvenuto con successo", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTsetLenBiella(NULL, 10) == 1);
    
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

TEST_CASE("test setWBiella in caso di errore e di processo avvenuto con successo", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTsetWBiella(NULL, 10) == 1);
    
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

TEST_CASE("test setHPistone in caso di errore e di processo avvenuto con successo", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTsetHPistone(NULL, 10) == 1);
    
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

TEST_CASE("test setDPistone in caso di errore e di processo avvenuto con successo", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTsetDPistone(NULL, 10) == 1);
    
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

TEST_CASE("test setAngle in caso di errore e di processo avvenuto con successo", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTsetAngle(NULL, 10) == 1);
    
    double dShaft = 80;
    double stroke = 200;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);

    REQUIRE( LBAMTTsetAngle(device,90) == 0 );
    REQUIRE( device->angle == 90 );

    LBAMTTdelete(device);  
}

//file e stringhe
TEST_CASE("test deviceToStringSVG in caso di errore", "[LBAMTTBiellaManovella]"){
    REQUIRE(LBAMTTdeviceToStringSVG(NULL,0,0) == "");
}

TEST_CASE("test saveToFile in caso di errori", "[LBAMTTBiellaManovella]"){
    REQUIRE(LBAMTTsaveToFile("test", "") == 1);
    REQUIRE(LBAMTTsaveToFile("test", "test.copia.svg") == 0); //nome valido
    REQUIRE(LBAMTTsaveToFile("test", "test.txt") == 1);
    REQUIRE(LBAMTTsaveToFile("test", ".svg") == 1);
}

TEST_CASE("test saveToFile in caso di processo avvenuto con successo", "[LBAMTTBiellaManovella]"){
    REQUIRE(LBAMTTsaveToFile("test", "test.svg") == 0);

    LBAMTTsaveToFile("test", "test.svg");
    ifstream fin("test.svg");
    REQUIRE(fin.is_open());

    stringstream buffer;
    buffer << fin.rdbuf();
    REQUIRE(buffer.str() == "test");

    fin.close();
}

TEST_CASE("test loadFromFile in caso di errori", "[LBAMTTBiellaManovella]"){
    REQUIRE(LBAMTTloadFromFile("") == "");
    REQUIRE(LBAMTTloadFromFile("test.txt") == "");
    REQUIRE(LBAMTTloadFromFile(".svg") == "");
    REQUIRE(LBAMTTloadFromFile("FILE_INESISTENTE.svg") == "");
}

TEST_CASE("test loadFromFile in caso di processo avvenuto con successo", "[LBAMTTBiellaManovella]"){
    ofstream fout("test.svg");
    fout << "test";
    fout.close();

    REQUIRE(LBAMTTloadFromFile("test.svg") == "test");
}

TEST_CASE("test deviceFromStringSVG in caso di errori", "[LBAMTTBiellaManovella]"){
    string test = "";
    test += LBAMTTrectSVG(0, 0, 10, 10, "black");
    test += LBAMTTrectSVG(0, 0, 10, 10, "black");
    test += LBAMTTcircleSVG(0, 0, 10, "black");
    test += LBAMTTcircleSVG(0, 0, 10, "black");
    REQUIRE(LBAMTTdeviceFromStringSVG(test) == NULL);

    test += LBAMTTrectSVG(0, 0, 10, 10, "black");
    test += LBAMTTrectSVG(0, 0, 10, 10, "black");
    test += LBAMTTcircleSVG(0, 0, 10, "black");
    test += LBAMTTcircleSVG(0, 0, 10, "black");
    REQUIRE(LBAMTTdeviceFromStringSVG(test) == NULL);

    test += LBAMTTcircleSVG(0, 0, 10, "black");
    REQUIRE(LBAMTTdeviceFromStringSVG(test) == NULL);
}

TEST_CASE("test deviceFromStringSVG in caso di processo avvenuto con successo", "[LBAMTTBiellaManovella]"){
    double dShaft = 120;
    double stroke = 300;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 150;
    double angle = 90;

    LBAMTTdevice * device1 = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone, angle);
    LBAMTTdevice * device2 = LBAMTTdeviceFromStringSVG(LBAMTTdeviceToStringSVG(device1, 400, 200, true, false));
    
    REQUIRE(device2->dShaft == dShaft);
    REQUIRE(device2->stroke == stroke);
    REQUIRE(device2->lenBiella == lenBiella);
    REQUIRE(device2->wBiella == wBiella);
    REQUIRE(device2->hPistone == hPistone);
    REQUIRE(device2->dPistone == dPistone);
    REQUIRE(device2->angle == angle);
}






