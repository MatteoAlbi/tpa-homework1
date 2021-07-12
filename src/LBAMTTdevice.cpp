#include "LBAMTTdevice.h"

LBAMTTdevice * LBAMTTinitDevice (cDbl dShaft, cDbl stroke, cDbl lRod, cDbl wRod, cDbl hPiston, cDbl dPiston, cDbl angle){
    LBAMTTdevice * device = new(nothrow) LBAMTTdevice;
    
    device->dShaft = dShaft;
    device->stroke = stroke;
    device->lRod = lRod;
    device->wRod = wRod;
    device->hPiston = hPiston;
    device->dPiston = dPiston;
    device->angle = angle;

    if(LBAMTTcheckDeviceIntegrity(device)){
        delete device;
        return NULL;
    }
    else return device;
};

int LBAMTTcheckDeviceIntegrity (const LBAMTTdevice * device){
    if (device == NULL) return 1;

    if (device->dShaft <= 0) return -1;
    else if(device->stroke <= 0) return -1;
    else if (device->lRod <= 0) return -1;
    else if (device->wRod <= 0) return -1;
    else if (device->hPiston <= 0) return -1;
    else if (device->dPiston <= 0) return -1;

    //constraints check (see README.md)
    else if (device->stroke/2  < device->dShaft/2 + device->wRod/2) return 2; //crank lenght constraint
    else if (device->hPiston < device->wRod*7/5 || device->dPiston < device->wRod*7/5) return 3; //piston constraints
    else if (device->lRod < device->stroke/2 + device->dShaft*7/10 + device->wRod*7/10) return 4; //lRod cosntraints
    else if (device->wRod < device->lRod*1/6 || device->wRod > device->dShaft) return 5; //wRod constraints

    else return 0;
};

bool LBAMTTdeviceCompare (const LBAMTTdevice * device1, const LBAMTTdevice * device2){
    bool ret = true;
    ret*= device1->dShaft == device2->dShaft;
    ret*= device1->stroke == device2->stroke;
    ret*= device1->lRod == device2->lRod;
    ret*= device1->wRod == device2->wRod;
    ret*= device1->hPiston == device2->hPiston;
    ret*= device1->dPiston == device2->dPiston;

    return ret;
}

int LBAMTTdelete (LBAMTTdevice * device){
    if (device == NULL) return 1;
    
    delete device;

    return 0;
}

