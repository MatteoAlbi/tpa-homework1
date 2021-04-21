#ifndef BIELLA_MANOVELLA
#define BIELLA_MANOVELLA

#include <iostream>
#include <math.h>
#include <new>
#include <string.h>

using namespace std;

typedef const double cDbl;
enum LBAMTTorientation {N,E,S,O}; //variabile orientazione

typedef struct LBAMTTdevice{
    double dShaft; //diametro albero
    double stroke; //corsa pistone
    double lenBiella; //lunghezza biella
    double wBiella; //larghezza biella
    double hPistone; //altezza pistone
    double dPistone; //diametro pistone
    double angle; //angolo in gradi della biella
    LBAMTTorientation verse; //orientazione pistone, preimpostata a N
} LBAMTTdevice;

/**
 * ALLOCA dinamicamente una struttura LBAMTTdevice.
 * @param dShaft diametro albero
 * @param stroke corsa pistone
 * @param lenBiella lunghezza biella
 * @param wBiella larghezza biella
 * @param hPistone altezza pistone
 * @param dPistone diametro pistone
 * @param angle angolo in gradi della manovella, preimpostato a 0 (punto morto superiore)
 * @param verse orientazione pistone, preimpostata a N
 * Controlla che le misure rispettino le condizioni di integrità del device.
 * Se i valori non rispettano i vincoli di costruzione il puntatore viene DEALLOCATO.
 * @return puntatore a device
 * @return in caso di errore: NULL (errore nell'allocazione o nei parametri passati)
*/
LBAMTTdevice * LBAMTTinitDevice (cDbl dShaft, cDbl stroke, cDbl lenBiella, cDbl wBiella, cDbl hPistone, cDbl dPistone, cDbl angle = 0, LBAMTTorientation verse = N);

/**
 * Controlla che le misure date siano compatibili con l'integrità strutturale del device.
 * @param device puntatore a device di cui controllare i parametri
 * @return 0 se vengono rispettati i vincoli
 * @return 1 se i parametri sono <= 0
 * @return 2 se non viene rispettato il vincolo sulla manovella
 * @return 3 se non vengono rispettati i vincoli sul pistone
 * @return 4 se non viene rispettato il vincolo sulla lunghezza della biella
 * @return 5 se non viene rispettato il vincolo sulla larghezza della biella
*/
int LBAMTTcheckIntegrity (const LBAMTTdevice * device);

/**
 * DEALLOCA la struttura puntata dal puntatore passato
 * @param device puntatore a struttura da deallocare
 * @return 0 se il procedimento è avvenuto con successo
 * @return 1 in caso di errore 
*/
int LBAMTTdelete (LBAMTTdevice * device);

/**
 * Crea una stringa in codice SVG per la rappresentazione del device
 * @param device puntatore a struttura da rappresentare
 * @return string deviceSVG se il processo è avvenuto con successo
 * @return NULL in caso di errore 
*/
string LBAMTTtoStringSVG (LBAMTTdevice * device);

#endif