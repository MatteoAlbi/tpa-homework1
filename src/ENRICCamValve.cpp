#include "ENRICCamValve.h"

ENRICdevice * ENRICinitDevice (double rMin, double rMax, double lenValve, double diamValve, double Alpha, double Gamma){
    
    ENRICdevice * device = new(nothrow) ENRICdevice;
    
    device->rMin = rMin;
    device->rMax = rMax;
    device->lenValve = lenValve;
    device->diamValve = diamValve;
    device->Alpha = fmod(Alpha , (2*PI));
    device->Gamma = Gamma;
    
    if(ENRICcheckIntegrity(device)){
        delete device;
        return NULL;
    }
    else return device;
};

int ENRICcheckIntegrity (const ENRICdevice * device){
    //Checking if all the measurs make sense (no negative lenghts)
    if (device->rMin <= 0) return 1;
    else if(device->rMax <= 0) return 1;
    else if (device->lenValve <= 0) return 1;
    else if (device->diamValve <= 0) return 1;

    //Checking if the parameters respect the construction constrain
    else if (device->rMax  <= device->rMin) return 2; //Cam constrain
    else if ((device->rMax - device->rMin) >= device->lenValve) return 3; //Constrain on the valve lift

    else return 0;
};

int ENRICsetrMin (ENRICdevice * device, double newrMin){
    double var = device->rMin;
    device->rMin = newrMin;
    
    if(ENRICcheckIntegrity(device)){
        device->rMin = var;
        return 1;
    }
    else return 0;
};

int ENRICsetrMax (ENRICdevice * device, double newrMax){
    double var = device->rMax;
    device->rMax = newrMax;
    
    if(ENRICcheckIntegrity(device)){
        device->rMax = var;
        return 1;
    }
    else return 0;
};

int ENRICsetlenValve (ENRICdevice * device, double newlenValve){
    double var = device->lenValve;
    device->lenValve = newlenValve;
    
    if(ENRICcheckIntegrity(device)){
        device->lenValve = var;
        return 1;
    }
    else return 0;
};

int ENRICsetAlpha (ENRICdevice * device, double alpha){
    double var = device->Alpha;
    device->Alpha = alpha;
    
    if(ENRICcheckIntegrity(device)){
        device->Alpha = var;
        return 1;
    }
    else return 0;
};

int ENRICdelete (ENRICdevice * device){
    if(device == NULL) return 1;
    else {
        delete device;
        return 0;
    }
}

string ENRIClineSVG(double x1, double y1, double x2, double y2, int stroke, string color, string opt){

    //checking values
    if(stroke <= 0) return "";
    if(x1 == x2 && y1 == y2) return "";

    string line = "";
    line += "<line x1=\"" + to_string(x1) + "\" y1=\"" + to_string(y1) + "\" x2=\"" + to_string(x2) + "\" y2=\"" + to_string(y2) + "\" "; //defining line
    line += "style=\"stroke:" + color + ";stroke-width:" + to_string(stroke) + "\" " + opt + " />\n"; //defining style and optional
    return line;
}

string ENRICarcSVG(double cx, double cy, double r, double startAngle, double endAngle, int stroke, string color, string anim){

    startAngle = fmod(startAngle, 360);
    endAngle = fmod(endAngle, 360);

    //checking values
    if(r <= 1) return "";
    if(abs( startAngle - endAngle) == 0) return "";

    //defining the arc using 4 points
    double x2 = cx + (r) * cos(startAngle * PI/180);
    double y2 = cy + (r) * sin(startAngle * PI/180);
    double x4 = cx + (r) * cos(endAngle * PI/180);
    double y4 = cy + (r) * sin(endAngle * PI/180);

    //defining the arc
    string arc = "";
    arc += "<path d=\"\n";
    arc += "M " + to_string(x2) + " " + to_string(y2) + "\n";
    arc += "A " + to_string(r) + " " + to_string(r) + " 0 0 1 " + to_string(x4) + " " + to_string(y4) + "\n";
    arc += "L " + to_string(cx) + " " + to_string(cy) + "\n";
    arc += "Z\"\n";
    arc += "style=\"fill:" + color + "\" >" + anim + "</path>";

    return arc;
}

