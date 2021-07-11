#ifndef LBAMTT_MOTOR
#define LBAMTT_MOTOR

#define PRECISION 0.001

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
 * Compare two double variables. 
 * @param a first double to compare
 * @param b second double to compare
 * @param precision precision of compare 
 * @return true If their difference is lower than precision
 *      false otherways
 */
bool LBAMTTdblCompare(cDbl a, cDbl b, cDbl precision = PRECISION);

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
LBAMTTmotor * LBAMTTinitMotor (const int n, cDbl bore, cDbl displacement, cDbl angle);

/**
 * DEALLOCATE the structure pointed by the given pointer
 * @param cylinder pointer to structure to be deallocated
 * @return 0 if the function succeed;
 *      1 if error occures
*/
int LBAMTTdelete(LBAMTTmotor * cylinder);

/**
 * DEALLOCATE the structure pointed by the given pointer
 * @param motor pointer to structure to be deallocated
 * @return 0 if the function succeed;
 *      1 if error occures
*/
int LBAMTTdelete(LBAMTTmotor * motor);

/**
 * Sets the angles at the correct values of the piston and valves of 
 * each cylinder of the struct pointed by the given pointer.
 * The modifies are based on angle and offsets of the given struct
 * @param motor pointer to struct to be modified
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
 */
int LBAMTTrotateMotor(LBAMTTmotor * motor);

/**
 * Modify n of the struct pointed by the given pointer
 * @param motor pointer to structure to be modified
 * @param n new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
 */
int LBAMTTsetMotorN(LBAMTTmotor * motor, const int n);

/**
 * Modify bore of the struct pointed by the given pointer
 * @param motor pointer to structure to be modified
 * @param bore new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
 */
int LBAMTTsetMotorBore(LBAMTTmotor * motor, cDbl bore);

/**
 * Modify displacement of the struct pointed by the given pointer
 * @param motor pointer to structure to be modified
 * @param displacement new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
 */
int LBAMTTsetMotorDisplacement(LBAMTTmotor * motor, cDbl displacement);

/**
 * Modify angle of the struct pointed by the given pointer
 * @param motor pointer to structure to be modified
 * @param angle new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
 */
int LBAMTTsetMotorAngle(LBAMTTmotor* motor, cDbl angle);

/**
 * Compare two motor structes. 
 * @param a first struct to compare
 * @param b second struct to compare
 * @return true if the structs have the same values
 *      false otherways (also if one pointer is NULL)
 */
bool LBAMTTmotorsCompare(LBAMTTmotor * a, LBAMTTmotor * b);

/**
 * Create an SVG string to represent the cylinder
 * @param cylinder pointer to structure to be represented
 * @param cxShaft x coordinate of the shaft's axis within the drawing area
 * @param cyShaft y coordinate of the shaft's axis within the drawing area
 * @param quote flag: if true quotes the cylinder (default false)
 * @param header flag: if true add the SVG header (default false)
 * @return string cylinderSVG;
 *      EMPTY if error occures
*/
string LBAMTTcylinderToStringSVG (LBAMTTcylinder * cylinder, double cxShaft, double cyShaft, bool quote = false, bool header = false);

/**
 * Create an SVG string to represent the motor
 * @param motor pointer to structure to be represented
 * @param quote flag: if true quotes the motor (default false)
 * @param header flag: if true add the SVG header (default true)
 * @return string motorSVG;
 *      EMPTY if error occures
*/
string LBAMTTmotorToStringSVG(LBAMTTmotor * motor, bool quote = false, bool header = true);

/**
 * Read an SVG string and return the motor associated to the drawing
 * @param s string to be analized, must have the same format of the one created by LBAMTTmotorToStringSVG
 * @return pointer to device device if the function succeed;
 *      NULL if error occures
 */
LBAMTTmotor * LBAMTTmotorFromStringSVG(string s);

/**
 * Create an SVG string to represent the cylinder animated
 * @param cylinder pointer to structure to be represented
 * @param cxShaft x coordinate of the shaft's axis within the drawing area
 * @param cyShaft y coordinate of the shaft's axis within the drawing area
 * @param anim struct that define the animation
 * @param header flag: if true add the SVG header (default false)
 * @return string cylinderSVG;
 *      EMPTY if error occures
*/
string LBAMTTanimateCylinderSVG(LBAMTTcylinder * cylinder, double cxShaft, double cyShaft, LBAMTTanimation * anim, bool header = true);

/**
 * Create an SVG string to represent the motor animated
 * @param motor pointer to structure to be represented
 * @param anim struct that define the animation
 * @param header flag: if true add the SVG header (default false)
 * @return string motorSVG;
 *      EMPTY if error occures
*/
string LBAMTTanimateMotorSVG(LBAMTTmotor * motor, LBAMTTanimation * anim, bool header = true);

#endif