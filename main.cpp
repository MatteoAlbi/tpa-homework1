#include <iostream>

#include "LBAMTTdevice.h"
#include "ENRICCamValve.h"
#include "LBAMTTmotor.h"

using namespace std;


int main(int argc, char ** argv) {

    LBAMTTcommandLineParam(argc, argv);

    int n = 4;
    double bore = 140;
    double stroke = 140;
    double displacement = PI * pow(bore/2,2) * stroke * n;
    LBAMTTmotor * motor = LBAMTTinitMotor(n, bore, displacement, 90);

    LBAMTTanimation * anim = LBAMTTinitAnimation(0, 180, 3);
    LBAMTTsaveToFile(LBAMTTanimateMotorSVG(motor, anim, true), "anim_motor.svg");
    LBAMTTanimSetN(anim, 360);
    LBAMTTanimSetT(anim, 6);
    LBAMTTsaveToFile(LBAMTTanimateDeviceSVG(motor->cylinders[0]->piston, 400, 480, anim, true), "anim_device.svg");
    
    //multiple test with angle from 0 a 630, step 30
    // int m = 24;
    // for(int i=0;i<m;i++){
    //     string s = "motor_example" + to_string(i) + ".svg";
    //     LBAMTTsetMotorAngle(motor, i*720/m);
    //     LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor, true), s);
    // }

    //LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("motor_1.svg"));

    LBAMTTdelete(motor);

    return 0;
} 