string ENRICAnimationarcSVG(double cx, double cy, double r, double startAngle, double endAngle, int stroke, string color){

    startAngle = fmod(startAngle, 360);
    endAngle = fmod(endAngle, 360);

    //checking values
    if(r <= 1) return "";
    if(abs(fmod(startAngle, 360) - endAngle) == 0) return "";

    //defining the arc using 4 points
    double x2 = cx + (r) * cos(startAngle * PI/180);
    double y2 = cy + (r) * sin(startAngle * PI/180);
    double x4 = cx + (r) * cos(endAngle * PI/180);
    double y4 = cy + (r) * sin(endAngle * PI/180);

    //defining the arc
    string arc = "";
    arc += "<path d=\"\n";
    arc += "M " + to_string(x2) + " " + to_string(y2) + "\n";
    arc += "A " + to_string(r) + " " + to_string(r) + " 0 0 1 " + to_string(x4) + " " + to_string(y4) + "\n";
    arc += "L " + to_string(cx) + " " + to_string(cy) + "\n";
    arc += "Z\"\n";
    arc += "style=\"fill:" + color + "\" >\n";
    arc += "<animateTransform attributeName=\"transform\""    //making the animation
                          " attributeType=\"XML\""                   //the arc rotates around the center of the cam (400,200)
                          " type=\"rotate\""
                          " from=\"0 400 200\""
                          " to=\"360 400 200\""
                          " dur=\"10s\""
                          " repeatCount=\"indefinite\"/> </path>";
  

    return arc;
}

string ENRICtextSVG(string s, double x, double y, double rotation, double xr, double yr, string color, string anchor, string opt){

    rotation = fmod(rotation, 360);

    string text = "";
    text += "<text x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" "; //defining starting point
    text += "fill=\"" + color + "\" "; //defining color
    if(rotation != 0.0) text += "transform=\"rotate(" + to_string(rotation) + "," + to_string(xr) + "," + to_string(yr) + ")\" "; //def rotazione
    text += "text-anchor=\"" + anchor + "\" " + opt + " >" + s + "</text>\n"; //defining anchor, opt, testo

    return text;
}

