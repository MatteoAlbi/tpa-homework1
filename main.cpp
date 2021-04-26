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
    double angle = 90;


    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone, angle);
    if(device == NULL) cout << "errore parametri" << endl;
    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true), "prova-biella-manovella.svg");
    
    //test multiplo con angle da 0 a 330, passo 30
    // else{
    //     for(int i=0;i<12;i++){
    //         string s = "prova-biella-manovella" + to_string(i) + ".svg";
    //         LBAMTTsetAngle(device, i*30);
    //         LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true), s);
    //     }
    // }
    
    return 0;
} 
