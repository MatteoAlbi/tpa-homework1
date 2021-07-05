# include "LBAMTTcadSVG.h"

double LBAMTTnormAng(double angle){
    if(angle >= 360) while(angle >= 360) angle -= 360;
    else if(angle < 0) while(angle < 0) angle += 360;

    return angle;
}

string LBAMTTheaderSVG(string s){
    return  "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n"
            "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\" >\n\n" + s + "</svg>\n";
}

string LBAMTTarrowMarkerSVG(){
    return  "<defs>\n"
            "<!-- arrowhead marker definition for distance quote -->\n"
            "<marker id=\"arrowDist\" viewBox=\"0 0 10 10\" refX=\"0\" refY=\"5\"\n"
            "markerWidth=\"4\" markerHeight=\"4\"\n"
            "orient=\"auto-start-reverse\">\n"
            "<path d=\"M 0 1.5 L 10 5 L 0 8.5 z\" />\n"
            "</marker>\n"
            "</defs>\n\n";
}

string LBAMTTrectSVG(cDbl x, cDbl y, cDbl w, cDbl h, string color, double rotation, cDbl xr, cDbl yr){

    rotation = LBAMTTnormAng(rotation);

    //check values
    if(w <= 0 || h <= 0) return "";

    string rect  = "";
    rect += "<rect x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" "; //def creation point
    rect += "width=\"" + to_string(w) + "\" height=\"" + to_string(h) + "\" "; //def dimensions 
    rect += "fill=\"" + color + "\" "; //def color
    rect += "transform=\"rotate(" + to_string(rotation) + "," + to_string(xr) + "," + to_string(yr) + ")\" "; //def rotation
    rect += "/>\n";

    return rect;
}

string LBAMTTcircleSVG(cDbl x, cDbl y, cDbl r, string color){

    //check values
    if(r <= 0) return "";

    string circle = "";
    circle += "<circle cx=\"" + to_string(x) + "\" cy=\"" + to_string(y) + "\" "; //def creation point
    circle += "r=\"" + to_string(r) +"\" fill=\"" + color + "\" />\n"; //def radius and color

    return circle;
}

string LBAMTTlineSVG(cDbl x1, cDbl y1, cDbl x2, cDbl y2, int stroke, string color, string opt){

    //check values
    if(stroke <= 0) return "";
    if(x1 == x2 && y1 == y2) return "";

    string line = "";
    line += "<line x1=\"" + to_string(x1) + "\" y1=\"" + to_string(y1) + "\" x2=\"" + to_string(x2) + "\" y2=\"" + to_string(y2) + "\" "; //def line
    line += "style=\"stroke:" + color + ";stroke-width:" + to_string(stroke) + "\" " + opt + " />\n"; //def style + option

    return line;
}

string LBAMTTarcSVG(cDbl cx, cDbl cy, cDbl r, double startAngle, double endAngle, int stroke, string color){

    startAngle = LBAMTTnormAng(startAngle);
    endAngle = LBAMTTnormAng(endAngle);
    double angle = endAngle - startAngle;
    if(angle < 0) angle += 360;
    bool largeArcFlag = angle >= 180;

    //check values
    if(r <= 1) return "";
    if(stroke <= 0 || stroke >= r) return "";
    if(abs( LBAMTTnormAng(startAngle) - endAngle) == 0) return "";

    //4 points to define the arc (as arc of ring)
    double rMin = r - stroke/2;
    double rMax = r + stroke/2;
    double x1start = cx + (rMin) * cos(startAngle * PI/180);
    double y1start = cy + (rMin) * sin(startAngle * PI/180);
    double x2start = cx + (rMax) * cos(startAngle * PI/180);
    double y2start = cy + (rMax) * sin(startAngle * PI/180);
    double x1end = cx + (rMin) * cos(endAngle * PI/180);
    double y1end = cy + (rMin) * sin(endAngle * PI/180);
    double x2end = cx + (rMax) * cos(endAngle * PI/180);
    double y2end = cy + (rMax) * sin(endAngle * PI/180);

    //define arc's path
    string arc = "";
    arc += "<path d=\"\n";
    arc += "M " + to_string(x2start) + " " + to_string(y2start) + "\n";
    arc += "A " + to_string(rMax) + " " + to_string(rMax) + 
          " 0 " + to_string(int(largeArcFlag)) + 
          " 1 " + to_string(x2end) + " " + to_string(y2end) + "\n";
    arc += "L " + to_string(x1end) + " " + to_string(y1end) + "\n";
    arc += "A " + to_string(rMin) + " " + to_string(rMin) + 
          " 0 " + to_string(int(largeArcFlag)) + 
          " 0 " + to_string(x1start) + " " + to_string(y1start) + "\n";
    arc += "Z\"\n";
    arc += "style=\"fill:" + color + "\" />\n";

    return arc;
}

