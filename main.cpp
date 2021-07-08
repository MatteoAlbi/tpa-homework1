#include <iostream>

#include "LBAMTTdevice.h"
#include "ENRICCamValve.h"
#include "LBAMTTmotor.h"

using namespace std;


int main(int argc, char ** argv) {

    LBAMTTcommandLineParam(argc, argv);

    int n = 4;
    double bore = 150;
    double stroke = 140;
    double displacement = PI * pow(bore/2,2) * stroke * n;
    LBAMTTmotor * motor = LBAMTTinitMotor(n, bore, displacement, 90);

    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor, false, true), "motor1.svg");
    LBAMTTsetMotorAngle(motor, 630);
    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor, false, true), "motor2.svg");
    for(int i=0;i<motor->n;i++){
        cout << motor->cylinders[i]->valveDx->Alpha << ", " << motor->cylinders[i]->valveSx->Alpha << endl;
    }
    //multiple test with angle from 0 a 330, step 15
    // int n = 24
    // for(int i=0;i<n;i++){
    //     string s = "device_example" + to_string(i) + ".svg";
    //     LBAMTTdeviceSetAngle(device, i*360/n);
    //     LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device, 400, 200, true), s);
    // }

    LBAMTTdelete(motor);

    return 0;
} 
