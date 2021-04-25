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
    double angle = 240;


    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone, angle);
    if(device == NULL) cout << "errore parametri" << endl;
    else{
        //cout << LBAMTTtoStringSVG(device);
        LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true),"prova-biella-manovella1.svg");
        LBAMTTsetAngle(device, 120);
        LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true),"prova-biella-manovella2.svg");
    }

    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(LBAMTTdeviceFromString(LBAMTTloadFromFile("prova-biella-manovella1.svg")), 400, 200, true), "copia-biella-manovella1.svg");
    
    return 0;
} 
