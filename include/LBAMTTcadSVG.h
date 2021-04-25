#ifndef LBAMTT_SVG
#define LBAMTT_SVG

#include <string>       //utilizzo string al posto di array di char
#include <math.h>       //calcoli per creare disegno

#ifndef PI
#define PI 3.14159265
#endif

#ifndef CONST_DOUBLE
#define CONST_DOUBLE
typedef const double cDbl;
#endif

using namespace std;

/**
 * Aggiunge l'header per i file SVG
 * @param s stringa a cui aggiungere header SVG
 * @return stringa passata con header aggiunto
 */
string LBAMTTheaderSVG(string s);

/**
 * Crea una stringa che definisce un marker a forma di freccia, necessario per le quote
 * @return stringa che definisce il marker
 */
string LBAMTTarrowMarkerSVG();

/**
 * Crea una stringa per la rappresentazione di un rettangolo in formato SVG
 * @param x coordinata x dell'angolo in basso a destra
 * @param y coordinata y dell'angolo in basso a destra
 * @param w larghezza rettangolo (deve essere maggiore di 0)
 * @param h altezza rettangolo (deve essere maggiore di 0)
 * @param color colore del rettangolo (deve essere compatibile col formato SVG)
 * @param rotation angolo del quale il rettangolo viene ruotato (default 0.0)
 * @param xr coordinata x del punto di rotazione (default 0.0)
 * @param yr coordinata y del punto di rotazione (default 0.0)
 * @return stringa SVG del rettangolo;
 *      vuoto in caso di errore
 */
string LBAMTTrectSVG(cDbl x, cDbl y, cDbl w, cDbl h, string color, double rotation = 0.0, cDbl xr = 0.0, cDbl yr = 0.0);

/**
 * Crea una stringa per la rappresentazione di un cerchio in formato SVG
 * @param x coordinata x del centro del cerchio
 * @param y coordinata y del centro del cerchio
 * @param r raggio cerchio (deve essere maggiore di 0)
 * @param color colore del cerchio (deve essere compatibile col formato SVG)
 * @return stringa SVG del cerchio;
 *      vuoto in caso di errore
 */
string LBAMTTcircleSVG(cDbl x, cDbl y, cDbl r, string color);

/**
 * Crea una stringa per la rappresentazione di una linea in formato SVG, 
 * i due punti che definiscono la linea devono essere diversi
 * @param x1 coordinata x del primo punto
 * @param y1 coordinata y del primo punto
 * @param x2 coordinata x del secondo punto
 * @param y2 coordinata y del secondo punto
 * @param stroke spessore della linea (default 2) (deve essere maggiore di 0)
 * @param color colore della linea (deve essere compatibile col formato SVG) (default nero)
 * @param opt opzioni aggiuntive di visualizzazione, non vi è controllo sulla formattazione (default vuoto) 
 * @return stringa SVG della linea;
 *      vuoto in caso di errore
 */
string LBAMTTlineSVG(cDbl x1, cDbl y1, cDbl x2, cDbl y2, int stroke = 2, string color = "black", string opt = "");

/**
 * Crea una stringa per la rappresentazione di un arco in formato SVG, 
 * deve avere un angolo maggiore di 0
 * @param cx coordinata x del centro dell'arco
 * @param cy coordinata y del centro dell'arco
 * @param r raggio dell'arco (deve essere maggiore di 1)
 * @param startAngle angolo dal quale parte l'arco in gradi
 * @param endAngle angolo dove finisce l'arco in gradi
 * @param stroke spessore arco (deve essere maggiore di 0 e minore di r)
 * @param color colore arco
 * @return stringa SVG dell'arco;
 *      vuoto in caso di errore
 */
string LBAMTTarcSVG(cDbl cx, cDbl cy, cDbl r, double startAngle, double endAngle, int stroke = 2, string color = "black");

/**
 * Crea una stringa per la rappresentazione di un testo in formato SVG
 * @param s stringa da inserire
 * @param x coordinata x del testo
 * @param y coordinata y del testo
 * @param rotation angolo del quale il testo viene ruotato (default 0.0)
 * @param xr coordinata x del punto di rotazione (default 0.0)
 * @param yr coordinata y del punto di rotazione (default 0.0)
 * @param color colore del testo (deve essere compatibile col formato SVG) (default nero)
 * @param anchor posizione relativa tra testo e punto dato (start, middle, end), non vi è controllo sulla formattazione (default middle) 
 * @param opt opzioni aggiuntive di visualizzazione, non vi è controllo sulla formattazione (default vuoto)
 * @return stringa SVG del testo
 */
string LBAMTTtextSVG(string s, cDbl x, cDbl y, double rotation = 0.0, cDbl xr = 0.0, cDbl yr = 0.0, string color = "black", string anchor = "middle", string opt = "");

/**
 * Crea una quota di distanza tra i punti A e B, 
 * i due punti che definiscono la quota devono essere diversi. 
 * NECESSARIO creare un marker freccia con la funzione LBAMTTarrowMarkerSVG a inizio file.
 * @param xA coordinata x di A
 * @param yA coordinata y di A
 * @param xB coordinata x di B
 * @param yB coordinata y di B
 * @param distQuote distanza della quota da AB (deve essere maggiore di lQuote)
 * @param lQuote lunghezza delle linee laterali della quota (deve essere maggiore uguale a 1)
 * @param side flag: se vero, la quota esce in direzione theta+90, con theta=arg(AB), altrimenti sull'altro lato
 * @return stringa SVG della quota;
 *      vuoto in caso di errore
 */
string LBAMTTquoteDistSVG(cDbl xA, cDbl yA, cDbl xB, cDbl yB, cDbl distQuote, cDbl lQuote, bool side = true);

#endif