
#include "catch2/catch2.hpp"
#include "LBAMTTdevice.h"
#include <cstring>

using std::string;

//init, check, delete
TEST_CASE("test init when parameters follow constraints", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lRod = 10;
    double wRod = 2;
    double hPiston = 3;
    double dPiston = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);
    REQUIRE( device != NULL );

    REQUIRE( device->dShaft == dShaft );
    REQUIRE( device->stroke == stroke );
    REQUIRE( device->lRod == lRod );
    REQUIRE( device->wRod == wRod );
    REQUIRE( device->hPiston == hPiston );
    REQUIRE( device->dPiston == dPiston );
    REQUIRE( device->angle == 0.0 );

    LBAMTTdeviceDelete(device);    
}

TEST_CASE("test error codes of checkIntegrity", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTcheckDeviceIntegrity(NULL) == 1);
    
    double dShaft = 5;
    double stroke = 10;
    double lRod = 10;
    double wRod = 2;
    double hPiston = 4;
    double dPiston = 4;

    LBAMTTdevice * device = new(nothrow) LBAMTTdevice;
    
    device->dShaft = dShaft;
    device->stroke = stroke;
    device->lRod = lRod;
    device->wRod = wRod;
    device->hPiston = hPiston;
    device->dPiston = dPiston;
    device->angle = 0;

    //parameter <= 0
    device->dShaft = -5;
    REQUIRE( LBAMTTcheckDeviceIntegrity(device) == -1 );
    device->dShaft = 0;
    REQUIRE( LBAMTTcheckDeviceIntegrity(device) == -1 );
    device->dShaft = dShaft;

    //crank lenght constraint
    device->stroke = 6;
    REQUIRE( LBAMTTcheckDeviceIntegrity(device) == 2 );
    device->stroke = stroke;

    //piston constraints
    device->hPiston = 2;
    REQUIRE( LBAMTTcheckDeviceIntegrity(device) == 3 );
    device->hPiston = hPiston;
    device->dPiston = 2;
    REQUIRE( LBAMTTcheckDeviceIntegrity(device) == 3 );
    device->dPiston = dPiston;
    
    //lRod cosntraints
    device->lRod = 7;
    REQUIRE( LBAMTTcheckDeviceIntegrity(device) == 4 );
    device->lRod = lRod;

    //wRod constraints
    device->wRod = 1;
    REQUIRE( LBAMTTcheckDeviceIntegrity(device) == 5 );
    device->wRod = 6;
    device->stroke = 12;
    device->lRod = 20;
    device->dPiston = 10;
    device->hPiston = 10;
    REQUIRE( LBAMTTcheckDeviceIntegrity(device) == 5 );

    LBAMTTdeviceDelete(device);    
}

TEST_CASE("test init when parameters violate constraints", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lRod = 7;
    double wRod = 2;
    double hPiston = 3;
    double dPiston = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);
    REQUIRE( device == NULL );
}

TEST_CASE("test delete", "[LBAMTTBiellaManovella]") {
    double dShaft = 5;
    double stroke = 10;
    double lRod = 10;
    double wRod = 2;
    double hPiston = 4;
    double dPiston = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);
    REQUIRE( device != NULL );
    REQUIRE( LBAMTTdeviceDelete(device) == 0 );

    dShaft = 0;
    device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);
    REQUIRE( device == NULL );
    REQUIRE( LBAMTTdeviceDelete(device) == 1 );    
}

//set
TEST_CASE("test setDShaft when succeed and error occures", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTdeviceSetDShaft(NULL, 10) == 1);
    
    double dShaft = 80;
    double stroke = 200;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);

    REQUIRE( LBAMTTdeviceSetDShaft(device,81) == 0 );
    REQUIRE( device->dShaft == 81 );

    REQUIRE( LBAMTTdeviceSetDShaft(device,50) == 1 );
    REQUIRE( device->dShaft == 81 );

    LBAMTTdeviceDelete(device);  
}

TEST_CASE("test setStroke when succeed and error occures", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTdeviceSetStroke(NULL, 10) == 1);
    
    double dShaft = 80;
    double stroke = 200;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);

    REQUIRE( LBAMTTdeviceSetStroke(device,201) == 0 );
    REQUIRE( device->stroke == 201 );

    REQUIRE( LBAMTTdeviceSetStroke(device,50) == 1 );
    REQUIRE( device->stroke == 201 );

    LBAMTTdeviceDelete(device);  
}

TEST_CASE("test setlRod when succeed and error occures", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTdeviceSetLRod(NULL, 10) == 1);
    
    double dShaft = 80;
    double stroke = 200;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);

    REQUIRE( LBAMTTdeviceSetLRod(device,301) == 0 );
    REQUIRE( device->lRod == 301 );

    REQUIRE( LBAMTTdeviceSetLRod(device,50) == 1 );
    REQUIRE( device->lRod == 301 );

    LBAMTTdeviceDelete(device);  
}

