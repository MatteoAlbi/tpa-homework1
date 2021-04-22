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
    double cxShaft, cyShaft; //cooridnate centro del centro dell'albero
    double cxBiella, cyBiella; //coordinate centro coppia biella-manovella
    double cxPistone, cyPistone; //coordinate cetro coppia biella-pistone
    double L1 = device->stroke/2; //lunghezza manovella
    double L2 = device->lenBiella;  
    double q = device->angle * PI / 180; //angolo manovella in radianti
    double theta = atan2(-L1 * cos(q) / L2, sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) / L2);

    cxShaft = 400;
    cyShaft = 100;
    cxBiella = cxShaft + L1 * cos(q);
    cyBiella = cyShaft + L1 * sin(q);
    cxPistone = cxShaft;
    cyPistone = cyShaft + sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) + L1 * sin(q);

    //biella
    deviceSVG += "\t<rect x=\"" + to_string(cxBiella) + "\" y=\"" + to_string(cyBiella - device->wBiella/2) + "\" "; //def punto creazione 
    deviceSVG += "width=\"" + to_string(L2) + "\" height=\"" + to_string(device->wBiella) + "\" "; //def dimensioni 
    deviceSVG += "fill=\"blue\" "; //def colore
    deviceSVG += "transform=\"rotate(" + to_string(90 - theta * 180 / PI) + "," + to_string(cxBiella) + "," + to_string(cyBiella) + ")\" />\n\n"; //def rotazione 

    //pistone
    deviceSVG += "\t<rect x=\"" + to_string(cxPistone - device->dPistone/2) + "\" y=\"" + to_string(cyPistone - device->wBiella*7/10) + "\" "; //def punto creazione 
    deviceSVG += "width=\"" + to_string(device->dPistone) + "\" height=\"" + to_string(device->hPistone) + "\" "; //def dimensioni 
    deviceSVG += "fill=\"red\" />\n\n"; //def colore

    //manovella
        //cerchio albero
    deviceSVG += "\t<circle cx=\"" + to_string(cxShaft) + "\" cy=\"" + to_string(cyShaft) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->dShaft*7/10) +"\" fill=\"green\" />\n\n"; //def raggio e colore
        //cerchio coppia biella-manovella
    deviceSVG += "\t<circle cx=\"" + to_string(cxBiella) + "\" cy=\"" + to_string(cyBiella) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->wBiella/2) +"\" fill=\"green\" />\n\n"; //def raggio e colore
        //rettangolo che collega i due cerchi
    deviceSVG += "\t<rect x=\"" + to_string(cxShaft) + "\" y=\"" + to_string(cyShaft - device->wBiella*7/10) + "\" "; //def punto creazione 
    deviceSVG += "width=\"" + to_string(L1) + "\" height=\"" + to_string(device->wBiella*7/5) + "\" "; //def dimensioni 
    deviceSVG += "fill=\"green\" "; //def colore
    deviceSVG += "transform=\"rotate(" + to_string(q * 180 / PI) + "," + to_string(cxShaft) + "," + to_string(cyShaft) + ")\" />\n\n"; //def rotazione 

    //albero
    deviceSVG += "\t<circle cx=\"" + to_string(cxShaft) + "\" cy=\"" + to_string(cyShaft) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->dShaft/2) +"\" fill=\"gray\" />\n\n"; //def raggio e colore

    //coppie biella
        //manovella
    deviceSVG += "\t<circle cx=\"" + to_string(cxBiella) + "\" cy=\"" + to_string(cyBiella) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->wBiella/2) +"\" fill=\"blue\" />\n\n"; //def raggio e colore
        //pistone
    deviceSVG += "\t<circle cx=\"" + to_string(cxPistone) + "\" cy=\"" + to_string(cyPistone) + "\" "; // def punto creazione
    deviceSVG += "r=\"" + to_string(device->wBiella/2) +"\" fill=\"blue\" />\n\n"; //def raggio e colore
    
    //def file e dimensioni foglio
    deviceSVG = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\" >\n\n" + deviceSVG;
    deviceSVG = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n" + deviceSVG;
    deviceSVG += "</svg>\n";
    
    return deviceSVG;
};

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