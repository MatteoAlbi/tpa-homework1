#ifndef LBAMTT_MOTOR
#define LBAMTT_MOTOR

#include "ENRICCamValve.h"
#include "LBAMTTdevice.h"

typedef struct LBAMTTcylinder{
    LBAMTTdevice * piston;
    ENRICdevice * valveSx, * valveDx;
} LBAMTTcylinder;

typedef struct LBAMTTmotor{
    int n; //number of cylinders of the motor
    LBAMTTcylinder ** cylinders;
    double angle; //motor shaft's angle
    double * offset; //offset between motor's shaft and cylinders
} LBAMTTmotor;

/**
 * Given the motor struct, set the offset for each cylinder
 * @param motor struct to be modified
 * @return @return 0 if the function succeed;
 *      1 if error occures
 */
int LBAMTTsetOffsets(LBAMTTmotor * motor);

/**
 * ALLOCATE an LBAMTTcylinder structure.
 * @param bore cylinder's bore
 * @param stroke piston's stroke
 * @param angle cyilinder's rotation angle
 * @return pointer to initialized motor structure;
*/
LBAMTTcylinder * LBAMTTinitCylinder (cDbl bore, cDbl stroke, cDbl angle);

/**
 * ALLOCATE an LBAMTTmotor structure.
 * It checks ith the parameters follow the constraints given to the device (see README.md)
 * If the parameters violet the constraints, the structure is DEALLOCATED.
 * @param n number of cylinders of the motor
 * @param bore motor's bore
 * @param displacement motor's displacement
 * @param angle motor shaft's angle
 * @return pointer to initialized motor structure; 
 *      NULL if error occures
*/
LBAMTTmotor * LBAMTTinitMotor (int n, cDbl bore, cDbl displacement, cDbl angle);

/**
 * DEALLOCATE the structure pointed by the given pointer
 * @param motor pointer to structure to be deallocated
 * @return 0 if the function succeed;
 *      1 if error occures
*/
int LBAMTTdeleteMotor(LBAMTTmotor * motor);

#endif