string ENRICquoteDistSVG(double xA, double yA, double xB, double yB, double distQuote, double lQuote, bool side){
    
    if(xA == xB && yA == yB) return "";
    if(lQuote < 1) return "";
    if(lQuote > distQuote) return "";

    double stroke = 2;
    string arrowOpt = "marker-start=\"url(#arrow)\" marker-end=\"url(#arrow)\""; //add arrows
    string quote = "";
    double theta = atan2(yB-yA, xB-xA); //inclination segment AB
    string val = to_string(sqrt(pow(yB - yA, 2) + pow(xB - xA, 2))); //quote value
    val.erase(val.length() - 5, 6); //stopping at the first decimal number

    //value to place the quote
    double x2Alat, y2Alat, x2Blat, y2Blat, x1cent, y1cent, x2cent, y2cent, xText, yText, angleText;

    if(side){
        //coordinatee point 2 lateral line on A
        x2Alat = xA + (distQuote + lQuote) * cos(theta + PI/2);
        y2Alat = yA + (distQuote + lQuote) * sin(theta + PI/2);
        //coordinatee point 2 lateral line on B
        x2Blat = xB + (distQuote + lQuote) * cos(theta + PI/2);
        y2Blat = yB + (distQuote + lQuote) * sin(theta + PI/2);
        //coordinatee point 2 of the central line
        x1cent = xA + distQuote * cos(theta + PI/2) + 9 * cos(theta); 
        y1cent = yA + distQuote * sin(theta + PI/2) + 9 * sin(theta);
        //coordinates point 2 of the central line
        x2cent = xB + distQuote * cos(theta + PI/2) - 9 * cos(theta);
        y2cent = yB + distQuote * sin(theta + PI/2) - 9 * sin(theta);
        //coordinates text
        xText = (x1cent + x2cent)/2;// - 3 * cos(theta + PI/2);
        yText = (y1cent + y2cent)/2;// - 3 * sin(theta + PI/2);
        angleText = fmod(theta * 180 / PI, 360);
        if (angleText > 90 && angleText <= 270){
            angleText += 180;
            xText += 3 * cos(theta + PI/2);
            yText += 3 * sin(theta + PI/2);
        } 
        else{
            xText -= 3 * cos(theta + PI/2);
            yText -= 3 * sin(theta + PI/2);
        }
    }
    else{
        //coordinatee point 2 lateral line on A
        x2Alat = xA - (distQuote + lQuote) * cos(theta + PI/2);
        y2Alat = yA - (distQuote + lQuote) * sin(theta + PI/2);
        //coordinatee point 2 lateral line on B
        x2Blat = xB - (distQuote + lQuote) * cos(theta + PI/2);
        y2Blat = yB - (distQuote + lQuote) * sin(theta + PI/2);
        //coordinatee point 1 of the central line
        x1cent = xA - distQuote * cos(theta + PI/2) + 9 * cos(theta); 
        y1cent = yA - distQuote * sin(theta + PI/2) + 9 * sin(theta);
        //coordinatee point 2 of the central line
        x2cent = xB - distQuote * cos(theta + PI/2) - 9 * cos(theta);
        y2cent = yB - distQuote * sin(theta + PI/2) - 9 * sin(theta);
        //coordinate text
        xText = (x1cent + x2cent)/2;// + 3 * cos(theta + PI/2);
        yText = (y1cent + y2cent)/2;// + 3 * sin(theta + PI/2);
        angleText = fmod(theta * 180 / PI, 360);
        if (angleText > 90 && angleText <= 270){
            angleText += 180;
            xText += 3 * cos(theta + PI/2);
            yText += 3 * sin(theta + PI/2);
        } 
        else{
            xText -= 3 * cos(theta + PI/2);
            yText -= 3 * sin(theta + PI/2);
        }
    }

    quote += ENRIClineSVG(xA, yA, x2Alat, y2Alat, stroke); //lateral line on A
    quote += ENRIClineSVG(xB, yB, x2Blat, y2Blat, stroke); //lateral line on B
    quote += ENRIClineSVG(x1cent, y1cent, x2cent, y2cent, stroke, "black", arrowOpt); //central line
    quote += ENRICtextSVG(val, xText, yText, angleText, xText, yText, "black", "middle", "");

    return quote;
}

string ENRICarrowMarkerSVG(){
    return  "<defs>\n"
            "<!-- arrowhead marker definition -->\n"
            "<marker id=\"arrow\" viewBox=\"0 0 10 10\" refX=\"0\" refY=\"5\"\n"
            "markerWidth=\"4\" markerHeight=\"4\"\n"
            "orient=\"auto-start-reverse\">\n"
            "<path d=\"M 0 1.5 L 10 5 L 0 8.5 z\" />\n"
            "</marker>\n"
            "</defs>\n\n";
}

