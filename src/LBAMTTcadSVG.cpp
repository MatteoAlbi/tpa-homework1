# include "LBAMTTcadSVG.h"

string LBAMTTarrowMarkerSVG(){
    return  "<defs>\n"
            "<!-- arrowhead marker definition -->\n"
            "<marker id=\"arrow\" viewBox=\"0 0 10 10\" refX=\"0\" refY=\"5\"\n"
            "markerWidth=\"4\" markerHeight=\"4\"\n"
            "orient=\"auto-start-reverse\">\n"
            "<path d=\"M 0 1.5 L 10 5 L 0 8.5 z\" />\n"
            "</marker>\n"
            "</defs>\n\n";
};

string LBAMTTrectSVG(cDbl x, cDbl y, cDbl w, cDbl h, string color, cDbl rotation, cDbl xr, cDbl yr){
    string rect  = "";
    rect += "<rect x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" "; //def punto creazione 
    rect += "width=\"" + to_string(w) + "\" height=\"" + to_string(h) + "\" "; //def dimensioni 
    rect += "fill=\"" + color + "\" "; //def colore
    if(rotation != 0.0) rect += "transform=\"rotate(" + to_string(rotation) + "," + to_string(xr) + "," + to_string(yr) + ")\" "; //def rotazione 
    rect += "/>\n";

    return rect;
}

string LBAMTTcircleSVG(cDbl x, cDbl y, cDbl r, string color){
    string circle = "";
    circle += "<circle cx=\"" + to_string(x) + "\" cy=\"" + to_string(y) + "\" "; // def punto creazione
    circle += "r=\"" + to_string(r) +"\" fill=\"" + color + "\" />\n"; //def raggio e colore

    return circle;
}

string LBAMTTlineSVG(cDbl x1, cDbl y1, cDbl x2, cDbl y2, int stroke, string color, string opt){
    string line = "";
    line += "<line x1=\"" + to_string(x1) + "\" y1=\"" + to_string(y1) + "\" x2=\"" + to_string(x2) + "\" y2=\"" + to_string(y2) + "\" "; //def linea
    line += "style=\"stroke:" + color + ";stroke-width:" + to_string(stroke) + "\" " + opt + " />\n"; //def stile + option

    return line;
}

string LBAMTTtextSVG(string s, cDbl x, cDbl y, cDbl rotation, cDbl xr, cDbl yr, string color, string anchor, string opt){
    string text = "";
    text += "<text x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" "; //def punto creazione 
    text += "fill=\"" + color + "\" "; //def colore
    if(rotation != 0.0) text += "transform=\"rotate(" + to_string(rotation) + "," + to_string(xr) + "," + to_string(yr) + ")\" "; //def rotazione
    text += "text-anchor=\"" + anchor + "\" " + opt + " >" + s + "</text>\n"; //def anchor + opt + testo

    return text;
}

string LBAMTTquoteDistSVG(cDbl xA, cDbl yA, cDbl xB, cDbl yB, cDbl distQuote, cDbl lQuote, bool side){
    cDbl stroke = 2;
    string arrowOpt = "marker-start=\"url(#arrow)\" marker-end=\"url(#arrow)\""; //opzione per aggiungere frecce
    string quote = "";
    double theta = atan2(yB-yA, xB-xA); //inclinazione del segmento AB
    string val = to_string(sqrt(pow(yB - yA, 2) + pow(xB - xA, 2))); //valore della quota
    val.erase(val.length() - 4, 5); //tronco alla prima cifra decimale

    //valori per posizionamento quota
    double x2Alat, y2Alat, x2Blat, y2Blat, x1cent, y1cent, x2cent, y2cent, xText, yText, angleText;

    if(side){
        //coordinate punto 2 della linea laterale su A
        x2Alat = xA + (distQuote + lQuote) * cos(theta + PI/2);
        y2Alat = yA + (distQuote + lQuote) * sin(theta + PI/2);
        //coordinate punto 2 della linea laterale su B
        x2Blat = xB + (distQuote + lQuote) * cos(theta + PI/2);
        y2Blat = yB + (distQuote + lQuote) * sin(theta + PI/2);
        //coordinate punto 1 della linea centrale
        x1cent = xA + distQuote * cos(theta + PI/2) + 9 * cos(theta); 
        y1cent = yA + distQuote * sin(theta + PI/2) + 9 * sin(theta);
        //coordinate punto 2 della linea centrale
        x2cent = xB + distQuote * cos(theta + PI/2) - 9 * cos(theta);
        y2cent = yB + distQuote * sin(theta + PI/2) - 9 * sin(theta);
        //coordinate testo
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
        //coordinate punto 2 della linea laterale su A
        x2Alat = xA - (distQuote + lQuote) * cos(theta + PI/2);
        y2Alat = yA - (distQuote + lQuote) * sin(theta + PI/2);
        //coordinate punto 2 della linea laterale su B
        x2Blat = xB - (distQuote + lQuote) * cos(theta + PI/2);
        y2Blat = yB - (distQuote + lQuote) * sin(theta + PI/2);
        //coordinate punto 1 della linea centrale
        x1cent = xA - distQuote * cos(theta + PI/2) + 9 * cos(theta); 
        y1cent = yA - distQuote * sin(theta + PI/2) + 9 * sin(theta);
        //coordinate punto 2 della linea centrale
        x2cent = xB - distQuote * cos(theta + PI/2) - 9 * cos(theta);
        y2cent = yB - distQuote * sin(theta + PI/2) - 9 * sin(theta);
        //coordinate testo
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

    quote += LBAMTTlineSVG(xA, yA, x2Alat, y2Alat, stroke); //linea laterale su A
    quote += LBAMTTlineSVG(xB, yB, x2Blat, y2Blat, stroke); //linea laterale su B
    quote += LBAMTTlineSVG(x1cent, y1cent, x2cent, y2cent, stroke, "black", arrowOpt); //linea centrale
    quote += LBAMTTtextSVG(val, xText, yText, angleText, xText, yText);

    return quote;
}
