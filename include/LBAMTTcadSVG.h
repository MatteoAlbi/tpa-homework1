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
 * Crea una stringa che definisce un marker a forma di freccia, necessario per le quote
 * @return stringa che definisce il marker
 */
string LBAMTTarrowMarkerSVG();

/**
 * Crea una stringa per la rappresentazione di un rettangolo in formato SVG
 * @param x coordinata x dell'angolo in basso a destra
 * @param y coordinata y dell'angolo in basso a destra
 * @param w larghezza rettangolo
 * @param h altezza rettangolo
 * @param color colore del rettangolo (deve essere compatibile col formato SVG)
 * @param rotation angolo del quale il rettangolo viene ruotato (default 0.0)
 * @param xr coordinata x del punto attorno al quale si ruota (default 0.0)
 * @param yr coordinata y del punto attorno al quale si ruota (default 0.0)
 * @return stringa SVG del rettangolo
 */
string LBAMTTrectSVG(cDbl x, cDbl y, cDbl w, cDbl h, string color, cDbl rotation = 0.0, cDbl xr = 0.0, cDbl yr = 0.0);

/**
 * Crea una stringa per la rappresentazione di un cerchio in formato SVG
 * @param x coordinata x del centro del cerchio
 * @param y coordinata y del centro del cerchio
 * @param r raggio cerchio
 * @param color colore del cerchio (deve essere compatibile col formato SVG)
 * @return stringa SVG del cerchio
 */
string LBAMTTcircleSVG(cDbl x, cDbl y, cDbl r, string color);

/**
 * Crea una stringa per la rappresentazione di una linea in formato SVG
 * @param x1 coordinata x del primo punto
 * @param y1 coordinata y del primo punto
 * @param x2 coordinata x del secondo punto
 * @param y2 coordinata y del secondo punto
 * @param stroke spessore della linea (default 2)
 * @param color colore della linea (deve essere compatibile col formato SVG) (default nero)
 * @param opt opzioni aggiuntive di visualizzazione, non vi è controllo sulla formattazione (default vuoto) 
 * @return stringa SVG della linea
 */
string LBAMTTlineSVG(cDbl x1, cDbl y1, cDbl x2, cDbl y2, int stroke = 2, string color = "black", string opt = "");

/**
 * Crea una stringa per la rappresentazione di un testo in formato SVG
 * @param s stringa da inserire
 * @param x coordinata x del testo
 * @param y coordinata y del testo
 * @param rotation angolo del quale il rettangolo viene ruotato (default 0.0)
 * @param xr coordinata x del punto attorno al quale si ruota (default 0.0)
 * @param yr coordinata y del punto attorno al quale si ruota (default 0.0)
 * @param color colore del testo (deve essere compatibile col formato SVG) (default nero)
 * @param anchor posizione relativa tra testo e punto dato (start, middle, end), non vi è controllo sulla formattazione (default middle) 
 * @param opt opzioni aggiuntive di visualizzazione, non vi è controllo sulla formattazione (default vuoto)
 * @return stringa SVG del testo
 */
string LBAMTTtextSVG(string s, cDbl x, cDbl y, cDbl rotation = 0.0, cDbl xr = 0.0, cDbl yr = 0.0, string color = "black", string anchor = "middle", string opt = "");

/**
 * Crea una quota di distanza tra i punti A e B.
 * NECESSARIO creare un marker freccia con la funzione LBAMTTarrowMarkerSVG a inizio file.
 * @param xA coordinata x di A
 * @param yA coordinata y di A
 * @param xB coordinata x di B
 * @param yB coordinata y di B
 * @param distQuote distanza della quota da AB
 * @param lQuote lunghezza delle linee laterali della quota
 * @param side flag: se vero, la quota esce in direzione theta+90, con theta=arg(AB)
 * @return stringa SVG della quota
 */
string LBAMTTquoteDistSVG(cDbl xA, cDbl yA, cDbl xB, cDbl yB, cDbl distQuote, cDbl lQuote, bool side = true);

#endif