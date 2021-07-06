#include "LBAMTTmotor.h"

int LBAMTTsetOffsets(LBAMTTmotor * motor){
    if(motor == NULL) return 1;
    if(motor->n > 4 || motor->n < 1) return 1;

    motor->offset = new double[motor->n];
    motor->offset[0] = 0.0;

    switch (motor->n)
    {
    case 1: //monocylinder
        break;
    case 2: //two cylinders 
        motor->offset[1] = 360.0;
        break;
    case 3: //three cylinders
        motor->offset[1] = 240.0;
        motor->offset[2] = 480.0;
        break;
    case 4: //four cylinders
        motor->offset[1] = 180.0;
        motor->offset[2] = 540.0;
        motor->offset[3] = 360.0;
        break;
    }

    return 0;
}

LBAMTTcylinder * LBAMTTinitCylinder(cDbl bore, cDbl stroke, cDbl angle){
    LBAMTTcylinder * ret = new LBAMTTcylinder;

    //All values respect constraints
    double dShaft = stroke/2;
    double lRod = stroke*1.1;
    double wRod = lRod/4.5;
    double hPiston = bore/15*10;

    double rMax = stroke/6;
    double rMin = rMax*5/7;
    double diamValve = bore*2/5;
    double lenValve = stroke/3;
    double Gamma = PI /8;

    double pistonAngle = fmod(angle, 720);
    /**
     * pistonAngle = 0 -> start compression: both valve closed
     * pistonAngle = 180 -> explosion: both valve closed
     * pistonAngle = 360 -> start expelling: valve Dx open, valve Sx closed
     * pistonAngle = 540 -> start aspiration: valve Sx open, valve Dx closed
    */
    double angleValveSx = PI*3/4 + (pistonAngle * PI/180 / 2);
    double angleValveDx = angleValveSx + PI/2;

    ret->piston = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, bore, pistonAngle);
    ret->valveSx = ENRICinitDevice(rMin, rMax, lenValve, diamValve, angleValveSx, Gamma);
    ret->valveDx = ENRICinitDevice(rMin, rMax, lenValve, diamValve, angleValveDx, Gamma);

    return ret;
}

LBAMTTmotor * LBAMTTinitMotor(int n, cDbl bore, cDbl displacement, cDbl angle){
    LBAMTTmotor * ret = new LBAMTTmotor;
    
    ret->n = n;
    if(LBAMTTsetOffsets(ret) == 1){//n violate constraints
        delete ret;
        return NULL;
    }

    double stroke = displacement / (pow(bore, 2) * PI) / n;

    //controlli
    bool error = false;
    double ratio = bore/stroke;
    if(stroke > 160) error = true;
    else if(ratio < 0.7 || ratio > 2.4) error = true;
    else switch (ret->n){
        case 1:
            if(bore > 384) error = true;
            break;
        case 2: 
            if(bore > 384) error = true;
            break;
        case 3: 
            if(bore > 200) error = true;
            break;
        case 4: 
            if(bore > 150) error = true;
            break;
    }

    if(error){
        delete ret->offset;
        delete ret;
        return NULL;
    }

    ret->angle = angle;
    ret->cylinders = new LBAMTTcylinder* [n];
    for(int i=0; i<n; i++){
        ret->cylinders[i] = LBAMTTinitCylinder(bore, stroke, angle + ret->offset[i]);
    }

    return ret;
}

int LBAMTTdeleteMotor(LBAMTTmotor * motor){
    if(motor == NULL) return 1;

    for(int i=0;i<motor->n;i++){
        delete motor->cylinders[i]->piston;
        delete motor->cylinders[i]->valveDx;
        delete motor->cylinders[i]->valveSx;
        delete motor->cylinders[i];
    }
    delete[] motor->cylinders;
    delete motor->offset;
    delete motor;
    return 0;
} 