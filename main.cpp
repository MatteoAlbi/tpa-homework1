#include <iostream>

#include "include/LBAMTTdevice.h"
#include "ENRICCamValve.h"

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

    double rMin = 50;
    double rMax = 70;
    double lenValve = 200;
    double diamValve = 80;
    double Alpha = 0*PI;
    double Gamma = PI /5;
    

    ENRICdevice * deviceE = ENRICinitDevice (rMin, rMax, lenValve, diamValve, Alpha, Gamma);

    ENRICsaveToFile(ENRICtoStringSVG(deviceE, 400, 200, true, false), "CamValveOriginal.svg");


    LBAMTTdevice * device = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston, angle);
    if(device == NULL) cout << "parameters error" << endl;
    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true, false), "device_example.svg");
    
    //multiple test with angle from 0 a 330, step 30

    // for(int i=0;i<24;i++){
    //     string s = "device_example" + to_string(i) + ".svg";
    //     LBAMTTdeviceSetAngle(device, i*15);
    //     LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true), s);
    // }

    LBAMTTdeviceDelete(device);
    
    return 0;
} 
