#ifndef LBAMTT_SVG
#define LBAMTT_SVG

#include <string>       
#include <math.h>      

#ifndef PI
#define PI 3.14159265
#endif

#ifndef CONST_DOUBLE
#define CONST_DOUBLE
typedef const double cDbl;
#endif

using namespace std;

/**
 * Modifies the angle value so that is inlcuded between 0 and norm.
 * @param angle angolo to be modified
 * @param norm value of normalization
 * @return modified angle
 */
double LBAMTTnormAng(double angle, double norm = 360.0);

/**
 * Adds the SVG file header.
 * @param s stringa where add the header
 * @return string with the header added
 */
string LBAMTTheaderSVG(string s);

/**
 * Create a string to define an arrow marker, needed for quotes.
 * @return string that define the marker
 */
string LBAMTTarrowMarkerSVG();

/**
 * Create a string to represent an SVG rectangle.
 * @param x x coordinate of the right-lower vertix
 * @param y y coordinate of the right-lower vertix
 * @param w rectangle's width (must be greater than 0)
 * @param h rectangle's height (must be greater than 0)
 * @param color rectangle's color (must be in SVG format)
 * @param rotation rectangle's rotation angle (default 0.0)
 * @param xr x coordinate of the center rotation point (default 0.0)
 * @param yr y coordinate of the center rotation point (default 0.0)
 * @return SVG rectangle string;
 *      EMPTY if error occures
 */
string LBAMTTrectSVG(cDbl x, cDbl y, cDbl w, cDbl h, string color, double rotation = 0.0, cDbl xr = 0.0, cDbl yr = 0.0);

/**
 * Create a string to represent an SVG circle.
 * @param x x coordinate of the circle's center
 * @param y y coordinate of the circle's center
 * @param r circle's radius (must be greater than 0)
 * @param color circle's color (must be in SVG format)
 * @return SVG circle string;
 *      EMPTY if error occures
 */
string LBAMTTcircleSVG(cDbl x, cDbl y, cDbl r, string color);

/**
 * Create a string to represent an SVG line,
 * start and end point must be different.
 * @param x1 x coordinate of the start point
 * @param y1 y coordinate of the start point
 * @param x2 x coordinate of the end point
 * @param y2 y coordinate of the end point
 * @param stroke line thickness (default 2) (must be greater than 0)
 * @param color line's color (must be in SVG format) (default black)
 * @param opt additional options (default empty) (no format check)
 * @return SVG line string;
 *      EMPTY if error occures
 */
string LBAMTTlineSVG(cDbl x1, cDbl y1, cDbl x2, cDbl y2, int stroke = 2, string color = "black", string opt = "");

/**
 * Create a string to represent an SVG arc.
 * the arc's angle must be grater than zero
 * @param cx x coordinate of the arc's center
 * @param cy x coordinate of the arc's center
 * @param r arc's radius (must be greater than 1)
 * @param startAngle arc's start angle (in degrees)
 * @param endAngle arc's end angle (in degrees)
 * @param stroke arc's thickness (must be greater 0 and lower than r)
 * @param color arc's color (must be in SVG format) (default black)
 * @return SVG arc string;
 *      EMPTY if error occures
 */
string LBAMTTarcSVG(cDbl cx, cDbl cy, cDbl r, double startAngle, double endAngle, int stroke = 2, string color = "black");

/**
 * Create a string to add an SVG text.
 * @param s text to be written
 * @param x x coordinate of the text
 * @param y y coordinate of the text
 * @param rotation text's rotation angle (default 0.0)
 * @param xr x coordinate of the center rotation point (default 0.0)
 * @param yr r coordinate of the center rotation point (default 0.0)
 * @param color text's color (must be in SVG format) (default black)
 * @param anchor relative position of the text respect to the given point  (start, middle, end) (default middle) (no format check)
 * @param opt additional options (default empty) (no format check)
 * @return SVG text string
 */
string LBAMTTtextSVG(string s, cDbl x, cDbl y, double rotation = 0.0, cDbl xr = 0.0, cDbl yr = 0.0, string color = "black", string anchor = "middle", string opt = "");

/**
 * Create a string to represent a distance quote between the points A and B, 
 * the two points cant be the same.
 * REQUIRED create an arrow marker with LBAMTTarrowMarkerSVG at file's start.
 * @param xA x coordinate of point A
 * @param yA y coordinate of point A
 * @param xB x coordinate of point B
 * @param yB y coordinate of point B
 * @param distQuote quote's distance from AB (must be greater than lQuote)
 * @param lQuote additional lenght of the quot's side lines (can't be lower than 1)
 * @param side flag: if true, the quote has direction theta+90, with theta=arg(AB), otherways on the opposite side
 * @return SVG quote string;
 *      EMPTY if error occures
 */
string LBAMTTquoteDistSVG(cDbl xA, cDbl yA, cDbl xB, cDbl yB, cDbl distQuote, cDbl lQuote, bool side = true);

/**
 * Create a string to represent aan angle quote between the two given angles, 
 * the two angles cant be the same.
 * REQUIRED create an arrow marker with LBAMTTarrowMarkerSVG at file's start.
 * @param cx x coordinate of the angle's center
 * @param cy y coordinate of the angle's center
 * @param startAngle quote's start angle (in degrees)
 * @param endAngle quote's end angle (in degrees)
 * @param distQuote quote's distance from the angle's center (must be greater than lQuote)
 * @param lQuote additional lenght of the quot's side lines (can't be lower than 1)
 * @return SVG quote string;
 *      EMPTY if error occures
 */
string LBAMTTquoteAngleSVG(cDbl cx, cDbl cy, cDbl startAngle, cDbl endAngle, cDbl distQuote, cDbl lQuote);

#endif