#include <iostream>

#include "include/LBAMTTdevice.h"
#include "ENRICCamValve.h"

using namespace std;


int main(int argc, char ** argv) {

    LBAMTTcommandLineParam(argc, argv);
    int N = 10;
    double dPiston = 15*N;
    double dShaft = dPiston/15*10;
    double stroke = dPiston/15*20;
    double lRod = dPiston/15*22;
    double wRod = lRod/4.5;
    double hPiston = dPiston/15*10;
    double angle = 180;

    
    double rMax = stroke/6;
    double rMin = rMax*5/7;
    double diamValve = dPiston*2/5;
    double lenValve = stroke/3;
    double Alpha = PI*3/4;
    double Gamma = PI /8;
    

    ENRICdevice * deviceE = ENRICinitDevice (rMin, rMax, lenValve, diamValve, Alpha, Gamma);
    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston, angle);

    if(device == NULL) cout << "parameters error" << endl;
    LBAMTTsaveToFile(LBAMTTheaderSVG(LBAMTTdeviceToStringSVG(device, 400, 480, false, false)+ENRICtoStringSVG(deviceE, 400, 80, false, false)), "device_example.svg");
    
    //multiple test with angle from 0 a 330, step 15
    // int n = 24
    // for(int i=0;i<n;i++){
    //     string s = "device_example" + to_string(i) + ".svg";
    //     LBAMTTdeviceSetAngle(device, i*360/n);
    //     LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true), s);
    // }

    LBAMTTdeviceDelete(device);

    return 0;
} 
