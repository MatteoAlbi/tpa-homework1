#include "LBAMTTBiellaManovella.h"

LBAMTTdevice * LBAMTTinitDevice (cDbl dShaft, cDbl stroke, cDbl lenBiella, cDbl wBiella, cDbl hPistone, cDbl dPistone, cDbl angle){
    
    LBAMTTdevice * device = new(nothrow) LBAMTTdevice;
    
    device->dShaft = dShaft;
    device->stroke = stroke;
    device->lenBiella = lenBiella;
    device->wBiella = wBiella;
    device->hPistone = hPistone;
    device->dPistone = dPistone;
    device->angle = angle;

    if(LBAMTTcheckIntegrity(device)){
        delete device;
        return NULL;
    }
    else return device;
};

int LBAMTTcheckIntegrity (const LBAMTTdevice * device){
    //controllo che le misure non siano nulle o minori di zero
    if (device->dShaft <= 0) return 1;
    else if(device->stroke <= 0) return 1;
    else if (device->lenBiella <= 0) return 1;
    else if (device->wBiella <= 0) return 1;
    else if (device->hPistone <= 0) return 1;
    else if (device->dPistone <= 0) return 1;

    //controllo che la lunghezze rispettino i vincoli di costruzione (vedi README.md)
    else if (device->stroke/2  < device->dShaft/2 + device->wBiella/2) return 2; //vincolo lunghezza MANOVELLA
    else if (device->hPistone < device->wBiella*7/5 || device->dPistone < device->wBiella*7/5) return 3; //vincoli PISTONE
    else if (device->lenBiella < device->stroke/2 + device->dShaft*7/10 + device->wBiella*7/10) return 4; //vincolo lunghezza BIELLA
    else if (device->wBiella < device->lenBiella*1/6 || device->wBiella > device->dShaft) return 5; //vincoli larghezza BIELLA

    else return 0;
};