TEST_CASE("test setwRod when succeed and error occures", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTdeviceSetWRod(NULL, 10) == 1);
    
    double dShaft = 80;
    double stroke = 200;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);

    REQUIRE( LBAMTTdeviceSetWRod(device,61) == 0 );
    REQUIRE( device->wRod == 61 );

    REQUIRE( LBAMTTdeviceSetWRod(device,100) == 1 );
    REQUIRE( device->wRod == 61 );

    LBAMTTdeviceDelete(device);  
}

TEST_CASE("test sethPiston when succeed and error occures", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTdeviceSetHPiston(NULL, 10) == 1);
    
    double dShaft = 80;
    double stroke = 200;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);
 
    REQUIRE( LBAMTTdeviceSetHPiston(device,101) == 0 );
    REQUIRE( device->hPiston == 101 );

    REQUIRE( LBAMTTdeviceSetHPiston(device,50) == 1 );
    REQUIRE( device->hPiston == 101 );

    LBAMTTdeviceDelete(device);  
}

TEST_CASE("test setdPiston when succeed and error occures", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTdeviceSetDPiston(NULL, 10) == 1);
    
    double dShaft = 80;
    double stroke = 200;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);

    REQUIRE( LBAMTTdeviceSetDPiston(device,101) == 0 );
    REQUIRE( device->dPiston == 101 );

    REQUIRE( LBAMTTdeviceSetDPiston(device,50) == 1 );
    REQUIRE( device->dPiston == 101 );

    LBAMTTdeviceDelete(device);  
}

TEST_CASE("test setAngle when succeed and error occures", "[LBAMTTBiellaManovella]") {
    REQUIRE(LBAMTTdeviceSetAngle(NULL, 10) == 1);
    
    double dShaft = 80;
    double stroke = 200;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 100;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston);

    REQUIRE( LBAMTTdeviceSetAngle(device,90) == 0 );
    REQUIRE( device->angle == 90 );

    LBAMTTdeviceDelete(device);  
}

//file e stringhe
TEST_CASE("test deviceToStringSVG when error occures", "[LBAMTTBiellaManovella]"){
    REQUIRE(LBAMTTdeviceToStringSVG(NULL,0,0) == "");
}

TEST_CASE("test saveToFile when error occures", "[LBAMTTBiellaManovella]"){
    REQUIRE(LBAMTTsaveToFile("test", "") == 1);
    REQUIRE(LBAMTTsaveToFile("test", "test.copia.svg") == 0); //valid name, works
    REQUIRE(LBAMTTsaveToFile("test", "test.txt") == 1);
    REQUIRE(LBAMTTsaveToFile("test", ".svg") == 1);

    remove("test.copia.svg");
}

TEST_CASE("test saveToFile when succeed", "[LBAMTTBiellaManovella]"){
    REQUIRE(LBAMTTsaveToFile("test", "test.svg") == 0);

    LBAMTTsaveToFile("test", "test.svg");
    ifstream fin("test.svg");
    REQUIRE(fin.is_open());

    stringstream buffer;
    buffer << fin.rdbuf();
    REQUIRE(buffer.str() == "test");

    fin.close();
    remove("test.svg");
}

TEST_CASE("test loadFromFile when error occures", "[LBAMTTBiellaManovella]"){
    REQUIRE(LBAMTTloadFromFile("") == "");
    REQUIRE(LBAMTTloadFromFile("test.txt") == "");
    REQUIRE(LBAMTTloadFromFile(".svg") == "");
    REQUIRE(LBAMTTloadFromFile("FILE_INESISTENTE.svg") == "");
}

TEST_CASE("test loadFromFile when succeed", "[LBAMTTBiellaManovella]"){
    ofstream fout("test.svg");
    fout << "test";
    fout.close();

    REQUIRE(LBAMTTloadFromFile("test.svg") == "test");

    remove("test.svg");
}

TEST_CASE("test deviceFromStringSVG when error occures", "[LBAMTTBiellaManovella]"){
    double dShaft = 120;
    double stroke = 300;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 150;
    double angle = 90;

    string test = ">\n\n";
    test += LBAMTTrectSVG(0, 0, lRod, wRod, "black") + "\n";
    test += LBAMTTrectSVG(0, 0, dPiston, hPiston, "black") + "\n";
    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    REQUIRE(LBAMTTdeviceFromStringSVG(test) == NULL);

    test += LBAMTTrectSVG(0, 0, stroke/2, 10, "black", angle, 0, 0) + "\n";
    test += LBAMTTcircleSVG(0, 0, dShaft/2, "black") + "\n";
    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    REQUIRE(LBAMTTdeviceFromStringSVG(test) != NULL); //works

    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    REQUIRE(LBAMTTdeviceFromStringSVG(test) == NULL);
}

