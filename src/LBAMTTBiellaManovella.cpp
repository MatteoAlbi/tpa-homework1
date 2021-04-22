#include "LBAMTTBiellaManovella.h"

LBAMTTdevice * LBAMTTinitDevice (cDbl dShaft, cDbl stroke, cDbl lenBiella, cDbl wBiella, cDbl hPistone, cDbl dPistone, cDbl angle, LBAMTTorientation verse){
    
    LBAMTTdevice * device = new(nothrow) LBAMTTdevice;
    
    device->dShaft = dShaft;
    device->stroke = stroke;
    device->lenBiella = lenBiella;
    device->wBiella = wBiella;
    device->hPistone = hPistone;
    device->dPistone = dPistone;
    device->angle = angle;
    device->verse = verse;

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
    else if (device->lenBiella < device->stroke/2 + device->dShaft*7/10) return 4; //vincolo lunghezza BIELLA
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

void LBAMTTsetVerse (LBAMTTdevice * device, LBAMTTorientation verse){
    device->verse = verse;
};

int LBAMTTdelete (LBAMTTdevice * device){
    if(device == NULL) return 1;
    else {
        delete device;
        return 0;
    }
}

string LBAMTTtoStringSVG (LBAMTTdevice * device){
    string deviceSVG = "";
    double xcShaft, ycShaft; //cooridnate centro del centro dell'albero
    double xcBiella, ycBiella; //coordinate centro coppia biella-manovella
    double xcPistone, ycPistone; //coordinate cetro coppia biella-pistone
    double L1 = device->stroke/2; //lunghezza manovella
    double L2 = device->lenBiella;  
    double q = device->angle * PI / 180; //angolo manovella in radianti
    double theta = atan2(-L1 * cos(q) / L2, sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) / L2);

    xcShaft = 400;
    ycShaft = 100;
    xcBiella = xcShaft + L1 * cos(q);
    ycBiella = ycShaft + L1 * sin(q);
    xcPistone = xcShaft;
    ycPistone = ycShaft + sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) + L1 * sin(q);

    //biella
    deviceSVG += "<rect x=\"" + to_string(xcBiella) + "\" y=\"" + to_string(ycBiella - device->wBiella/2) + "\" "; //def punto creazione 
    deviceSVG += "width=\"" + to_string(L2) + "\" height\"" + to_string(device->wBiella) + "\" "; //def dimensioni 
    deviceSVG += "style=\"fill:blue\" "; //def colore
    deviceSVG += "transform=\"rotate(" + to_string(theta * 180 / PI) + ")\" />\n\n"; //def rotazione 

    //pistone
    deviceSVG += "<rect x=\"" + to_string(xcPistone - device->wBiella*7/10) + "\" y=\"" + to_string(ycPistone - device->wBiella*7/10) + "\" "; //def punto creazione 
    deviceSVG += "width=\"" + to_string(device->dPistone) + "\" height\"" + to_string(device->hPistone) + "\" "; //def dimensioni 
    deviceSVG += "style=\"fill:red\" />\n\n"; //def colore

    //manovella
        //cerchio albero
    deviceSVG += "<circle xc=\"" + to_string(xcShaft) + "\" yc=\"" + to_string(ycShaft) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->dShaft*7/10) +"\" style=\"fill:green\" />\n\n"; //def raggio e colore
        //cerchio coppia biella-manovella
    deviceSVG += "<circle xc=\"" + to_string(xcBiella) + "\" yc=\"" + to_string(ycBiella) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->wBiella/2) +"\" style=\"fill:green\" />\n\n"; //def raggio e colore
        //rettangolo che collega i due cerchi
    deviceSVG += "<rect x=\"" + to_string(xcShaft) + "\" y=\"" + to_string(ycShaft - device->wBiella*7/10) + "\" "; //def punto creazione 
    deviceSVG += "width=\"" + to_string(L1) + "\" height\"" + to_string(device->wBiella*7/5) + "\" "; //def dimensioni 
    deviceSVG += "style=\"fill:green\" "; //def colore
    deviceSVG += "transform=\"rotate(" + to_string(q * 180 / PI) + ")\" />\n\n"; //def rotazione 

    //albero
    deviceSVG += "<circle xc=\"" + to_string(xcShaft) + "\" yc=\"" + to_string(ycShaft) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->dShaft/2) +"\" style=\"fill:gray\" />\n\n"; //def raggio e colore

    //coppie biella
        //manovella
    deviceSVG += "<circle xc=\"" + to_string(xcBiella) + "\" yc=\"" + to_string(ycBiella) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->wBiella/2) +"\" style=\"fill:blue\" />\n\n"; //def raggio e colore
        //pistone
    deviceSVG += "<circle xc=\"" + to_string(xcPistone) + "\" yc=\"" + to_string(ycPistone) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->wBiella/2) +"\" style=\"fill:blue\" />\n\n"; //def raggio e colore
    
    //def file e dimensioni foglio
    deviceSVG = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\" >\n\n" + deviceSVG;
    deviceSVG = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n" + deviceSVG;
    deviceSVG += "</svg>\n";
    
    return deviceSVG;
};