#include <iostream>

#include "include/LBAMTTBiellaManovella.h"

using namespace std;


int main(int argc, char * argv[]) {

    double dShaft = 120;
    double stroke = 300;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 150;
    double angle = 90;


    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston, angle);
    if(device == NULL) cout << "parameters error" << endl;
    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true), "piston_test.svg");
    
    //test multiplo con angle da 0 a 330, passo 30
    // else{
    //     for(int i=0;i<12;i++){
    //         string s = "piston_test" + to_string(i) + ".svg";
    //         LBAMTTsetAngle(device, i*30);
    //         LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true), s);
    //     }
    // }

    LBAMTTdelete(device);
    
    return 0;
} 
