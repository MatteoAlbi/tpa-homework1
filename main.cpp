#include <iostream>

#include "LBAMTTdevice.h"
#include "ENRICCamValve.h"
#include "LBAMTTmotor.h"

using namespace std;


int main(int argc, char ** argv) {

    LBAMTTcommandLineParam(argc, argv);

    int n = 2;
    double bore = 140;
    double stroke = 140;
    double displacement = PI * pow(bore/2,2) * stroke * n;
    LBAMTTmotor * motor = LBAMTTinitMotor(n, bore, displacement, 90);

    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor, true), "motor_example.svg");
    LBAMTTmotorFromStringSVG(LBAMTTanimateMotorSVG(motor, LBAMTTinitAnimation(6, 3)));

    LBAMTTanimation * anim = LBAMTTinitAnimation(180, 3);
    LBAMTTanimSetN(anim, 360);
    LBAMTTanimSetT(anim, 3);
    LBAMTTsaveToFile(LBAMTTanimateMotorSVG(motor, anim, true), "anim_motor.svg");
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