string ENRICtoStringSVG (ENRICdevice * device, double cxShaft, double cyShaft, bool quote, bool header, LBAMTTanimation * anim){
    if(quote) anim = NULL; //priority to quotes

    string deviceSVG = "";
    double xC, yC; //cooridnates center of rotation (internal circle of the cam)
    double ValveStartY; // starting point to draw the valve

    //Defining parameters
    xC = cxShaft;
    yC = cyShaft;
    ValveStartY = 0;

    // Calculating the 4 point needed to properly draw the cam, check README for more info
    double PCest1x = device->rMax*cos(device->Gamma-device->Alpha)+xC;
    double PCest1y = yC-device->rMax*sin(device->Gamma-device->Alpha);
    double PCest2x = device->rMax*cos(-device->Alpha-device->Gamma)+xC;
    double PCest2y = yC-device->rMax*sin(-device->Alpha-device->Gamma);
    double PCint1x = device->rMin*cos(-device->Alpha+PI/2)+xC;
    double PCint1y = yC-device->rMin*sin(-device->Alpha+PI/2);
    double PCint2x = device->rMin*cos(-device->Alpha-PI/2)+xC;
    double PCint2y = yC-device->rMin*sin(-device->Alpha-PI/2);

    //Building the cam
        //Internal circle of the cam: I just need a section of the circle, so I just draw an arc
    deviceSVG += ENRICarcSVG(xC, yC, device->rMin, device->Alpha*180/PI+90, device->Alpha*180/PI+270, device->rMin-1, "silver", LBAMTTappearSVG("silver", anim));
    deviceSVG += "\n"; 
        //External circle of the cam: I just need a section of the circle, so I just draw an arc
    deviceSVG += ENRICarcSVG(xC, yC, device->rMax, device->Alpha*180/PI-(device->Gamma)*180/PI, device->Alpha*180/PI+(device->Gamma)*180/PI, device->rMax-1, "silver", LBAMTTappearSVG("silver", anim));   
    deviceSVG += "\n";
    // Making a polygon to fill the empty space of the cam
    deviceSVG += "<polygon points=\"" + to_string(PCest1x) + ", " + to_string(PCest1y) + " " +
                                      to_string(PCest2x) + ", " + to_string(PCest2y) + " " +
                                      to_string(PCint2x) + ", " + to_string(PCint2y) + " " +
                                      to_string(PCint1x) + ", " + to_string(PCint1y) + " ";
    deviceSVG += "\" style=\"fill:silver\">" + LBAMTTappearSVG("silver", anim) + "</polygon>\n"; //defining colour
    
    
    //Valve
        //First rectangle componing the valve
            // The height of the valve depends on the angle of rotation of the cam, so I will make 4 separate cases
            if(device->Alpha >= 0 && device->Alpha < (PI/2-device->Gamma)){ //to check
                ValveStartY =  (device->rMax*device->rMin*cos(device->Gamma) + 
                device->rMax*yC*cos(device->Alpha+device->Gamma)+device->rMin*yC*sin(device->Alpha))/
                (device->rMax*cos(device->Alpha+device->Gamma)+device->rMin*sin(device->Alpha));
            }
            else if(device->Alpha >= (PI/2-device->Gamma) && device->Alpha < (PI/2+device->Gamma)){
                ValveStartY = yC+device->rMax;
            }
            else if (device->Alpha >= (PI/2+device->Gamma) && device->Alpha < PI){
                ValveStartY = (device->rMax*yC*cos(device->Alpha-device->Gamma)
                -device->rMax*device->rMin*cos(device->Gamma)-device->rMin*yC*sin(device->Alpha))/
                (device->rMax*cos(device->Alpha-device->Gamma)-device->rMin*sin(device->Alpha));
            }
            else if (device->Alpha >= PI && device->Alpha <= 2*PI){
                ValveStartY = yC+device->rMin;
            }

    //First rectangle componing the valve            
    deviceSVG += "<rect x=\"" + to_string(xC-(device->lenValve)/20) + "\" y=\"" + to_string(ValveStartY) + "\" "; //defining starting point
    deviceSVG += "width=\"" + to_string((device->lenValve)/10) + "\" height=\"" + to_string(device->lenValve) + "\" "; //defining dimension
    deviceSVG += "style=\"fill:dimgray\">" + LBAMTTappearSVG("dimgray", anim) + "</rect>\n"; //defining colour
    //Second rectangle componing the valve
    deviceSVG += "<rect x=\"" + to_string(xC-(device->diamValve)/2) + "\" y=\"" + to_string(device->lenValve+ValveStartY) + "\" "; //defining starting point
    deviceSVG += "width=\"" + to_string(device->diamValve) + "\" height=\"" + to_string((device->lenValve)/10) + "\" "; //defining dimension
    deviceSVG += "style=\"fill:dimgray\">" + LBAMTTappearSVG("dimgray", anim) + "</rect>\n"; //defining colour

    // Making the quote of the device
    if(quote){
        double lQuote = device->diamValve / 4 ;//length lateral line of the quote
        double distQuote = lQuote*2; //distance of the quote from the device
        deviceSVG = ENRICarrowMarkerSVG() + "\n" + deviceSVG; //marker arrow def
        // Quote of the length of the valve
        if((fmod(device->Alpha, PI) > PI/2)||(fmod(device->Alpha, PI) < 3*PI/2)){ // the quote is on the right side
            deviceSVG += ENRICquoteDistSVG(xC+device->lenValve/20,ValveStartY,xC+device->lenValve/20 , ValveStartY+device->lenValve, device->lenValve/10+distQuote, lQuote, false);
        }
        else{
            deviceSVG += ENRICquoteDistSVG(xC-device->lenValve/20,ValveStartY,xC-device->lenValve/20, ValveStartY+device->lenValve, device->lenValve/10+distQuote, lQuote, true);
        }

        // Quote of the diameter of the valve
        deviceSVG += ENRICquoteDistSVG(xC-device->diamValve/2, device->lenValve+ValveStartY+device->lenValve/10, xC+device->diamValve/2, device->lenValve+ValveStartY+device->lenValve/10, device->lenValve/7+distQuote, lQuote, true);

        // Quote of the external circle of the cam
        if((fmod(device->Alpha, PI) < PI/2)||(fmod(device->Alpha, PI) > 3*PI/2)){ // the quote is on the right side
            deviceSVG += ENRICquoteDistSVG(xC, yC, xC+((cos(device->Alpha))*device->rMax), yC+((sin(device->Alpha))*device->rMax), device->lenValve/4+distQuote, lQuote, false);
        }
        else{
            deviceSVG += ENRICquoteDistSVG(xC, yC, xC+((cos(device->Alpha))*device->rMax), yC+((sin(device->Alpha))*device->rMax), device->lenValve/4+distQuote, lQuote, true);
        }

        // Quote of the internal circle of the cam
        if((fmod(device->Alpha, PI) < PI/2)||(fmod(device->Alpha, PI) > 3*PI/2)){ // the quote is on the right side
            deviceSVG += ENRICquoteDistSVG(xC-device->rMin, yC-device->rMin,xC-device->rMin,yC+device->rMin, device->lenValve/5+distQuote, lQuote, true);
        }
        else{
            deviceSVG += ENRICquoteDistSVG(xC+device->rMin, yC-device->rMin,xC+device->rMin,yC+device->rMin, device->lenValve/5+distQuote, lQuote, false);
        }
    }
    //defining file and sheet dimension
    if (header){
        deviceSVG = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\" >\n\n" + 
        deviceSVG + "\n" + "</svg>\n";
    }
    return deviceSVG;
}