int LBAMTTsetDShaft (LBAMTTdevice * device, cDbl dShaft){
    double tmp = device->dShaft;
    device->dShaft = dShaft;
    
    if(LBAMTTcheckIntegrity(device)){
        device->dShaft = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetStroke (LBAMTTdevice * device, cDbl stroke){
    double tmp = device->stroke;
    device->stroke = stroke;
    
    if(LBAMTTcheckIntegrity(device)){
        device->stroke = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetLenBiella (LBAMTTdevice * device, cDbl lenBiella){
    double tmp = device->lenBiella;
    device->lenBiella = lenBiella;
    
    if(LBAMTTcheckIntegrity(device)){
        device->lenBiella = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetWBiella (LBAMTTdevice * device, cDbl wBiella){
    double tmp = device->wBiella;
    device->wBiella = wBiella;
    
    if(LBAMTTcheckIntegrity(device)){
        device->wBiella = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetHPistone (LBAMTTdevice * device, cDbl hPistone){
    double tmp = device->hPistone;
    device->hPistone = hPistone;
    
    if(LBAMTTcheckIntegrity(device)){
        device->hPistone = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetDPistone (LBAMTTdevice * device, cDbl dPistone){
    double tmp = device->dPistone;
    device->dPistone = dPistone;
    
    if(LBAMTTcheckIntegrity(device)){
        device->dPistone = tmp;
        return 1;
    }
    else return 0;
};

void LBAMTTsetAngle (LBAMTTdevice * device, cDbl angle){
    device->angle = angle;
};

int LBAMTTdelete (LBAMTTdevice * device){
    if(device == NULL) return 1;
    else {
        delete device;
        return 0;
    }
}

string LBAMTTrectStringSVG(double x, double y, double w, double h, string color, double rotation, double xr, double yr){
    string rect  = "";
    rect += "<rect x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" "; //def punto creazione 
    rect += "width=\"" + to_string(w) + "\" height=\"" + to_string(h) + "\" "; //def dimensioni 
    rect += "fill=\"" + color + "\" "; //def colore
    if(rotation != 0.0) rect += "transform=\"rotate(" + to_string(rotation) + "," + to_string(xr) + "," + to_string(yr) + ")\" "; //def rotazione 
    rect += "/>\n\n";

    return rect;
}

string LBAMTTcircleStringSVG(double x, double y, double r, string color){
    string circle = "";
    circle += "<circle cx=\"" + to_string(x) + "\" cy=\"" + to_string(y) + "\" "; // def punto creazione
    circle += "r=\"" + to_string(r) +"\" fill=\"" + color + "\" />\n\n"; //def raggio e colore

    return circle;
}

string LBAMTTlineStringSVG(double x1, double y1, double x2, double y2, int stroke, string color){
    string line = "";
    line += "<line x1=\"" + to_string(x1) + "\" y1=\"" + to_string(y1) + "\" x2=\"" + to_string(x2) + "\" y2=\"" + to_string(y2) + "\" "; //def linea
    line += "style=\"stroke:" + color + ";stroke-width:" + to_string(stroke) + "\" />\n\n"; //def stile

    return line;
}

string LBAMTTdeviceToStringSVG (LBAMTTdevice * device, double cxShaft, double cyShaft, bool quote, bool header){
    if(device == NULL) return NULL;

    double cxBiella, cyBiella; //coordinate centro coppia biella-manovella
    double cxPistone, cyPistone; //coordinate cetro coppia biella-pistone
    double L1 = device->stroke/2; //lunghezza manovella
    double L2 = device->lenBiella;  
    double q = PI/2 - device->angle * PI / 180.0; //angolo manovella in radianti
    double theta = atan2(-L1 * cos(q) / L2, sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) / L2); //angolo biella
    
    cxBiella = cxShaft + L1 * cos(q);
    cyBiella = cyShaft + L1 * sin(q);
    cxPistone = cxShaft;
    cyPistone = cyShaft + sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) + L1 * sin(q);

    string deviceSVG = "";

    //biella
    deviceSVG += LBAMTTrectStringSVG(cxBiella, cyBiella - device->wBiella/2, L2, device->wBiella, "blue", 90 - theta * 180 / PI, cxBiella, cyBiella); 

    //pistone
    deviceSVG += LBAMTTrectStringSVG(cxPistone - device->dPistone/2, cyPistone - device->wBiella*7/10, device->dPistone, device->hPistone, "red");

    //manovella
        //cerchio albero
    deviceSVG += LBAMTTcircleStringSVG(cxShaft, cyShaft, device->dShaft*7/10, "green");
        //cerchio coppia biella-manovella
    deviceSVG += LBAMTTcircleStringSVG(cxBiella, cyBiella, device->wBiella*7/10, "green");
        //rettangolo che collega i due cerchi
    deviceSVG += LBAMTTrectStringSVG(cxShaft, cyShaft - device->wBiella*7/10, L1, device->wBiella*7/5, "green", q * 180 / PI, cxShaft, cyShaft);

    //albero
    deviceSVG += LBAMTTcircleStringSVG(cxShaft, cyShaft, device->dShaft/2, "gray");

    //coppie biella
        //manovella
    deviceSVG += LBAMTTcircleStringSVG(cxBiella, cyBiella, device->wBiella/2, "Blue");
        //pistone
    deviceSVG += LBAMTTcircleStringSVG(cxPistone, cyPistone, device->wBiella/2, "Blue");
    
    //quote
    if(quote){
        double lQuote = 40; //lunghezza linee laterali di quota
        double stroke = 2; //spessore delle linee di quota
        double distQuote = lQuote/2; //distanza quota dal pezzo

    //dShaft
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a sinistra
            deviceSVG += LBAMTTlineStringSVG(cxShaft - device->dShaft*7/10 - lQuote*3/2, cyShaft - device->dShaft/2, cxShaft, cyShaft - device->dShaft/2, stroke); //linea laterale superiore
            deviceSVG += LBAMTTlineStringSVG(cxShaft - device->dShaft*7/10 - lQuote*3/2, cyShaft + device->dShaft/2, cxShaft, cyShaft + device->dShaft/2, stroke); //linea laterale inferiore
            deviceSVG += LBAMTTlineStringSVG(cxShaft - device->dShaft*7/10 - lQuote, cyShaft - device->dShaft/2, cxShaft - device->dShaft*7/10 - lQuote, cyShaft + device->dShaft/2, stroke); //linea centrale
            //scritta
        }
        else{ //posiziono a destra
            deviceSVG += LBAMTTlineStringSVG(cxShaft + device->dShaft*7/10 + lQuote*3/2, cyShaft - device->dShaft/2, cxShaft, cyShaft - device->dShaft/2, stroke); //linea laterale superiore
            deviceSVG += LBAMTTlineStringSVG(cxShaft + device->dShaft*7/10 + lQuote*3/2, cyShaft + device->dShaft/2, cxShaft, cyShaft + device->dShaft/2, stroke); //linea laterale inferiore
            deviceSVG += LBAMTTlineStringSVG(cxShaft + device->dShaft*7/10 + lQuote, cyShaft - device->dShaft/2, cxShaft + device->dShaft*7/10 + lQuote, cyShaft + device->dShaft/2, stroke); //linea centrale
            //scritta
        }

    //wBiella
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a destra
            deviceSVG += LBAMTTlineStringSVG(cxBiella + device->wBiella*7/10 + lQuote*3/2, cyBiella - device->wBiella/2, cxBiella, cyBiella - device->wBiella/2, stroke); //linea laterale superiore
            deviceSVG += LBAMTTlineStringSVG(cxBiella + device->wBiella*7/10 + lQuote*3/2, cyBiella + device->wBiella/2, cxBiella, cyBiella + device->wBiella/2, stroke); //linea laterale inferiore
            deviceSVG += LBAMTTlineStringSVG(cxBiella + device->wBiella*7/10 + lQuote, cyBiella - device->wBiella/2, cxBiella + device->wBiella*7/10 + lQuote, cyBiella + device->wBiella/2, stroke); //linea centrale
            //scritta
            
        }
        else{ //posiziono a sinistra
            deviceSVG += LBAMTTlineStringSVG(cxBiella - device->wBiella*7/10 - lQuote*3/2, cyBiella - device->wBiella/2, cxBiella, cyBiella - device->wBiella/2, stroke); //linea laterale superiore
            deviceSVG += LBAMTTlineStringSVG(cxBiella - device->wBiella*7/10 - lQuote*3/2, cyBiella + device->wBiella/2, cxBiella, cyBiella + device->wBiella/2, stroke); //linea laterale inferiore
            deviceSVG += LBAMTTlineStringSVG(cxBiella - device->wBiella*7/10 - lQuote, cyBiella - device->wBiella/2, cxBiella - device->wBiella*7/10 - lQuote, cyBiella + device->wBiella/2, stroke); //linea centrale
            //scritta
        }

    //hPistone
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a sinistra
            deviceSVG += LBAMTTlineStringSVG(cxPistone - device->dPistone/2 - lQuote*3/2, cyPistone - device->wBiella*7/10, cxPistone - device->dPistone/2, cyPistone - device->wBiella*7/10, stroke); //linea laterale superiore
            deviceSVG += LBAMTTlineStringSVG(cxPistone - device->dPistone/2 - lQuote*3/2, cyPistone + device->hPistone - device->wBiella*7/10, cxPistone - device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10, stroke); //linea laterale inferiore
            deviceSVG += LBAMTTlineStringSVG(cxPistone - device->dPistone/2 - lQuote, cyPistone - device->wBiella*7/10, cxPistone - device->dPistone/2 - lQuote, cyPistone + device->hPistone - device->wBiella*7/10, stroke); //linea centrale
            //scritta
        }
        else{ //posiziono a destra
            deviceSVG += LBAMTTlineStringSVG(cxPistone + device->dPistone/2 + lQuote*3/2, cyPistone - device->wBiella*7/10, cxPistone + device->dPistone/2, cyPistone - device->wBiella*7/10, stroke); //linea laterale superiore
            deviceSVG += LBAMTTlineStringSVG(cxPistone + device->dPistone/2 + lQuote*3/2, cyPistone + device->hPistone - device->wBiella*7/10, cxPistone + device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10, stroke); //linea laterale inferiore
            deviceSVG += LBAMTTlineStringSVG(cxPistone + device->dPistone/2 + lQuote, cyPistone - device->wBiella*7/10, cxPistone + device->dPistone/2 + lQuote, cyPistone + device->hPistone - device->wBiella*7/10, stroke); //linea centrale
            //scritta
        }

    //dPistone
        deviceSVG += LBAMTTlineStringSVG(cxPistone - device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10, cxPistone - device->dPistone/2, cyPistone - device->wBiella*7/10 + device->hPistone + lQuote*3/2, stroke); //linea laterale superiore
        deviceSVG += LBAMTTlineStringSVG(cxPistone + device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10, cxPistone + device->dPistone/2, cyPistone - device->wBiella*7/10 + device->hPistone + lQuote*3/2, stroke); //linea laterale inferiore
        deviceSVG += LBAMTTlineStringSVG(cxPistone - device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10 + lQuote, cxPistone + device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10 + lQuote, stroke); //linea centrale
    
    //lenBiella
        distQuote = lQuote;
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a sinistra
            deviceSVG += LBAMTTlineStringSVG(cxBiella, cyBiella, cxBiella + (device->wBiella*7/10 + distQuote + lQuote) * cos(-theta), cyBiella + (device->wBiella*7/10 + distQuote + lQuote) * sin(-theta), stroke); //linea laterale superiore
            deviceSVG += LBAMTTlineStringSVG(cxPistone, cyPistone, cxPistone + (device->wBiella*7/10 + distQuote + lQuote) * cos(-theta), cyPistone + (device->wBiella*7/10 + distQuote + lQuote) * sin(-theta), stroke); //linea laterale superiore
            //scritta
        }
        else{ //posiziono a destra
            deviceSVG += LBAMTTlineStringSVG(cxBiella, cyBiella, cxBiella + (device->wBiella*7/10 + distQuote + lQuote) * cos(-theta + PI), cyBiella + (device->wBiella*7/10 + distQuote + lQuote) * sin(-theta + PI), stroke); //linea laterale superiore
            deviceSVG += LBAMTTlineStringSVG(cxPistone, cyPistone, cxPistone + (device->wBiella*7/10 + distQuote + lQuote) * cos(-theta + PI), cyPistone + (device->wBiella*7/10 + distQuote + lQuote) * sin(-theta + PI), stroke); //linea laterale superiore
            //scritta
        }
        
    }
    

    //def file e dimensioni foglio
    if(header){
        deviceSVG = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\" >\n\n" + deviceSVG;
        deviceSVG = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n" + deviceSVG;
        deviceSVG += "</svg>\n";
    }
    
    return deviceSVG;
}

vector<string> LBAMTTsplitString (string s, string delimiter){ //stringa da splittare passata in copia
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {//cerca il delimitatore -> se non lo trova esce
        token = s.substr (pos_start, pos_end - pos_start);//estrae la porzione di stringa tra l'inizio e la posizione del delimitatore; ATT: modifica la stringa
        pos_start = pos_end + delim_len; //aggiorna il puntatore a inizio stringa
        res.push_back (token); //salva la sottostringa estratta
    }

    res.push_back (s.substr (pos_start)); //quando non trova più il delimitatore salva la parte di stringa rimanente
    return res;
}

int LBAMTTsaveToFile(string stringSVG, string fileName){
    if (fileName == "") return 1;
    //controllo se l'estensione è corretta
    vector<string> checkFileName = LBAMTTsplitString(fileName,".");
    if (checkFileName.size() != 2) return 1;
    if (checkFileName.back() != "svg") return 1;

    ofstream MyFile(fileName);
    MyFile << stringSVG;
    MyFile.close();

    return 0;
}