TEST_CASE("test deviceFromStringSVG when succeed", "[LBAMTTBiellaManovella]"){
    double dShaft = 120;
    double stroke = 300;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 150;
    double angle = 90;

    string test = ">\n\n";
    test += LBAMTTrectSVG(0, 0, lRod, wRod, "black") + "\n";
    test += LBAMTTrectSVG(0, 0, dPiston, hPiston, "black") + "\n";
    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    test += LBAMTTrectSVG(0, 0, stroke/2, 10, "black", 90-angle, 0, 0) + "\n";
    test += LBAMTTcircleSVG(0, 0, dShaft/2, "black") + "\n";
    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    test += LBAMTTcircleSVG(0, 0, 10, "black") + "\n";
    
    LBAMTTdevice * device = LBAMTTdeviceFromStringSVG(test);
    REQUIRE(device != NULL);
    REQUIRE(device->dShaft == dShaft);
    REQUIRE(device->stroke == stroke);
    REQUIRE(device->lRod == lRod);
    REQUIRE(device->wRod == wRod);
    REQUIRE(device->hPiston == hPiston);
    REQUIRE(device->dPiston == dPiston);
    REQUIRE(device->angle == angle);
}

char** s_arr2c_arr(string * s_arr, int n){
    char ** c_arr = new char*[n];
    for(int i=0; i<n; i++){
        c_arr[i] = new char[s_arr[i].length()+1];
        strcpy(c_arr[i], s_arr[i].c_str());
    }    
    return c_arr;
}

TEST_CASE("test LBAMTTcommandLineParam, helper"){
    int argc = 3;
    string s_arr[] = {"./mainentry", "-h", "test.svg"};
    char **argv = s_arr2c_arr(s_arr, argc);
    
    cout << "\nhelper display:\n";
    LBAMTTcommandLineParam(argc, argv);
    cout << "\n";
}

TEST_CASE("test LBAMTTcommandLineParam, import request"){
    LBAMTTdevice *device_in = LBAMTTinitDevice(80, 200, 300, 60, 100, 100);
    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device_in, 400, 200, true, true), "test.svg");

    char **argv;
    LBAMTTdevice *device_out;
    int argc = 3;
    string s_arr[] = {"./mainentry", "-i", "test.svg"};
    argv = s_arr2c_arr(s_arr, argc);
    
    device_out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(device_out != NULL); //import successful
    REQUIRE(LBAMTTdeviceCompare(device_out, device_in));

    LBAMTTsaveToFile("test", "test.svg");
    device_out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(device_out == NULL); //not a device

    remove("test.svg");
    device_out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(device_out == NULL); //file not found   
}

TEST_CASE("test LBAMTTcommandLineParam, export with params request"){
    double dShaft = 120;
    double stroke = 300;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 150;
    double angle = 90;
    LBAMTTdevice *device_in = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston, angle);
    char **argv; 
    LBAMTTdevice *device_out;
    string s_arr[] = {"./mainentry", "-eq", "400", "200", "test.svg", "-p", to_string(dShaft), to_string(stroke), to_string(lRod), 
                                                                            to_string(wRod), to_string(hPiston), to_string(dPiston), 
                                                                            to_string(angle)};

    int argc = 13;
    argv = s_arr2c_arr(s_arr, argc);
    device_out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(device_out != NULL);
    REQUIRE(LBAMTTdeviceCompare(device_out, device_in));
    REQUIRE(device_out->dShaft == dShaft);

    s_arr[9] = "600";
    argv = s_arr2c_arr(s_arr, argc);
    device_out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(device_out == NULL); //constraints not respected

    argc = 11;
    argv = s_arr2c_arr(s_arr, argc);
    device_out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(device_out == NULL); //missing param

    remove("test.svg");
}

TEST_CASE("test LBAMTTcommandLineParam, import and export request"){
    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(LBAMTTinitDevice(80, 200, 300, 60, 100, 100), 400, 200, true, true), "test.svg");

    int argc = 14;
    string s_arr[] = {"./mainentry", "-i", "test.svg", "-eq", "400", "200", "test_copy.svg", "-p", "80", "200", "300", "60", "100", "80"};
    char **argv = s_arr2c_arr(s_arr, argc);
    LBAMTTdevice * device_out = LBAMTTcommandLineParam(argc, argv);

    REQUIRE(LBAMTTloadFromFile("test.svg") == LBAMTTloadFromFile("test_copy.svg"));

    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(LBAMTTinitDevice(80, 200, 300, 60, 100, 100), 400, 200), "test.svg");
    s_arr[3] = "-e";
    argv = s_arr2c_arr(s_arr, argc);
    device_out = LBAMTTcommandLineParam(argc, argv);

    REQUIRE(LBAMTTloadFromFile("test.svg") == LBAMTTloadFromFile("test_copy.svg"));

    remove("test.svg");
    remove("test_copy.svg");
}