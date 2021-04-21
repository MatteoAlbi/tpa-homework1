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
 * Controlla che le misure rispettino le condizioni di integrità del device.
 * Se i valori non rispettano i vincoli di costruzione il puntatore viene DEALLOCATO.
 * @param dShaft diametro albero
 * @param stroke corsa pistone
 * @param lenBiella lunghezza biella
 * @param wBiella larghezza biella
 * @param hPistone altezza pistone
 * @param dPistone diametro pistone
 * @param angle angolo in gradi della manovella, preimpostato a 0 (punto morto superiore)
 * @param verse orientazione pistone, preimpostata a N
 * @return puntatore a device; 
 *      in caso di errore: NULL (errore nell'allocazione o nei parametri passati)
*/
LBAMTTdevice * LBAMTTinitDevice (cDbl dShaft, cDbl stroke, cDbl lenBiella, cDbl wBiella, cDbl hPistone, cDbl dPistone, cDbl angle = 0, LBAMTTorientation verse = N);

/**
 * Controlla che le misure date siano compatibili con l'integrità strutturale del device.
 * @param device puntatore a device di cui controllare i parametri
 * @return 0 se vengono rispettati i vincoli;
 *      1 se i parametri sono <= 0;
 *      2 se non viene rispettato il vincolo sulla manovella;
 *      3 se non vengono rispettati i vincoli sul pistone;
 *      4 se non viene rispettato il vincolo sulla lunghezza della biella;
 *      5 se non vengono rispettati i vincoli sulla larghezza della biella
*/
int LBAMTTcheckIntegrity (const LBAMTTdevice * device);

/**
 * modifica il parametro dShaft della struttura puntata dal puntatore passato
 * @param device puntatore a struttura da modificare
 * @param dShaft nuovo valore
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore, e ripristina il valore iniziale
*/
int LBAMTTsetDShaft (LBAMTTdevice * device, cDbl dShaft);

/**
 * modifica il parametro stroke della struttura puntata dal puntatore passato
 * @param device puntatore a struttura da modificare
 * @param stroke nuovo valore
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore, e ripristina il valore iniziale
*/
int LBAMTTsetStroke (LBAMTTdevice * device, cDbl stroke);

/**
 * modifica il parametro lenBiella della struttura puntata dal puntatore passato
 * @param device puntatore a struttura da modificare
 * @param lenBiella nuovo valore
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore, e ripristina il valore iniziale
*/
int LBAMTTsetLenBiella (LBAMTTdevice * device, cDbl lenBiella);

/**
 * modifica il parametro wBiella della struttura puntata dal puntatore passato
 * @param device puntatore a struttura da modificare
 * @param wBiella nuovo valore
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore, e ripristina il valore iniziale
*/
int LBAMTTsetWBiella (LBAMTTdevice * device, cDbl wBiella);

/**
 * modifica il parametro hPistone della struttura puntata dal puntatore passato
 * @param device puntatore a struttura da modificare
 * @param hPistone nuovo valore
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore, e ripristina il valore iniziale
*/
int LBAMTTsetHPistone (LBAMTTdevice * device, cDbl hPistone);

/**
 * modifica il parametro dPistone della struttura puntata dal puntatore passato
 * @param device puntatore a struttura da modificare
 * @param dPistone nuovo valore
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore, e ripristina il valore iniziale
*/
int LBAMTTsetDPistone (LBAMTTdevice * device, cDbl dPistone);

/**
 * modifica il parametro angle della struttura puntata dal puntatore passato
 * @param device puntatore a struttura da modificare
 * @param angle nuovo valore
*/
void LBAMTTsetAngle (LBAMTTdevice * device, cDbl angle);

/**
 * modifica il parametro verse della struttura puntata dal puntatore passato
 * @param device puntatore a struttura da modificare
 * @param verse nuovo valore
*/
void LBAMTTsetVerse (LBAMTTdevice * device, LBAMTTorientation verse);

/**
 * DEALLOCA la struttura puntata dal puntatore passato
 * @param device puntatore a struttura da deallocare
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore
*/
int LBAMTTdelete (LBAMTTdevice * device);

/**
 * Crea una stringa in codice SVG per la rappresentazione del device
 * @param device puntatore a struttura da rappresentare
 * @return string deviceSVG
*/
string LBAMTTtoStringSVG (LBAMTTdevice * device);

#endif