string LBAMTTtextSVG(string s, cDbl x, cDbl y, double rotation, cDbl xr, cDbl yr, string color, string anchor, string opt){

    rotation = LBAMTTnormAng(rotation);

    string text = "";
    text += "<text x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" "; //def creation point
    text += "fill=\"" + color + "\" "; //def color
    if(rotation != 0.0) text += "transform=\"rotate(" + to_string(rotation) + "," + to_string(xr) + "," + to_string(yr) + ")\" "; //def rotation
    text += "text-anchor=\"" + anchor + "\" " + opt + " >" + s + "</text>\n"; //def anchor + opt + text

    return text;
}

string LBAMTTquoteDistSVG(cDbl xA, cDbl yA, cDbl xB, cDbl yB, cDbl distQuote, cDbl lQuote, bool side){
    
    if(xA == xB && yA == yB) return "";
    if(lQuote < 1) return "";
    if(lQuote > distQuote) return "";

    cDbl stroke = 2;
    string arrowOpt = "marker-start=\"url(#arrowDist)\" marker-end=\"url(#arrowDist)\""; //option to add arrow markers
    string quote = "";
    double theta = atan2(yB-yA, xB-xA); //angle of segment AB
    string val = to_string(sqrt(pow(yB - yA, 2) + pow(xB - xA, 2))); //quote val
    val.erase(val.length() - 5, 6); //keep only the first decimal digit

    //values to place the quote
    double x2Alat, y2Alat, x2Blat, y2Blat, x1cent, y1cent, x2cent, y2cent, xText, yText, angleText;

    if(side){
        //coordinates of point 2 of side line on A
        x2Alat = xA + (distQuote + lQuote) * cos(theta + PI/2);
        y2Alat = yA + (distQuote + lQuote) * sin(theta + PI/2);
        //coordinates of point 2 of side line on B
        x2Blat = xB + (distQuote + lQuote) * cos(theta + PI/2);
        y2Blat = yB + (distQuote + lQuote) * sin(theta + PI/2);
        //coordinates of point 1 of central line
        x1cent = xA + distQuote * cos(theta + PI/2) + 9 * cos(theta); 
        y1cent = yA + distQuote * sin(theta + PI/2) + 9 * sin(theta);
        //coordinates of point 2 of central line
        x2cent = xB + distQuote * cos(theta + PI/2) - 9 * cos(theta);
        y2cent = yB + distQuote * sin(theta + PI/2) - 9 * sin(theta);
        //coordinates of text
        xText = (x1cent + x2cent)/2;
        yText = (y1cent + y2cent)/2;
        angleText = LBAMTTnormAng(theta * 180 / PI);
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
        //coordinates of point 2 of side line on A
        x2Alat = xA - (distQuote + lQuote) * cos(theta + PI/2);
        y2Alat = yA - (distQuote + lQuote) * sin(theta + PI/2);
        //coordinates of point 2 of side line on B
        x2Blat = xB - (distQuote + lQuote) * cos(theta + PI/2);
        y2Blat = yB - (distQuote + lQuote) * sin(theta + PI/2);
        //coordinates of point 1 of central line
        x1cent = xA - distQuote * cos(theta + PI/2) + 9 * cos(theta); 
        y1cent = yA - distQuote * sin(theta + PI/2) + 9 * sin(theta);
        //coordinates of point 2 of central line
        x2cent = xB - distQuote * cos(theta + PI/2) - 9 * cos(theta);
        y2cent = yB - distQuote * sin(theta + PI/2) - 9 * sin(theta);
        //coordinates of text
        xText = (x1cent + x2cent)/2;
        yText = (y1cent + y2cent)/2;
        angleText = LBAMTTnormAng(theta * 180 / PI);
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

    quote += LBAMTTlineSVG(xA, yA, x2Alat, y2Alat, stroke); //side line on A
    quote += LBAMTTlineSVG(xB, yB, x2Blat, y2Blat, stroke); //side line on B
    quote += LBAMTTlineSVG(x1cent, y1cent, x2cent, y2cent, stroke, "black", arrowOpt); //central line
    quote += LBAMTTtextSVG(val, xText, yText, angleText, xText, yText);//text

    return quote;
}

string LBAMTTquoteAngleSVG(cDbl cx, cDbl cy, cDbl startAngle, cDbl endAngle, cDbl distQuote, cDbl lQuote){

    if(startAngle == endAngle) return "";
    if(lQuote < 1) return "";
    if(lQuote > distQuote) return "";

    //offset on arc for arrow markers
    double arrowOffest = 6.45; //distance to add the arrow markers
    double angleOffset = acos(1 - pow(arrowOffest, 2) / pow(distQuote, 2)) * 180 / PI; //offset on quote's angle to add markers
    double arcStartAngle = LBAMTTnormAng(startAngle) + angleOffset;
    double arcEndAngle = LBAMTTnormAng(endAngle) - angleOffset;
    
    //parameters of arc to add markers
    double angle = LBAMTTnormAng(endAngle - startAngle);
    bool largeArcFlag = angle >= 180;

    //coordinate per text
    double xText = cx + distQuote * cos((startAngle + endAngle) * PI / 360);
    double yText = cy + distQuote * sin((startAngle + endAngle) * PI / 360);
    double angleText = LBAMTTnormAng((startAngle + endAngle)/2);
    if (angleText > 180 && angleText <= 360){
        xText += 4 * cos(angleText * PI / 180);
        yText += 4 * sin(angleText * PI / 180);
        angleText += 90;
        
    } 
    else{
        xText += 15 * cos(angleText * PI / 180);
        yText += 15 * sin(angleText * PI / 180);
        angleText -= 90;
    }

    string arrowOpt = "marker-start=\"url(#arrowDist)\" marker-end=\"url(#arrowDist)\""; //option to add markers
    string val = to_string(angle); //quote val
    val.erase(val.length() - 5, 6); ////keep only the first decimal digit

    string quote;
    quote += LBAMTTlineSVG(cx, cy, cx + (distQuote + lQuote) * cos(startAngle * PI / 180), cy + (distQuote + lQuote) * sin(startAngle * PI / 180));
    quote += LBAMTTlineSVG(cx, cy, cx + (distQuote + lQuote) * cos(endAngle * PI / 180), cy + (distQuote + lQuote) * sin(endAngle * PI / 180));
    quote += LBAMTTarcSVG(cx, cy, distQuote, arcStartAngle, arcEndAngle);
    
    //add arrow markers
    quote += "<path d=\"\n"; //arc to add markers
    quote += "M " + to_string(cx + distQuote * cos(arcStartAngle * PI / 180)) + 
              " " + to_string(cy + distQuote * sin(arcStartAngle * PI / 180)) + "\n";
    quote += "A " + to_string(distQuote) + " " + to_string(distQuote) + 
            " 0 " + to_string(int(largeArcFlag)) + 
            " 1 " + to_string(cx + distQuote * cos(arcEndAngle * PI / 180)) +
              " " + to_string(cy + distQuote * sin(arcEndAngle * PI / 180)) + "\"\n";
    quote += "fill=\"transparent\" stroke-width=\"2\" ";
    quote += arrowOpt + " />\n";

    quote += LBAMTTtextSVG(val, xText, yText, angleText, xText, yText); 

    return quote;
}
