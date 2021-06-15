#ifndef LBAMTT_PISTON
#define LBAMTT_PISTON

#include <iostream>
#include <string>
#include <math.h>
#include <new>
#include <vector>
#include <cstdlib>

#include <fstream>      
#include <streambuf>    
#include <sstream>     
#include <cstdio>

#include "LBAMTTcadSVG.h"

#ifndef PI
#define PI 3.14159265
#endif

#ifndef CONST_DOUBLE
#define CONST_DOUBLE
typedef const double cDbl;
#endif

using namespace std;

typedef struct LBAMTTdevice{
    double dShaft; //shaft's diameter
    double stroke; //piston's stroke
    double lRod; //connecting rod's lenght
    double wRod; //connecting rod's width
    double hPiston; //piston's height
    double dPiston; //piston's diameter
    double angle; //crank rotation's angle
} LBAMTTdevice;

/**
 * ALLOCATE an LBAMTTdevice structure.
 * It checks ith the parameters follow the constraints given to the device (see README.md)
 * If the parameters violet the constraints, the structure is DEALLOCATED.
 * @param dShaft shaft's diameter
 * @param stroke piston's stroke
 * @param lRod connecting rod's lenght
 * @param wRod connecting rod's width
 * @param hPiston piston's height
 * @param dPiston piston's diameter
 * @param angle crank rotation's angle (default 0.0, higher dead point)
 * @return pointer to initialized device structure; 
 *      NULL if error occures
*/
LBAMTTdevice * LBAMTTinitDevice (cDbl dShaft, cDbl stroke, cDbl lRod, cDbl wRod, cDbl hPiston, cDbl dPiston, cDbl angle = 0.0);

/**
 * Checks if the parameters follow the given constraints.
 * @param device pointer to device of wich check the parameters
 * @return 0 if the constraints aren't violated;
 *      -1 if a parameter is <= 0;
 *      1 if pointer == NULL
 *      2 if the crank's constraint is violated;
 *      3 if the piston's constraints are violated;
 *      4 if the connecting rod's lenght constraint is violated;
 *      5 if the connecting rod's width constraints are violated;
*/
int LBAMTTcheckDeviceIntegrity (const LBAMTTdevice * device);

/**
 * DEALLOCATE the structure pointed by the given pointer
 * @param device pointer to structure to be deallocated
 * @return 0 if the function succeed;
 *      1 if error occures
*/
int LBAMTTdeviceDelete (LBAMTTdevice * device);

/**
 * Modify dShaft of the structure pointed by the given pointer
 * @param device pointer to structure to be modified
 * @param dShaft new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
*/
int LBAMTTdeviceSetDShaft (LBAMTTdevice * device, cDbl dShaft);

/**
 * Modify stroke of the structure pointed by the given pointer
 * @param device pointer to structure to be modified
 * @param stroke new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
*/
int LBAMTTdeviceSetStroke (LBAMTTdevice * device, cDbl stroke);

/**
 * Modify lRod of the structure pointed by the given pointer
 * @param device pointer to structure to be modified
 * @param lRod new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
*/
int LBAMTTdeviceSetLRod (LBAMTTdevice * device, cDbl lRod);

/**
 Modify wRod of the structure pointed by the given pointer
 * @param device pointer to structure to be modified
 * @param wRod new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
*/
int LBAMTTdeviceSetWRod (LBAMTTdevice * device, cDbl wRod);

/**
 * Modify hPiston of the structure pointed by the given pointer
 * @param device pointer to structure to be modified
 * @param hPiston new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
*/
int LBAMTTdeviceSetHPiston (LBAMTTdevice * device, cDbl hPiston);

/**
 * Modify dPiston of the structure pointed by the given pointer
 * @param device pointer to structure to be modified
 * @param dPiston new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
*/
int LBAMTTdeviceSetDPiston (LBAMTTdevice * device, cDbl dPiston);

/**
 * Modify angle of the structure pointed by the given pointer
 * @param device pointer to structure to be modified
 * @param angle new value
 * @return 0 if the function succeed;
 *      1 if error occures, and keeps the original value
*/
int LBAMTTdeviceSetAngle (LBAMTTdevice * device, cDbl angle);

/**
 * Create an SVG string to represent the device
 * @param device pointer to structure to be represented
 * @param cxShaft x coordinate of the shaft's axis within the drawing area
 * @param cyShaft y coordinate of the shaft's axis within the drawing area
 * @param quote flag: if true quotes the device (default false)
 * @param header flag: if trueadd the SVG header (default true)
 * @return string deviceSVG;
 *      EMPTY if error occures
*/
string LBAMTTdeviceToStringSVG (LBAMTTdevice * device, double cxShaft, double cyShaft, bool quote = false, bool header = true);

/**
 * Split a string relying on a given substring
 * @param s string to be splitted
 * @param delimeter substring
 * @return vector of the splitted string
*/
vector<string> LBAMTTsplitString (string s, string delimiter);

/**
 * Write on file the given string
 * @param stringSVG string to be written
 * @param fileName file where write the string, must be .svg extension
 * @return 0 if the function succeed;
 *      1 if error occures
*/
int LBAMTTsaveToFile(string s, string fileName);

/**
 * Read a file and return its content
 * @param fileName file to be red, must be .svg extension
 * @return string with the file content if the function succeed;
 *      EMPTY if error occures
 */
string LBAMTTloadFromFile(string fileName);

/**
 * Read an SVG string and return the device associated to the drawing
 * @param s string to be analized, must have the same format of the one created by LBAMTTdeviceToString
 * @return pointer to device device if the function succeed;
 *      NULL if error occures
 */
LBAMTTdevice * LBAMTTdeviceFromStringSVG(string s);

/**
 * Manage the params red by command line
 * @param argc number of params passed
 * @param argv array where the params are stored as char arrays
 * @return pointer to device created following commands
 *      NULL if error occurs or no creation is requested
 */
LBAMTTdevice * LBAMTTcommandLineParam(int argc, char** argv);

#endif