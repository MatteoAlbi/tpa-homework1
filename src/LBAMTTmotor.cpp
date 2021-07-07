#include "LBAMTTmotor.h"

bool dblCompare(cDbl a, cDbl b, cDbl precision){
    if(abs(a-b) < precision) return true;
    else return false;
}

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
    double hPiston = bore/1.5;

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

LBAMTTmotor * LBAMTTinitMotor(const int n, cDbl bore, cDbl displacement, cDbl angle){
    LBAMTTmotor * ret = new LBAMTTmotor;
    
    ret->n = n;
    if(LBAMTTsetOffsets(ret) == 1){//n violate constraints
        delete ret;
        return NULL;
    }

    double stroke = displacement / (pow(bore/2, 2) * PI * n);

    //checks
    bool error = false;
    double ratio = bore/stroke;
    if(stroke > 160) error = true;
    else if(bore < 60) error = true;
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

    if(angle < 0) ret->angle = 720 - fmod(angle, 720);
    else ret->angle =  fmod(angle, 720);
    
    ret->cylinders = new LBAMTTcylinder* [n];
    for(int i=0; i<ret->n; i++){
        ret->cylinders[i] = LBAMTTinitCylinder(bore, stroke, ret->angle + ret->offset[i]);
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

int LBAMTTrotateMotor(LBAMTTmotor * motor){
    if (motor == NULL) return 1;

    for(int i=0; i<motor->n; i++){
        double cylinderAngle = fmod(motor->angle + motor->offset[i], 720);
        motor->cylinders[i]->piston->angle = cylinderAngle;
        motor->cylinders[i]->valveSx->Alpha = PI*3/4 + (cylinderAngle * PI/180 / 2);
        motor->cylinders[i]->valveDx->Alpha = PI*3/4 + (cylinderAngle * PI/180 / 2) + PI/2;
    }
    return 0;
}

int LBAMTTsetMotorN(LBAMTTmotor * motor, const int n){
    if(motor == NULL) return 1;

    //checks
    if(n == motor->n) return 0;
    if(n < 1 || n > 4) return 1;

    //set
    double stroke = motor->cylinders[0]->piston->stroke;
    double bore = motor->cylinders[0]->piston->dPiston;

    for(int i=0;i<motor->n;i++){
        delete motor->cylinders[i]->piston;
        delete motor->cylinders[i]->valveDx;
        delete motor->cylinders[i]->valveSx;
        delete motor->cylinders[i];
    }
    delete[] motor->cylinders;
    delete motor->offset;

    motor->n = n;
    LBAMTTsetOffsets(motor);
    motor->cylinders = new LBAMTTcylinder* [motor->n];  
    for(int i=0; i<motor->n; i++){
        motor->cylinders[i] = LBAMTTinitCylinder(bore, stroke, motor->angle + motor->offset[i]);
    }  

    return 0;
}

int LBAMTTsetMotorBore(LBAMTTmotor * motor, cDbl bore){
    if(motor == NULL) return 1;
    
    //checks
    if(bore == motor->cylinders[0]->piston->dPiston) return 0; //no need to modify

    if(bore < 60) return 1;
    double ratio = bore / motor->cylinders[0]->piston->stroke;
    if(ratio < 0.7 || ratio > 2.4) return 1;
    switch (motor->n){
        case 1:
            if(bore > 384) return 1;
            break;
        case 2: 
            if(bore > 384) return 1;
            break;
        case 3: 
            if(bore > 200) return 1;
            break;
        case 4: 
            if(bore > 150) return 1;
            break;
    }

    //set
    for(int i=0;i<motor->n;i++){
        motor->cylinders[i]->piston->dPiston = bore;
        motor->cylinders[i]->piston->hPiston = bore/1.5;
        motor->cylinders[i]->valveDx->diamValve = bore*2/5;
        motor->cylinders[i]->valveSx->diamValve = bore*2/5;
    }

    return 0;
}

int LBAMTTsetMotorDisplacement(LBAMTTmotor * motor, cDbl displacement){
    if(motor == NULL) return 1;

    //checks
    double bore = motor->cylinders[0]->piston->dPiston;
    double oldStroke = motor->cylinders[0]->piston->stroke;
    double stroke = displacement / (pow(bore/2, 2) * PI) / motor->n;
    if(dblCompare(stroke, oldStroke)) return 0; //no need to modify

    if(stroke > 160) return 1;
    double ratio = bore/stroke;
    if(ratio < 0.7 || ratio > 2.4) return 1;

    //set
    for(int i=0;i<motor->n;i++){
        motor->cylinders[i]->piston->stroke = stroke;
        motor->cylinders[i]->piston->dShaft = stroke/2;
        motor->cylinders[i]->piston->lRod = stroke*1.1;
        motor->cylinders[i]->piston->wRod = motor->cylinders[i]->piston->lRod/4.5;

        motor->cylinders[i]->valveDx->lenValve = stroke/3;
        motor->cylinders[i]->valveDx->rMax = stroke/6;
        motor->cylinders[i]->valveDx->rMin = motor->cylinders[i]->valveDx->rMax*5/7;

        motor->cylinders[i]->valveSx->lenValve = stroke/3;
        motor->cylinders[i]->valveSx->rMax = stroke/6;
        motor->cylinders[i]->valveSx->rMin = motor->cylinders[i]->valveSx->rMax*5/7;
    }

    return 0;
}

int LBAMTTsetMotorAngle(LBAMTTmotor* motor, cDbl angle){
    if(motor == NULL) return 1;

    double newAngle;
    if(angle < 0) newAngle = 720 - fmod(angle,720);
    else newAngle = fmod(angle,720);

    //checks
    if(motor->angle == newAngle) return 0; //no need to modify

    //set
    motor->angle = newAngle;
    LBAMTTrotateMotor(motor);

    return 0;
}
