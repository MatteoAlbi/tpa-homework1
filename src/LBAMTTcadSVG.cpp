# include "LBAMTTcadSVG.h"

string LBAMTTheaderSVG(string s){
    return  "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n"
            "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\" >\n\n" + s + "</svg>\n";
}

string LBAMTTarrowMarkerSVG(){
    return  "<defs>\n"
            "<!-- arrowhead marker definition -->\n"
            "<marker id=\"arrow\" viewBox=\"0 0 10 10\" refX=\"0\" refY=\"5\"\n"
            "markerWidth=\"4\" markerHeight=\"4\"\n"
            "orient=\"auto-start-reverse\">\n"
            "<path d=\"M 0 1.5 L 10 5 L 0 8.5 z\" />\n"
            "</marker>\n"
            "</defs>\n\n";
}

string LBAMTTrectSVG(cDbl x, cDbl y, cDbl w, cDbl h, string color, double rotation, cDbl xr, cDbl yr){

    rotation = fmod(rotation, 360);

    //controllo valori passati
    if(w <= 0 || h <= 0) return "";

    string rect  = "";
    rect += "<rect x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" "; //def punto creazione 
    rect += "width=\"" + to_string(w) + "\" height=\"" + to_string(h) + "\" "; //def dimensioni 
    rect += "fill=\"" + color + "\" "; //def colore
    rect += "transform=\"rotate(" + to_string(rotation) + "," + to_string(xr) + "," + to_string(yr) + ")\" "; //def rotazione 
    rect += "/>\n";

    return rect;
}

string LBAMTTcircleSVG(cDbl x, cDbl y, cDbl r, string color){

    //controllo valori passati
    if(r <= 0) return "";

    string circle = "";
    circle += "<circle cx=\"" + to_string(x) + "\" cy=\"" + to_string(y) + "\" "; // def punto creazione
    circle += "r=\"" + to_string(r) +"\" fill=\"" + color + "\" />\n"; //def raggio e colore

    return circle;
}

string LBAMTTlineSVG(cDbl x1, cDbl y1, cDbl x2, cDbl y2, int stroke, string color, string opt){

    //controllo valori passati
    if(stroke <= 0) return "";
    if(x1 == x2 && y1 == y2) return "";

    string line = "";
    line += "<line x1=\"" + to_string(x1) + "\" y1=\"" + to_string(y1) + "\" x2=\"" + to_string(x2) + "\" y2=\"" + to_string(y2) + "\" "; //def linea
    line += "style=\"stroke:" + color + ";stroke-width:" + to_string(stroke) + "\" " + opt + " />\n"; //def stile + option

    return line;
}

string LBAMTTarcSVG(cDbl cx, cDbl cy, cDbl r, double startAngle, double endAngle, int stroke, string color){

    startAngle = fmod(startAngle, 360);
    endAngle = fmod(endAngle, 360);

    //controllo valori passati
    if(r <= 1) return "";
    if(stroke <= 0 || stroke >= r) return "";
    if(abs(fmod(startAngle, 360) - endAngle) == 0) return "";

    //4 punti per definire path dell'arco
    double rMin = r - stroke/2;
    double rMax = r + stroke/2;
    double x1 = cx + (rMin) * cos(startAngle * PI/180);
    double y1 = cy + (rMin) * sin(startAngle * PI/180);
    double x2 = cx + (rMax) * cos(startAngle * PI/180);
    double y2 = cy + (rMax) * sin(startAngle * PI/180);
    double x3 = cx + (rMin) * cos(endAngle * PI/180);
    double y3 = cy + (rMin) * sin(endAngle * PI/180);
    double x4 = cx + (rMax) * cos(endAngle * PI/180);
    double y4 = cy + (rMax) * sin(endAngle * PI/180);

    //definisco l'arco
    string arc = "";
    arc += "<path d=\"\n";
    arc += "M " + to_string(x2) + " " + to_string(y2) + "\n";
    arc += "A " + to_string(rMax) + " " + to_string(rMax) + " 0 0 1 " + to_string(x4) + " " + to_string(y4) + "\n";
    arc += "L " + to_string(x3) + " " + to_string(y3) + "\n";
    arc += "A " + to_string(rMin) + " " + to_string(rMin) + " 0 0 0 " + to_string(x1) + " " + to_string(y1) + "\n";
    arc += "Z\"\n";
    arc += "style=\"fill:" + color + "\" />\n";

    return arc;
}

string LBAMTTtextSVG(string s, cDbl x, cDbl y, double rotation, cDbl xr, cDbl yr, string color, string anchor, string opt){

    rotation = fmod(rotation, 360);

    string text = "";
    text += "<text x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" "; //def punto creazione 
    text += "fill=\"" + color + "\" "; //def colore
    if(rotation != 0.0) text += "transform=\"rotate(" + to_string(rotation) + "," + to_string(xr) + "," + to_string(yr) + ")\" "; //def rotazione
    text += "text-anchor=\"" + anchor + "\" " + opt + " >" + s + "</text>\n"; //def anchor + opt + testo

    return text;
}

string LBAMTTquoteDistSVG(cDbl xA, cDbl yA, cDbl xB, cDbl yB, cDbl distQuote, cDbl lQuote, bool side){
    
    if(xA == xB && yA == yB) return "";
    if(lQuote < 1) return "";
    if(lQuote > distQuote) return "";

    cDbl stroke = 2;
    string arrowOpt = "marker-start=\"url(#arrow)\" marker-end=\"url(#arrow)\""; //opzione per aggiungere frecce
    string quote = "";
    double theta = atan2(yB-yA, xB-xA); //inclinazione del segmento AB
    string val = to_string(sqrt(pow(yB - yA, 2) + pow(xB - xA, 2))); //valore della quota
    val.erase(val.length() - 5, 6); //tronco alla prima cifra decimale

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