vector<string> ENRICsplitString (string s, string delimiter){
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {//looks for delimiter, exit if not found
        token = s.substr (pos_start, pos_end - pos_start);//gives the portion of the string between the start and the position of the delimiter
        pos_start = pos_end + delim_len; //updateds the pointer to the start of the string
        res.push_back (token); //saves the extracted little string
    }

    res.push_back (s.substr (pos_start)); //saves the remaning part of the vector when the delimiter cannot be found
    return res;
}

int ENRICsaveToFile(string s, string fileName){

    //check if there is a name
    if (fileName == "") return 1;

    //check if the extension is correct
    vector<string> checkFileName = ENRICsplitString(fileName,".");
    if (checkFileName.front() == "") return 1;
    if (checkFileName.back() != "svg") return 1;

    ofstream MyFile(fileName);
    MyFile << s;
    MyFile.close();

    return 0;
}

string ENRICloadFromFile(string fileName){

    //check if there is a name
    if (fileName == "") return "";

    //check if the extension is correct
    vector<string> checkFileName = ENRICsplitString(fileName,".");
    if (checkFileName.front() == "") return "";
    if (checkFileName.back() != "svg") return "";

    ifstream fin(fileName);
    stringstream buffer;

    if(! fin.is_open()) return "";

    buffer << fin.rdbuf();
    string s = buffer.str();

    return s;
}

