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

    //controllo sia stato passato un device
    if (device == NULL) return 1;

    //controllo che le misure non siano nulle o minori di zero
    if (device->dShaft <= 0) return -1;
    else if(device->stroke <= 0) return -1;
    else if (device->lenBiella <= 0) return -1;
    else if (device->wBiella <= 0) return -1;
    else if (device->hPistone <= 0) return -1;
    else if (device->dPistone <= 0) return -1;

    //controllo che la lunghezze rispettino i vincoli di costruzione (vedi README.md)
    else if (device->stroke/2  < device->dShaft/2 + device->wBiella/2) return 2; //vincolo lunghezza MANOVELLA
    else if (device->hPistone < device->wBiella*7/5 || device->dPistone < device->wBiella*7/5) return 3; //vincoli PISTONE
    else if (device->lenBiella < device->stroke/2 + device->dShaft*7/10 + device->wBiella*7/10) return 4; //vincolo lunghezza BIELLA
    else if (device->wBiella < device->lenBiella*1/6 || device->wBiella > device->dShaft) return 5; //vincoli larghezza BIELLA

    else return 0;
};

int LBAMTTdelete (LBAMTTdevice * device){

    //controllo sia stato passato un device
    if (device == NULL) return 1;
    
    delete device;

    return 0;
}

int LBAMTTsetDShaft (LBAMTTdevice * device, cDbl dShaft){

    //controllo sia stato passato un device
    if (device == NULL) return 1;

    double tmp = device->dShaft;
    device->dShaft = dShaft;
    
    if(LBAMTTcheckIntegrity(device)){
        device->dShaft = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetStroke (LBAMTTdevice * device, cDbl stroke){

    //controllo sia stato passato un device
    if (device == NULL) return 1;

    double tmp = device->stroke;
    device->stroke = stroke;
    
    if(LBAMTTcheckIntegrity(device)){
        device->stroke = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetLenBiella (LBAMTTdevice * device, cDbl lenBiella){

    //controllo sia stato passato un device
    if (device == NULL) return 1;

    double tmp = device->lenBiella;
    device->lenBiella = lenBiella;
    
    if(LBAMTTcheckIntegrity(device)){
        device->lenBiella = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetWBiella (LBAMTTdevice * device, cDbl wBiella){

    //controllo sia stato passato un device
    if (device == NULL) return 1;

    double tmp = device->wBiella;
    device->wBiella = wBiella;
    
    if(LBAMTTcheckIntegrity(device)){
        device->wBiella = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetHPistone (LBAMTTdevice * device, cDbl hPistone){

    //controllo sia stato passato un device
    if (device == NULL) return 1;

    double tmp = device->hPistone;
    device->hPistone = hPistone;
    
    if(LBAMTTcheckIntegrity(device)){
        device->hPistone = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetDPistone (LBAMTTdevice * device, cDbl dPistone){

    //controllo sia stato passato un device
    if (device == NULL) return 1;

    double tmp = device->dPistone;
    device->dPistone = dPistone;
    
    if(LBAMTTcheckIntegrity(device)){
        device->dPistone = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTsetAngle (LBAMTTdevice * device, cDbl angle){

    //controllo sia stato passato un device
    if (device == NULL) return 1;

    device->angle = angle;

    return 0;
};

string LBAMTTdeviceToStringSVG (LBAMTTdevice * device, double cxShaft, double cyShaft, bool quote, bool header){

    //controllo sia stato passato un device
    if(device == NULL) return "";

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
        deviceSVG = LBAMTTheaderSVG(deviceSVG);
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

int LBAMTTsaveToFile(string s, string fileName){

    //controllo sia stato passato un nome
    if (fileName == "") return 1;

    //controllo se l'estensione è corretta
    vector<string> checkFileName = LBAMTTsplitString(fileName,".");
    if (checkFileName.front() == "") return 1;
    if (checkFileName.back() != "svg") return 1;

    ofstream fout(fileName);
    fout << s;
    fout.close();

    return 0;
}

string LBAMTTloadFromFile(string fileName){

    //controllo sia stato passato un nome
    if (fileName == "") return "";

    //controllo se l'estensione è corretta
    vector<string> checkFileName = LBAMTTsplitString(fileName,".");
    if (checkFileName.front() == "") return "";
    if (checkFileName.back() != "svg") return "";

    ifstream fin(fileName);
    stringstream buffer;

    //controllo se il file è stato aperto
    if(! fin.is_open()) return "";

    buffer << fin.rdbuf();
    string s = buffer.str();

    return s;
}

LBAMTTdevice * LBAMTTdeviceFromStringSVG(string s){

    //splitto per ottenere stringhe contenti le singole figure
    vector<string> vTot = LBAMTTsplitString(s, ">\n\n<");
    
    //elimino le stringhe che non contengono figure di interesse
    int i = 0;
    while(i < vTot.size()){
        if(vTot[i][0] != 'r' && vTot[i][0] != 'c') vTot.erase(vTot.begin() + i);
        else i++;
    }

    //controllo numero figure
    if(vTot.size() != 8) return NULL;

    //controllo che la successione di figure sia corretta
    string control = "rrccrccc";
    for(i = 0; i < 8; i++) if(vTot[i][0] != control[i]) return NULL;

    //estrazione dati
    double dShaft;
    double stroke; 
    double lenBiella;
    double wBiella;
    double hPistone; 
    double dPistone; 
    double angle;

    vector<string> vTmp;
    //biella
    vTmp = LBAMTTsplitString(vTot[0],"\"");
    lenBiella = atof(vTmp[5].c_str());
    wBiella = atof(vTmp[7].c_str());

    //pistone
    vTmp = LBAMTTsplitString(vTot[1],"\"");
    dPistone = atof(vTmp[5].c_str());
    hPistone = atof(vTmp[7].c_str());

    //manovella
    vTmp = LBAMTTsplitString(vTot[4],"\"");
    stroke = 2 * atof(vTmp[5].c_str());
    vTmp = LBAMTTsplitString(vTmp[11],"(");
    vTmp = LBAMTTsplitString(vTmp[1],",");//ottengo valori del rotate
    angle = 90 - atof(vTmp[0].c_str());

    //shaft
    vTmp = LBAMTTsplitString(vTot[5],"\"");
    dShaft = 2 * atof(vTmp[5].c_str());

    return LBAMTTinitDevice(dShaft, stroke, lenBiella, wBiella, hPistone, dPistone, angle);
}