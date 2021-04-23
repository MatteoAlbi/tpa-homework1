#include <iostream>

#include "include/LBAMTTBiellaManovella.h"

using namespace std;


int main(int, char**) {
    double dShaft = 120;
    double stroke = 300;
    double lenBiella = 300;
    double wBiella = 60;
    double hPistone = 100;
    double dPistone = 150;
    double angle = 180;


    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone, angle);
    if(device == NULL) cout << "errore parametri" << endl;
    else{
        //cout << LBAMTTtoStringSVG(device);
        LBAMTTsaveToFile(LBAMTTtoStringSVG(device, 400, 400),"prova-biella-manovella.svg");
    }
    return 0;
} 