string ENRICAnimationtoStringSVG (ENRICdevice * device, bool quote){
    string deviceSVG = "";
    double xC, yC; //cooridnates center of rotation (internal circle of the cam)
    double ValveStartY; // starting point to draw the valve

    //Defining parameters
    xC = 400;
    yC = 200;
    ValveStartY = 0;

    // Calculating the 4 point needed to properly draw the cam, check README for more info
    double PCest1x = device->rMax*cos(device->Gamma-device->Alpha)+xC;
    double PCest1y = yC-device->rMax*sin(device->Gamma-device->Alpha);
    double PCest2x = device->rMax*cos(-device->Alpha-device->Gamma)+xC;
    double PCest2y = yC-device->rMax*sin(-device->Alpha-device->Gamma);
    double PCint1x = device->rMin*cos(-device->Alpha+PI/2)+xC;
    double PCint1y = yC-device->rMin*sin(-device->Alpha+PI/2);
    double PCint2x = device->rMin*cos(-device->Alpha-PI/2)+xC;
    double PCint2y = yC-device->rMin*sin(-device->Alpha-PI/2); 

    //Building the cam
        //Internal circle of the cam: I just need a section of the circle, so I just draw an arc
    deviceSVG += ENRICAnimationarcSVG(xC, yC, device->rMin, device->Alpha*180/PI+90, device->Alpha*180/PI+270, device->rMin-1, "grey");
    deviceSVG += "\n"; 
        //External circle of the cam: I just need a section of the circle, so I just draw an arc
    deviceSVG += ENRICAnimationarcSVG(xC, yC, device->rMax, device->Alpha*180/PI-(device->Gamma)*180/PI, device->Alpha*180/PI+(device->Gamma)*180/PI, device->rMax-1, "grey");   
    deviceSVG += "\n";
    // Making a polygon to fill the empty space of the cam
    deviceSVG += "<polygon points=\"" + to_string(PCest1x) + ", " + to_string(PCest1y) + " " +
                                      to_string(PCest2x) + ", " + to_string(PCest2y) + " " +
                                      to_string(PCint2x) + ", " + to_string(PCint2y) + " " +
                                      to_string(PCint1x) + ", " + to_string(PCint1y) + " ";
    deviceSVG += "\" style=\"fill:grey\" >\n"; //defining colour
    deviceSVG += "<animateTransform attributeName=\"transform\""    //making the animation
                          " attributeType=\"XML\""                   //the polygon rotates around the center of the cam (400,200)
                          " type=\"rotate\""
                          " from=\"0 400 200\""
                          " to=\"360 400 200\""
                          " dur=\"10s\""
                          " begin=\"0s\""
                          " repeatCount=\"indefinite\"/> </polygon>";
    
    //Valve
        //First rectangle componing the valve
            // The height of the valve depends on the angle of rotation of the cam, so I will make 4 separate cases
            if(device->Alpha >= 0 && device->Alpha < (PI/2-device->Gamma)){ //to check
                ValveStartY =  (device->rMax*device->rMin*cos(device->Gamma) + 
                device->rMax*yC*cos(device->Alpha+device->Gamma)+device->rMin*yC*sin(device->Alpha))/
                (device->rMax*cos(device->Alpha+device->Gamma)+device->rMin*sin(device->Alpha));
            }
            else if(device->Alpha >= (PI/2-device->Gamma) && device->Alpha < (PI/2+device->Gamma)){
                ValveStartY = yC+device->rMax;
            }
            else if (device->Alpha >= (PI/2+device->Gamma) && device->Alpha < PI){
                ValveStartY = (device->rMax*yC*cos(device->Alpha-device->Gamma)
                -device->rMax*device->rMin*cos(device->Gamma)-device->rMin*yC*sin(device->Alpha))/
                (device->rMax*cos(device->Alpha-device->Gamma)-device->rMin*sin(device->Alpha));
            }
            else if (device->Alpha >= PI && device->Alpha < 2*PI){
                ValveStartY = yC+device->rMin;
            }

    //First rectangle componing the valve            
    deviceSVG += "<rect x=\"" + to_string(xC-(device->lenValve)/20) + "\" y=\"" + to_string(ValveStartY) + "\" "; //defining starting point
    deviceSVG += "width=\"" + to_string((device->lenValve)/10) + "\" height=\"" + to_string(device->lenValve) + "\" "; //defining dimension
    deviceSVG += "style=\"fill:black\" >\n"; //defining colour
    deviceSVG += "<animate "
                     " attributeName=\"y\" "
                     " from=\"250\" "
                     " to=\"265\""
                     " dur=\"1.875s\""
                     " begin=\"0s\""
                     " fill=\"remove\"/>";
    deviceSVG += "<animate "
                     " attributeName=\"y\" "
                     " from=\"265\" "
                     " to=\"265\""
                     " dur=\"1.25s\""
                     " begin=\"1.875s\""
                     " fill=\"remove\"/>";
    deviceSVG += "<animate "
                     " attributeName=\"y\" "
                     " from=\"265\" "
                     " to=\"250\""
                     " dur=\"1.875s\""
                     " begin=\"3.125s\""
                     " fill=\"remove\"/>";
    deviceSVG += "<animate "
                     " attributeName=\"y\" "
                     " from=\"250\" "
                     " to=\"250\""
                     " dur=\"5s\""
                     " begin=\"5s\""
                     " fill=\"remove\"/> </rect>";
    //Second rectangle componing the valve
    deviceSVG += "<rect x=\"" + to_string(xC-(device->diamValve)/2) + "\" y=\"" + to_string(device->lenValve+ValveStartY) + "\" "; //defining starting point
    deviceSVG += "width=\"" + to_string(device->diamValve) + "\" height=\"" + to_string((device->lenValve)/10) + "\" "; //defining dimension
    deviceSVG += "style=\"fill:black\" />\n"; //defining colour
    // deviceSVG += "<animate "
    //                  " attributeName=\"y\" "
    //                  " from=\"465\" "
    //                  " to=\"480\""
    //                  " dur=\"1.875s\""
    //                  " begin=\"0s\""
    //                  " fill=\"remove\"/> </rect>";
    // deviceSVG += "<animate "
    //                  " attributeName=\"y\" "
    //                  " from=\"480\" "
    //                  " to=\"480\""
    //                  " dur=\"1.25s\""
    //                  " begin=\"1.875s\""
    //                  " fill=\"remove\"/>";
    // deviceSVG += "<animate "
    //                  " attributeName=\"y\" "
    //                  " from=\"480\" "
    //                  " to=\"465\""
    //                  " dur=\"1.875s\""
    //                  " begin=\"3.125s\""
    //                  " fill=\"remove\"/>";
    // deviceSVG += "<animate "
    //                  " attributeName=\"y\" "
    //                  " from=\"465\" "
    //                  " to=\"465\""
    //                  " dur=\"5s\""
    //                  " begin=\"5s\""
    //                  " fill=\"remove\"/> </rect>";

    //defining file and sheet dimension
    deviceSVG = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n"
    "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\" >\n\n" + deviceSVG + "</svg>\n";

    return deviceSVG;
}
