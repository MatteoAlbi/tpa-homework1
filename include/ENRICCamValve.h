#ifndef CAMVALVE
#define CAMVALVE

#include <vector>       //variable lenght vector
#include <fstream>      //file
#include <streambuf>    //file
#include <sstream>      //file
#include <cstdio>

#include <iostream>
#include <math.h>
#include <new>
#include <string.h>

using namespace std;

typedef struct ENRICdevice{
    double rMin; //Minimum cam radius
    double rMax; //Maximum cam radius
    double lenValve; //Valve length
    double diamValve; //Valve diameter
    double Alpha; // Angle of rotation of the cam
    double Gamma; // Important parameter of the cam, check README for more info
    } ENRICdevice;

/**
 * Dinamically allocates a ENRICdevice struct.
 * @param rMin; //Minimum cam radius
 * @param rMax; //Maximum cam radius
 * @param lenValve; //Valve length
 * @param diamValve; //Valve diameter
 * @param Alpha; // Angle of rotation of the cam
 * @param Gamma; // Important parameter of the cam, check README for more info
 * //Checks if the parameters allow the device to be assemblated.
 * //If the parameters don't allow the device to be assemblated, the pointer will be deallocated
 * @return pointer to device
*/
ENRICdevice * ENRICinitDevice (double rMin, double rMax, double lenValve, double diamValve, double Alpha, double Gamma);
/**
 * Checks if the parameters allow the device to be built
 * @param device pointer to device
 * @return 0 if the integrity is respected
 * @return 1 in case of error
*/
int ENRICcheckIntegrity (const ENRICdevice * device);

/**
 * Deallocates the structure pointed by device
 * @param device pointer to the struct that has to be deallocated
 * @return 0 if the checking process was succesfull
 * @return 1 in case of error
*/
int ENRICdelete (ENRICdevice * device);

/**
 * Changes the parameter rMin of the struct pointed by device
 * @param device pointer to the struct
 * @param rMin new value
 * @return 0 if the change was succesfull
 *      1 in case of error, and sets rMin back to the original value
*/
int ENRICsetrMin (ENRICdevice * device, double rMin);

/**
 * Changes the parameter rMax of the struct pointed by device
 * @param device pointer to the struct
 * @param rMax new value
 * @return 0 if the change was succesfull
 *      1 in case of error, and sets rMax back to the original value
*/
int ENRICsetrMax (ENRICdevice * device, double rMax);

/**
 * Changes the parameter lenValve of the struct pointed by device
 * @param device pointer to the struct
 * @param lenValve new value
 * @return 0 if the change was succesfull
 *      1 in case of error, and sets lenValve back to the original value
*/
int ENRICsetlenValve (ENRICdevice * device, double lenValve);

/**
 * Changes the parameter Alpha of the struct pointed by device
 * @param device pointer to the struct
 * @param alpha new value
 * @return 0 if the change was succesfull
 *      1 in case of error, and sets Alpha back to the original value
*/
int ENRICsetAlpha (ENRICdevice * device, double alpha);

/**
 * Creates a string to rapresent a line in SVG, 
 * The two points that define the line need to be different
 * @param x1 coordinate x of the first point
 * @param y1 coordinate y of the first point
 * @param x2 coordinate x of the second point
 * @param y2 coordinate y of the second point
 * @param stroke thickness of the line (default 2) (needs to be > 0)
 * @param color color of the line (default black)
 * @param opt more visual options
 * @return string SVG of the line;
 *      empty in case of error
 */
string ENRIClineSVG(double x1, double y1, double x2, double y2, int stroke = 2, string color = "black", string opt = "");

/**
 * Creates a string to rapresent an arc in SVG format, 
 * the angle needs to be > 0
 * @param cx coordinate x of the center of the arc
 * @param cy coordinata y of the center of the arc
 * @param r radius of the arc (needs to be > 1)
 * @param startAngle starting point of the arc (in degrees)
 * @param endAngle ending point of the arc (in degrees)
 * @param stroke thickness arc (need to be > 0 && < r)
 * @param color arc color
 * @return SVG string of the arc
 *      empty in case of error
 */
string ENRICarcSVG(double cx, double cy, double r, double startAngle, double endAngle, int stroke = 2, string color = "black");

/**
 * Creates a string to rapresent an arc in SVG format, with a beautiful animation
 * the angle needs to be > 0
 * @param cx coordinate x of the center of the arc
 * @param cy coordinata y of the center of the arc
 * @param r radius of the arc (needs to be > 1)
 * @param startAngle starting point of the arc (in degrees)
 * @param endAngle ending point of the arc (in degrees)
 * @param stroke thickness arc (need to be > 0 && < r)
 * @param color arc color
 * @return SVG string of the arc
 *      empty in case of error
 */
string ENRICAnimationarcSVG(double cx, double cy, double r, double startAngle, double endAngle, int stroke = 2, string color = "black");

/**
 * Creates a distance quote between point A and B 
 * A and B need to be different
 * Need to create an arrow at the beginning of the file
 * @param xA coordinate x of A
 * @param yA coordinate y of A
 * @param xB coordinate x of B
 * @param yB coordinate y of B
 * @param distQuote distance of the quote from AB (needs to be > lQuote)
 * @param lQuote lenght of the lateral line of the quote (needs to be >= 1)
 * @param side flag: if true the quote comes at direction theta+90, with theta=arg(AB), if false, the direction is the opposite
 * @return SVG string of the quote;
 *      empty in case of error
 */
string ENRICquoteDistSVG(double xA, double yA, double xB, double yB, double distQuote, double lQuote, bool side);

/**
 * Creates a vector to SVG to show the new device
 * @param device pointer to the device struct to show
 * @param quote if true includes the quote of the device in the drawing, not implemented yet
 * @return String that rapresents a SVG device
*/
string ENRICtoStringSVG (ENRICdevice * device, bool quote = false);

/**
 * Creates a string that defines an arrow shaped marked, it is used in the quote
 * @return SVG string that defines the marker
 */
string ENRICarrowMarkerSVG();

/**
 * Divides a string using a second little string
 * @param s string to divide
 * @param delimeter little string that I need to look for
 * @return divided string put in a vector
*/
vector<string> ENRICsplitString (string s, string delimiter);

/**
 * Saves on a file the passed string
 * @param stringSVG string to save
 * @param fileName name of the file
 * @return 0 if it is succesfull
 *      1 in case of error
*/
int ENRICsaveToFile(string s, string fileName);

/**
 * Reads a file and gives back a string
 * @param fileName name of the file, extension must be .svg
 * @return string from the file, empty in case of error
 */
string ENRICloadFromFile(string fileName);

/**
 * Creates a vector to SVG to show the new device, with a beautiful animation
 * @param device pointer to the device struct to show
 * @param quote if true includes the quote of the device in the drawing, not implemented yet
 * @return String that rapresents a SVG device
*/
string ENRICAnimationtoStringSVG (ENRICdevice * device, bool quote = false);


#endif