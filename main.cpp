#include <iostream>

#include "include/LBAMTTBiellaManovella.h"

using namespace std;


int main(int, char**) {
    double dShaft = 80;
    double stroke = 200;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 100;


    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
    //cout << LBAMTTtoStringSVG(device);
    LBAMTTsaveToFile(LBAMTTtoStringSVG(device),"prova-biella-manovella.svg");
    
    return 0;
} 
