#include "LBAMTTBiellaManovella.h"
#include "LBAMTTcadSVG.h"

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

string LBAMTTdeviceToStringSVG (LBAMTTdevice * device, double cxShaft, double cyShaft, bool quote, bool header){
    if(device == NULL) return NULL;

    double cxBiella, cyBiella; //coordinate centro coppia biella-manovella
    double cxPistone, cyPistone; //coordinate cetro coppia biella-pistone
    double L1 = device->stroke/2; //lunghezza manovella
    double L2 = device->lenBiella;  
    double q = PI/2 - device->angle * PI / 180.0; //angolo manovella in radianti
    double theta = atan2(-L1 * cos(q) / L2, sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) / L2); //angolo biella in radianti
    
    cxBiella = cxShaft + L1 * cos(q);
    cyBiella = cyShaft + L1 * sin(q);
    cxPistone = cxShaft;
    cyPistone = cyShaft + sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) + L1 * sin(q);

    string deviceSVG = "";

    //definizione marker arrow
    deviceSVG += LBAMTTarrowMarkerSVG(); 

    //biella
    deviceSVG += LBAMTTrectSVG(cxBiella, cyBiella - device->wBiella/2, 
                               L2, device->wBiella, 
                               "blue", 
                               90 - theta * 180 / PI, cxBiella, cyBiella);
    deviceSVG += "\n"; 

    //pistone
    deviceSVG += LBAMTTrectSVG(cxPistone - device->dPistone/2, cyPistone - device->wBiella*7/10, 
                               device->dPistone, device->hPistone, 
                               "red");
    deviceSVG += "\n"; 

    //manovella
        //cerchio albero
    deviceSVG += LBAMTTcircleSVG(cxShaft, cyShaft, 
                                 device->dShaft*7/10, 
                                 "green");
    deviceSVG += "\n"; 
        //cerchio coppia biella-manovella
    deviceSVG += LBAMTTcircleSVG(cxBiella, cyBiella, 
                                 device->wBiella*7/10, 
                                 "green");
    deviceSVG += "\n"; 
        //rettangolo che collega i due cerchi
    deviceSVG += LBAMTTrectSVG(cxShaft, cyShaft - device->wBiella*7/10, 
                               L1, device->wBiella*7/5, 
                               "green", 
                               q * 180 / PI, cxShaft, cyShaft);
    deviceSVG += "\n"; 

    //albero
    deviceSVG += LBAMTTcircleSVG(cxShaft, cyShaft, 
                                 device->dShaft/2, 
                                 "gray");
    deviceSVG += "\n"; 

    //coppie biella
        //manovella
    deviceSVG += LBAMTTcircleSVG(cxBiella, cyBiella, 
                                 device->wBiella/2, 
                                 "Blue");
    deviceSVG += "\n"; 
        //pistone
    deviceSVG += LBAMTTcircleSVG(cxPistone, cyPistone, 
                                 device->wBiella/2, 
                                 "Blue");
    deviceSVG += "\n"; 
    
    //quote
    if(quote){
        double lQuote = device->wBiella / 3; //lunghezza linee laterali di quota
        double distQuote = lQuote*2; //distanza quota dal pezzo

    //dShaft
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a sinistra
            deviceSVG += LBAMTTquoteDistSVG(cxShaft, cyShaft - device->dShaft/2, 
                                            cxShaft, cyShaft + device->dShaft/2, 
                                            device->dShaft*7/10 + distQuote, lQuote, true);
        }
        else{ //posiziono a destra
            deviceSVG += LBAMTTquoteDistSVG(cxShaft, cyShaft - device->dShaft/2, 
                                            cxShaft, cyShaft + device->dShaft/2, 
                                            device->dShaft*7/10 + distQuote, lQuote, false);
        }
        deviceSVG += "\n"; 

    //wBiella
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a destra
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella - device->wBiella/2, 
                                            cxBiella, cyBiella + device->wBiella/2, 
                                            device->wBiella*7/10 + distQuote, lQuote, false);
        }
        else{ //posiziono a sinistra
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella - device->wBiella/2, 
                                            cxBiella, cyBiella + device->wBiella/2, 
                                            device->wBiella*7/10 + distQuote, lQuote, true);
        }
        deviceSVG += "\n"; 

    //hPistone
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a sinistra
            deviceSVG += LBAMTTquoteDistSVG(cxPistone - device->dPistone/2, cyPistone - device->wBiella*7/10, 
                                            cxPistone - device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10, 
                                            distQuote, lQuote, true);
        }
        else{ //posiziono a destra
            deviceSVG += LBAMTTquoteDistSVG(cxPistone + device->dPistone/2, cyPistone - device->wBiella*7/10, 
                                            cxPistone + device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10, 
                                            distQuote, lQuote, false);
        }
        deviceSVG += "\n"; 

    //dPistone
        deviceSVG += LBAMTTquoteDistSVG(cxPistone - device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10, 
                                        cxPistone + device->dPistone/2, cyPistone + device->hPistone - device->wBiella*7/10, 
                                        distQuote, lQuote, true);
        deviceSVG += "\n"; 

    //stroke
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a sinistra
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella, 
                                            cxShaft, cyShaft, 
                                            device->dShaft*7/10 + distQuote, lQuote, true);
        }
        else{ //posiziono a destra
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella, 
                                            cxShaft, cyShaft, 
                                            device->dShaft*7/10 + distQuote, lQuote, false);
        }
        deviceSVG += "\n"; 

     //lenBiella
        distQuote = device->dPistone * 2/3 + lQuote;
        if(fmod(device->angle, 360.0) < 180.0){ //posiziono a sinistra
            deviceSVG += LBAMTTquoteDistSVG(cxPistone, cyPistone, 
                                            cxBiella, cyBiella, 
                                            distQuote, lQuote, true);
        }
        else{ //posiziono a destra
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella, 
                                            cxPistone, cyPistone, 
                                            distQuote, lQuote, true);
        }
        deviceSVG += "\n";    
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