# Homework 1:  Incredible Machines
## Biella Manovella

### Definizione del device
Si intende creare un meccanismo biella manovella per la trasmissione di moto traslatorio in moto rotatorio o viceversa.

#### Parametri:
* _dShaft_: diametro dell'albero motore
* _stroke_: corsa del pistone
* _lenBiella_: lunghezza della biella
* _wBiella_: larghezza della biella
* _hPistone_: altezza del pistone
* _dPistone_: diametro del pistone
* _angle_: angolo in gradi della biella
* _verse_: orientazione pistone, può essere _N_, _E_, _S_, _O_ (Nord, Est, Sud, Ovest)

#### Definizione corpi e vincoli dei parametri:
* __ALBERO MOTORE__: organo di collegamento del moto rotatorio, definito dal suo diametro (_dShaft_).
* __MANOVELLA__: collega l'albero alla biella.
    - La corsa del pistone (_stroke_) è la distanza tra asse dell'albero e asse della coppia biella-manovella. 
    - La coppia biella-manovella ha diametro pari alla larghezza della biella.
    - La lunghezza totale dipende da: _stroke_, _dShaft_, _wBiella_. Sarà pari alla loro somma più dei termini aggiuntivi per l'accoppiamento con l'albero e con la biella.
    - Termine aggiuntivo per accoppiamento biella-manovella è pari a 1/5 della larghezza della biella.
    - Termine aggiuntivo per accoppiamento manovella-albero è pari a 1/5 del diametro dell'albero.

    VINCOLI:
    - La metà della corsa deve essere maggiore dei raggi delle due coppie.
* __BIELLA__: collega biella e pistone.
    - La lunghezza della biella (_lenBiella_) è la distanza tra gli assi degli accoppiamenti biella-manovella e biella-pistone.
    - La coppia manovella-pistone è interna al pistone, con un termine per l'accoppiamento biella-pistone pari a 1/5 della larghezza della biella.

    VINCOLI:
    - La lunghezza minima deve essere tale da evitare interferenza tra biella e pistone nella posizione di punto morto inferiore (_angle_ pari a 180°), quindi dovrà essere maggiore della distanza tra asse della coppia biella-manovella e di tutto il corpo di accoppiamento manovella-albero (considerando le condizioni di accoppiamento biella-pistone).
    - La larghezza della biella non può essere inferiore ad 1/6 della sua lunghezza
    - La larghezza della biella non può essere superiore al diametro dell'albero motore
* __PISTONE__: organo di collegamento per moto traslatorio.

    VINCOLI:
    - Diametro del pistone e altezza del pistone (rispettivamente _dPiston_ e _hPiston_) devono essere tali da rispettare il limiti per laccoppiamento biella pistone.
* __ANGOLO__: angolo della biella per la rappresentazione del meccanismo (_angle_) in gradi.
* __ORIENTAMENTO__: orientamento del meccanismo per la sua rappresentazione (_verse_).

NB: per termine di accoppiamento si intende una porzione di materiale aggiuntivo intorno alla coppia rotoidale in termini radiali

