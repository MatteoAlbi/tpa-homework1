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

    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor, true, true), "motor_1.svg");
    LBAMTTsetMotorAngle(motor, 630);
    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor, true, true), "motor_2.svg");
    
    //multiple test with angle from 0 a 330, step 15
    // int m = 24;
    // for(int i=0;i<m;i++){
    //     string s = "motor_example" + to_string(i) + ".svg";
    //     LBAMTTsetMotorAngle(motor, i*720/m);
    //     LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor, true), s);
    // }
    LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("motor_1.svg"));

    LBAMTTdelete(motor);

    return 0;
} 
