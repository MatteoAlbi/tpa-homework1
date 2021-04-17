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
    else if (device->stroke/2  <= device->dShaft/2 + device->wBiella/2) return 2; //vincolo lunghezza MANOVELLA
    else if (device->hPistone <= device->wBiella*7/5 || device->dPistone <= device->wBiella*7/5) return 3; //vincoli PISTONE
    else if (device->lenBiella <= device->stroke/2 + device->dShaft*7/10) return 4; //vincolo lunghezza BIELLA
    else if (device->wBiella <= device->lenBiella*1/6) return 5; //vincolo larghezza BIELLA

    else return 0;
};

int LBAMTTdelete (LBAMTTdevice * device){
    if(device == NULL) return 1;
    else {
        delete device;
        return 0;
    }
}