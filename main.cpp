#include <iostream>

#include "include/LBAMTTdevice.h"

using namespace std;


int main(int argc, char ** argv) {

    LBAMTTcommandLineParam(argc, argv);

    double dShaft = 120;
    double stroke = 300;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 150;
    double angle = 90;


    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston, angle);
    if(device == NULL) cout << "parameters error" << endl;
    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 400, true), "device_example.svg");
    
    //multiple test with angle from 0 a 330, step 30
    // else{
        for(int i=0;i<12;i++){
            string s = "device_example" + to_string(i) + ".svg";
            LBAMTTdeviceSetAngle(device, i*30);
            LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 400, true), s);
        }
    // }

    LBAMTTdeviceDelete(device);
    
    return 0;
} 
