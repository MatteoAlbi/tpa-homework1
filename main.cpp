#include <iostream>

#include "include/LBAMTTBiellaManovella.h"

using namespace std;


int main(int, char**) {
    double dShaft = 5;
    double stroke = 10;
    double lenBiella = 10;
    double wBiella = 2;
    double hPistone = 3;
    double dPistone = 4;

    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone);
    
    return 0;
} 
