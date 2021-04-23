#ifndef BIELLA_MANOVELLA
#define BIELLA_MANOVELLA

#include <iostream>     //output terminale
#include <string>       //utilizzo string al posto di array di char
#include <math.h>       //calcoli per creare disegno
#include <new>          //eccezione su new in caso di memoria piena
#include <vector>       //vettore lunghezza variabile

#include <fstream>      //file
#include <streambuf>    //file
#include <sstream>      //file

#define PI 3.14159265
using namespace std;

typedef const double cDbl;

typedef struct LBAMTTdevice{
    double dShaft; //diametro albero
    double stroke; //corsa pistone
    double lenBiella; //lunghezza biella
    double wBiella; //larghezza biella
    double hPistone; //altezza pistone
    double dPistone; //diametro pistone
    double angle; //angolo in gradi della biella
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
 * @param angle angolo in gradi della manovella (default 0.0, punto morto superiore)
 * @return puntatore a device; 
 *      in caso di errore: NULL (errore nell'allocazione o nei parametri passati)
*/
LBAMTTdevice * LBAMTTinitDevice (cDbl dShaft, cDbl stroke, cDbl lenBiella, cDbl wBiella, cDbl hPistone, cDbl dPistone, cDbl angle = 0.0);

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
 * DEALLOCA la struttura puntata dal puntatore passato
 * @param device puntatore a struttura da deallocare
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore
*/
int LBAMTTdelete (LBAMTTdevice * device);

/**
 * Crea una stringa per la rappresentazione di un rettangolo in formato SVG
 * @param x posizione x dell'angolo in basso a destra
 * @param y posizione y dell'angolo in basso a destra
 * @param w larghezza rettangolo
 * @param h altezza rettangolo
 * @param color colore del rettangolo (deve essere compatibile col formato SVG)
 * @param rotation angolo del quale il rettangolo viene ruotato (default 0.0)
 * @param xr coordinata x del punto attorno al quale si ruota (default 0.0)
 * @param yr coordinata y del punto attorno al quale si ruota (default 0.0)
 * @return stringa SVG del rettangolo
*/
string LBAMTTrectStringSVG(double x, double y, double w, double h, string color, double rotation = 0.0, double xr = 0.0, double yr = 0.0);

/**
 * Crea una stringa per la rappresentazione di un cerchio in formato SVG
 * @param x posizione x del centro del cerchio
 * @param y posizione y del centro del cerchio
 * @param r raggio cerchio
 * @param color colore del cerchio (deve essere compatibile col formato SVG)
 * @return stringa SVG del cerchio
*/
string LBAMTTcircleStringSVG(double x, double y, double r, string color);

/**
 * Crea una stringa per la rappresentazione di una linea in formato SVG
 * @param x1 posizione x del primo punto
 * @param y1 posizione y del primo punto
 * @param x2 posizione x del secondo punto
 * @param y2 posizione y del secondo punto
 * @param stroke spessore della linea (default 2)
 * @param color colore della linea (deve essere compatibile col formato SVG) (default nero)
 * @return stringa SVG del cerchio 
*/
string LBAMTTlineStringSVG(double x1, double y1, double x2, double y2, int stroke = 2, string color = "black");

/**
 * Crea una quota di distanza tra i punti A e B
 * @param xA coordinata x di A
 * @param yA coordinata y di A
 * @param xB coordinata x di B
 * @param yB coordinata y di B
 * @param distQuote distanza della quota da AB
 * @param lQuote lunghezza delle linee laterali della quota
 * @param stroke spessore linee di quota
 * @param side flag: se vero, la quota esce in direzione theta+90, con theta=arg(AB)
 * @return stringa SVG della quota
*/
string LBAMTTquoteDistString(double xA, double yA, double xB, double yB, double distQuote, double lQuote, double stroke = 2, bool side = true);

/**
 * Crea una stringa in codice SVG per la rappresentazione del device
 * @param device puntatore a struttura da rappresentare
 * @param cxShaft coordinata x del centro dell'albero nell'area di disegno
 * @param cyShaft coordinata y del centro dell'albero nell'area di disegno
 * @param quote flag: se vero include le quote delle misure del pezzo (default false)
 * @param header flag: se vero include l'header per il file svg (default true)
 * @return string deviceSVG
*/
string LBAMTTdeviceToStringSVG (LBAMTTdevice * device, double cxShaft, double cyShaft, bool quote = false, bool header = true);

/**
 * Splitta una stringa secondo una sottostringa passata
 * @param s stringa da splittare
 * @param delimeter sottostringa secondo la quale splittare
 * @return stringa splittata inserita in un vector
*/
vector<string> LBAMTTsplitString (string s, string delimiter);

/**
 * Salva su file la stringa passata
 * @param stringSVG stringa da salvare
 * @param fileName nome del file sul quale salvare la stringa, estensione deve essere .svg
 * @return 0 se il procedimento è avvenuto con successo;
 *      1 in caso di errore
*/
int LBAMTTsaveToFile(string stringSVG, string fileName);

#endif