int LBAMTTdeviceSetDShaft (LBAMTTdevice * device, cDbl dShaft){
    if (device == NULL) return 1;

    double tmp = device->dShaft;
    device->dShaft = dShaft;
    
    if(LBAMTTcheckDeviceIntegrity(device)){
        device->dShaft = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTdeviceSetStroke (LBAMTTdevice * device, cDbl stroke){
    if (device == NULL) return 1;

    double tmp = device->stroke;
    device->stroke = stroke;
    
    if(LBAMTTcheckDeviceIntegrity(device)){
        device->stroke = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTdeviceSetLRod (LBAMTTdevice * device, cDbl lRod){
    if (device == NULL) return 1;

    double tmp = device->lRod;
    device->lRod = lRod;
    
    if(LBAMTTcheckDeviceIntegrity(device)){
        device->lRod = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTdeviceSetWRod (LBAMTTdevice * device, cDbl wRod){
    if (device == NULL) return 1;

    double tmp = device->wRod;
    device->wRod = wRod;
    
    if(LBAMTTcheckDeviceIntegrity(device)){
        device->wRod = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTdeviceSetHPiston (LBAMTTdevice * device, cDbl hPiston){
    if (device == NULL) return 1;

    double tmp = device->hPiston;
    device->hPiston = hPiston;
    
    if(LBAMTTcheckDeviceIntegrity(device)){
        device->hPiston = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTdeviceSetDPiston (LBAMTTdevice * device, cDbl dPiston){
    if (device == NULL) return 1;

    double tmp = device->dPiston;
    device->dPiston = dPiston;
    
    if(LBAMTTcheckDeviceIntegrity(device)){
        device->dPiston = tmp;
        return 1;
    }
    else return 0;
};

int LBAMTTdeviceSetAngle (LBAMTTdevice * device, cDbl angle){
    if (device == NULL) return 1;

    device->angle = angle;

    return 0;
};

string LBAMTTdeviceToStringSVG (LBAMTTdevice * device, double cxShaft, double cyShaft, bool quote, bool header, LBAMTTanimation * anim){
    if(device == NULL) return "";

    if(quote) anim = NULL; //priority to quotes

    double cxBiella, cyBiella; //crank-connecting rod joint's center
    double cxPistone, cyPistone; //piston-connecting rod joint's center
    double L1 = device->stroke/2; //crank lenght
    double L2 = device->lRod;  
    double q = PI/2 - device->angle * PI / 180.0; //crank angle in radiants
    double theta = atan2(L1 * cos(q) / L2, - sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) / L2); //connecting rod angle in radiants
    
    cxBiella = cxShaft + L1 * cos(q);
    cyBiella = cyShaft + L1 * sin(q);
    cxPistone = cxShaft;
    cyPistone = cyShaft - sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) + L1 * sin(q);

    string deviceSVG = "";

    //connecting rod
    deviceSVG += LBAMTTrectSVG(cxBiella, cyBiella - device->wRod/2, 
                               L2, device->wRod, 
                               "darkgray", 
                               90 + theta * 180 / PI, cxBiella, cyBiella,
                               LBAMTTappearSVG("darkgray", anim));
    deviceSVG += "\n"; 

    //piston
    deviceSVG += LBAMTTrectSVG(cxPistone - device->dPiston/2, cyPistone + device->wRod*7/10 - device->hPiston , 
                               device->dPiston, device->hPiston, 
                               "gray", 0.0, 0.0, 0.0, LBAMTTappearSVG("gray", anim));
    deviceSVG += "\n"; 

    //crank
        //shaft circle + coupling factor
    deviceSVG += LBAMTTcircleSVG(cxShaft, cyShaft, 
                                 device->dShaft*7/10, 
                                 "gray", LBAMTTappearSVG("gray", anim));
    deviceSVG += "\n"; 
        //crank-connecting rod joint circle + coupling factor
    deviceSVG += LBAMTTcircleSVG(cxBiella, cyBiella, 
                                 device->wRod*7/10, 
                                 "gray", LBAMTTappearSVG("gray", anim));
    deviceSVG += "\n"; 
        //rectangle that connects the two circles
    deviceSVG += LBAMTTrectSVG(cxShaft, cyShaft - device->wRod*7/10, 
                               L1, device->wRod*7/5, 
                               "gray", 
                               q * 180 / PI, cxShaft, cyShaft,
                               LBAMTTappearSVG("gray", anim));
    deviceSVG += "\n"; 

    //shaft
    deviceSVG += LBAMTTcircleSVG(cxShaft, cyShaft, 
                                 device->dShaft/2, 
                                 "darkgray", LBAMTTappearSVG("darkgray", anim));
    deviceSVG += "\n"; 

    //connecting rod joints
        //crank
    deviceSVG += LBAMTTcircleSVG(cxBiella, cyBiella, 
                                 device->wRod/2, 
                                 "darkgray", LBAMTTappearSVG("darkgray", anim));
    deviceSVG += "\n"; 
        //piston
    deviceSVG += LBAMTTcircleSVG(cxPistone, cyPistone, 
                                 device->wRod/2, 
                                 "darkgray", LBAMTTappearSVG("darkgray", anim));
    deviceSVG += "\n"; 
    
    //quotes
    if(quote){
        double lQuote = device->wRod / 3;
        double distQuote = lQuote*2;
        deviceSVG = LBAMTTarrowMarkerSVG() + deviceSVG; //marker arrow def

    //dShaft
        if(fmod(device->angle, 360.0) < 180.0){ //quote on left
            deviceSVG += LBAMTTquoteDistSVG(cxShaft, cyShaft - device->dShaft/2, 
                                            cxShaft, cyShaft + device->dShaft/2, 
                                            device->dShaft*7/10 + distQuote, lQuote, true);
        }
        else{ //quote on right
            deviceSVG += LBAMTTquoteDistSVG(cxShaft, cyShaft - device->dShaft/2, 
                                            cxShaft, cyShaft + device->dShaft/2, 
                                            device->dShaft*7/10 + distQuote, lQuote, false);
        }
        deviceSVG += "\n"; 

    //wRod
        if(fmod(device->angle, 360.0) < 180.0){ //quote on right
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella - device->wRod/2, 
                                            cxBiella, cyBiella + device->wRod/2, 
                                            device->wRod*7/10 + distQuote, lQuote, false);
        }
        else{ //quote on left
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella - device->wRod/2, 
                                            cxBiella, cyBiella + device->wRod/2, 
                                            device->wRod*7/10 + distQuote, lQuote, true);
        }
        deviceSVG += "\n"; 

    //hPiston
        if(fmod(device->angle, 360.0) < 180.0){ //quote on left
            deviceSVG += LBAMTTquoteDistSVG(cxPistone - device->dPiston/2, cyPistone + device->wRod*7/10 - device->hPiston, 
                                            cxPistone - device->dPiston/2, cyPistone + device->wRod*7/10, 
                                            distQuote, lQuote, true);
        }
        else{ //quote on right
            deviceSVG += LBAMTTquoteDistSVG(cxPistone + device->dPiston/2, cyPistone + device->wRod*7/10 - device->hPiston, 
                                            cxPistone + device->dPiston/2, cyPistone + device->wRod*7/10, 
                                            distQuote, lQuote, false);
        }
        deviceSVG += "\n"; 

    //dPiston
        deviceSVG += LBAMTTquoteDistSVG(cxPistone - device->dPiston/2, cyPistone - device->hPiston + device->wRod*7/10, 
                                        cxPistone + device->dPiston/2, cyPistone - device->hPiston + device->wRod*7/10, 
                                        distQuote, lQuote, false);
        deviceSVG += "\n"; 

    //stroke
        if(fmod(device->angle, 360.0) < 180.0){ //quote on left
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella, 
                                            cxShaft, cyShaft, 
                                            device->dShaft*7/10 + distQuote, lQuote, false);
        }
        else{ //quote on right
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella, 
                                            cxShaft, cyShaft, 
                                            device->dShaft*7/10 + distQuote, lQuote, true);
        }
        deviceSVG += "\n"; 

     //lRod
        distQuote = device->dPiston * 2/3 + lQuote;
        if(fmod(device->angle, 360.0) < 180.0){ //quote on left
            deviceSVG += LBAMTTquoteDistSVG(cxPistone, cyPistone, 
                                            cxBiella, cyBiella, 
                                            distQuote, lQuote, false);
        }
        else{ //quote on right
            deviceSVG += LBAMTTquoteDistSVG(cxBiella, cyBiella, 
                                            cxPistone, cyPistone, 
                                            distQuote, lQuote, false);
        }
        deviceSVG += "\n";   

    //angle
        distQuote = device->dShaft *6/10;
        deviceSVG += LBAMTTquoteAngleSVG(cxShaft, cyShaft, 
                                         q * 180 / PI, 90, 
                                         distQuote, lQuote);
        deviceSVG += "\n";
    }
    

    //add SVG header with drawing dimensions
    if(header){
        deviceSVG = LBAMTTheaderSVG(deviceSVG);
    }
    
    return deviceSVG;
}

vector<string> LBAMTTsplitString (string s, string delimiter){ //string to be splitted passed as copy

    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {//find delimeter -> if not found exit
        token = s.substr (pos_start, pos_end - pos_start);//extract substing between string start and delimiter position; ATT: modifies the string
        pos_start = pos_end + delim_len; //move the pointer to the new start string
        res.push_back (token); //save the extracted string into the vector
    }

    res.push_back (s.substr (pos_start)); //when delimiter is not found -> save the rimaining string
    return res;
}

int LBAMTTsaveToFile(string s, string fileName){
    if (fileName == "") return 1;

    //extension check
    vector<string> checkFileName = LBAMTTsplitString(fileName,".");
    if (checkFileName.front() == "") return 1;
    if (checkFileName.back() != "svg") return 1;

    ofstream fout(fileName);
    fout << s;
    fout.close();

    return 0;
}

string LBAMTTloadFromFile(string fileName){
    if (fileName == "") return "";

    //extension check
    vector<string> checkFileName = LBAMTTsplitString(fileName,".");
    if (checkFileName.front() == "") return "";
    if (checkFileName.back() != "svg") return "";

    ifstream fin(fileName);
    stringstream buffer;

    //check is the file has been opened (otherways the path doesn's exist)
    if(! fin.is_open()) return "";

    buffer << fin.rdbuf();
    string s = buffer.str();

    return s;
}

LBAMTTdevice * LBAMTTdeviceFromStringSVG(string s){

    //split to extract strings with the different objects
    vector<string> vTot = LBAMTTsplitString(s, ">\n\n<");
    
    //erase the strings that aren't circles or rectangles
    int i = 0;
    while(i < vTot.size()){
        if(vTot[i][0] != 'r' && vTot[i][0] != 'c') vTot.erase(vTot.begin() + i);
        else i++;
    }

    string control = "rrccrccc";
    //check number of figure
    if(vTot.size()%control.size() != 0 || vTot.size() < control.size()) return NULL;
    //check if the figure succession is correct
    for(i = 0; i < vTot.size(); i++) if(vTot[i][0] != control[i%control.size()]) return NULL;

    //parameters extraction
    double dShaft;
    double stroke; 
    double lRod;
    double wRod;
    double hPiston; 
    double dPiston; 
    double angle;

    vector<string> vTmp;
    //connecting rod
    vTmp = LBAMTTsplitString(vTot[0],"\"");
    lRod = atof(vTmp[5].c_str());
    wRod = atof(vTmp[7].c_str());

    //piston
    vTmp = LBAMTTsplitString(vTot[1],"\"");
    dPiston = atof(vTmp[5].c_str());
    hPiston = atof(vTmp[7].c_str());

    //crank
    vTmp = LBAMTTsplitString(vTot[4],"\"");
    stroke = 2 * atof(vTmp[5].c_str());
    vTmp = LBAMTTsplitString(vTmp[11],"(");
    vTmp = LBAMTTsplitString(vTmp[1],",");//gets rotate values
    angle = 90 - atof(vTmp[0].c_str());

    //shaft
    vTmp = LBAMTTsplitString(vTot[5],"\"");
    dShaft = 2 * atof(vTmp[5].c_str());

    return LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston, angle);
}

string LBAMTTanimateDeviceSVG(LBAMTTdevice * device, double cxShaft, double cyShaft, LBAMTTanimation * anim, bool header){
    if(device == NULL) return "";

    string deviceSVG = "";

    for(int i = 0; i < anim->n; i++){
        LBAMTTanimSetIndex(anim, i);
        device->angle += 360.0 / anim->n;
        device->angle = LBAMTTnormAng(device->angle, 360);
        //piston
        deviceSVG += LBAMTTdeviceToStringSVG(device, cxShaft, cyShaft, false, false, anim);
    }

    if(header){
        deviceSVG = LBAMTTheaderSVG(deviceSVG);
    }

    return deviceSVG;
}
