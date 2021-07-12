#include "LBAMTTmotor.h"

bool LBAMTTdblCompare(cDbl a, cDbl b, cDbl precision){
    if(abs(a-b) < precision) return true;
    else return false;
}

int LBAMTTsetOffsets(LBAMTTmotor * motor){
    if(motor == NULL) return 1;
    if(motor->n > 4 || motor->n < 1) return 1;

    motor->offset = new double[motor->n];
    motor->offset[0] = 0.0;

    switch (motor->n)
    {
    case 1: //monocylinder
        break;
    case 2: //two cylinders 
        motor->offset[1] = 360.0;
        break;
    case 3: //three cylinders
        motor->offset[1] = 240.0;
        motor->offset[2] = 480.0;
        break;
    case 4: //four cylinders
        motor->offset[1] = 180.0;
        motor->offset[2] = 540.0;
        motor->offset[3] = 360.0;
        break;
    }

    return 0;
}

LBAMTTcylinder * LBAMTTinitCylinder(cDbl bore, cDbl stroke, cDbl angle){
    LBAMTTcylinder * ret = new LBAMTTcylinder;

    //All values respect constraints
    double dShaft = stroke/2;
    double lRod = stroke*1.1;
    double wRod = lRod/4.5;
    double hPiston = wRod*3;

    double rMax = stroke/6;
    double rMin = rMax*5/7;
    double diamValve = bore*2/5;
    double lenValve = stroke/3;
    double Gamma = PI /8;

    double pistonAngle = LBAMTTnormAng(angle, 720);
    /**
     * pistonAngle = 0 -> start compression: both valve closed
     * pistonAngle = 180 -> explosion: both valve closed
     * pistonAngle = 360 -> start expelling: valve Dx open, valve Sx closed
     * pistonAngle = 540 -> start aspiration: valve Sx open, valve Dx closed
    */
    double angleValveSx = LBAMTTnormAng(PI*3/4 + (pistonAngle * PI/180 / 2), 2*PI);
    double angleValveDx = LBAMTTnormAng(angleValveSx + PI/2, 2*PI);

    ret->piston = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, bore, pistonAngle);
    ret->valveSx = ENRICinitDevice(rMin, rMax, lenValve, diamValve, angleValveSx, Gamma);
    ret->valveDx = ENRICinitDevice(rMin, rMax, lenValve, diamValve, angleValveDx, Gamma);

    return ret;
}

LBAMTTmotor * LBAMTTinitMotor(const int n, cDbl bore, cDbl displacement, cDbl angle){
    LBAMTTmotor * ret = new LBAMTTmotor;
    
    ret->n = n;
    if(LBAMTTsetOffsets(ret) == 1){//n violate constraints
        delete ret;
        return NULL;
    }

    double stroke = displacement / (pow(bore/2, 2) * PI * n);

    //checks
    bool error = false;
    double ratio = bore/stroke;
    if(stroke > 160) error = true;
    else if(bore < 60) error = true;
    else if(ratio < 0.7 || ratio > 2.4) error = true;
    else switch (ret->n){
        case 1:
            if(bore > 384) error = true;
            break;
        case 2: 
            if(bore > 384) error = true;
            break;
        case 3: 
            if(bore > 200) error = true;
            break;
        case 4: 
            if(bore > 150) error = true;
            break;
    }

    if(error){
        delete ret->offset;
        delete ret;
        return NULL;
    }

    // if(angle < 0) ret->angle = 720 - fmod(angle, 720);
    // else ret->angle =  fmod(angle, 720);
    ret->angle = LBAMTTnormAng(angle, 720);
    
    ret->cylinders = new LBAMTTcylinder* [n];
    for(int i=0; i<ret->n; i++){
        ret->cylinders[i] = LBAMTTinitCylinder(bore, stroke, ret->angle + ret->offset[i]);
    }

    return ret;
}

int LBAMTTdelete(LBAMTTcylinder * cylinder){
    if(cylinder == NULL) return 1;

    if(LBAMTTdelete(cylinder->piston) == 1) return 1;
    if(ENRICdelete(cylinder->valveDx) == 1) return 1;
    if(ENRICdelete(cylinder->valveSx) == 1) return 1;
    delete cylinder;

    return 0;
}

int LBAMTTdelete(LBAMTTmotor * motor){
    if(motor == NULL) return 1;

    for(int i=0;i<motor->n;i++){
        if(LBAMTTdelete(motor->cylinders[i]) == 1) return 1;
    }

    delete[] motor->cylinders;
    delete motor->offset;
    delete motor;

    return 0;
} 

int LBAMTTrotateMotor(LBAMTTmotor * motor){
    if (motor == NULL) return 1;

    for(int i=0; i<motor->n; i++){
        double cylinderAngle = LBAMTTnormAng(motor->angle + motor->offset[i], 720);
        motor->cylinders[i]->piston->angle = cylinderAngle;

        double angleValveSx = LBAMTTnormAng(PI*3/4 + (cylinderAngle * PI/180 / 2), 2*PI);
        //if(angleValveSx >= 2*PI) angleValveSx -= 2*PI;
        double angleValveDx = LBAMTTnormAng(angleValveSx + PI/2, 2*PI);
        //if(angleValveDx >= 2*PI) angleValveSx -= 2*PI;

        motor->cylinders[i]->valveSx->Alpha = angleValveSx;
        motor->cylinders[i]->valveDx->Alpha = angleValveDx;
    }
    return 0;
}

int LBAMTTsetMotorN(LBAMTTmotor * motor, const int n){
    if(motor == NULL) return 1;

    //checks
    if(n == motor->n) return 0;
    if(n < 1 || n > 4) return 1;

    //set
    double stroke = motor->cylinders[0]->piston->stroke;
    double bore = motor->cylinders[0]->piston->dPiston;

    for(int i=0;i<motor->n;i++){
        delete motor->cylinders[i]->piston;
        delete motor->cylinders[i]->valveDx;
        delete motor->cylinders[i]->valveSx;
        delete motor->cylinders[i];
    }
    delete[] motor->cylinders;
    delete motor->offset;

    motor->n = n;
    LBAMTTsetOffsets(motor);
    motor->cylinders = new LBAMTTcylinder* [motor->n];  
    for(int i=0; i<motor->n; i++){
        motor->cylinders[i] = LBAMTTinitCylinder(bore, stroke, motor->angle + motor->offset[i]);
    }  

    return 0;
}

int LBAMTTsetMotorBore(LBAMTTmotor * motor, cDbl bore){
    if(motor == NULL) return 1;
    
    //checks
    if(bore == motor->cylinders[0]->piston->dPiston) return 0; //no need to modify

    if(bore < 60) return 1;
    double ratio = bore / motor->cylinders[0]->piston->stroke;
    if(ratio < 0.7 || ratio > 2.4) return 1;
    switch (motor->n){
        case 1:
            if(bore > 384) return 1;
            break;
        case 2: 
            if(bore > 384) return 1;
            break;
        case 3: 
            if(bore > 200) return 1;
            break;
        case 4: 
            if(bore > 150) return 1;
            break;
    }

    //set
    for(int i=0;i<motor->n;i++){
        motor->cylinders[i]->piston->dPiston = bore;
        motor->cylinders[i]->piston->hPiston = bore/1.5;
        motor->cylinders[i]->valveDx->diamValve = bore*2/5;
        motor->cylinders[i]->valveSx->diamValve = bore*2/5;
    }

    return 0;
}

int LBAMTTsetMotorDisplacement(LBAMTTmotor * motor, cDbl displacement){
    if(motor == NULL) return 1;

    //checks
    double bore = motor->cylinders[0]->piston->dPiston;
    double oldStroke = motor->cylinders[0]->piston->stroke;
    double stroke = displacement / (pow(bore/2, 2) * PI) / motor->n;
    if(LBAMTTdblCompare(stroke, oldStroke)) return 0; //no need to modify

    if(stroke > 160) return 1;
    double ratio = bore/stroke;
    if(ratio < 0.7 || ratio > 2.4) return 1;

    //set
    for(int i=0;i<motor->n;i++){
        motor->cylinders[i]->piston->stroke = stroke;
        motor->cylinders[i]->piston->dShaft = stroke/2;
        motor->cylinders[i]->piston->lRod = stroke*1.1;
        motor->cylinders[i]->piston->wRod = motor->cylinders[i]->piston->lRod/4.5;

        motor->cylinders[i]->valveDx->lenValve = stroke/3;
        motor->cylinders[i]->valveDx->rMax = stroke/6;
        motor->cylinders[i]->valveDx->rMin = motor->cylinders[i]->valveDx->rMax*5/7;

        motor->cylinders[i]->valveSx->lenValve = stroke/3;
        motor->cylinders[i]->valveSx->rMax = stroke/6;
        motor->cylinders[i]->valveSx->rMin = motor->cylinders[i]->valveSx->rMax*5/7;
    }

    return 0;
}

int LBAMTTsetMotorAngle(LBAMTTmotor* motor, cDbl angle){
    if(motor == NULL) return 1;

    double newAngle = LBAMTTnormAng(angle,720);
    // if(angle < 0) newAngle = 720 - fmod(angle,720);
    // else newAngle = fmod(angle,720);

    //checks
    if(motor->angle == newAngle) return 0; //no need to modify

    //set
    motor->angle = newAngle;
    LBAMTTrotateMotor(motor);

    return 0;
}

bool LBAMTTmotorCompare(LBAMTTmotor * a, LBAMTTmotor * b){
    if(a == b) return true;
    if(a == NULL || b == NULL) return false;
    if(a->n != b->n) return false;
    if(a->cylinders[0]->piston->dPiston != b->cylinders[0]->piston->dPiston) return false;
    if(! LBAMTTdblCompare(a->cylinders[0]->piston->stroke, b->cylinders[0]->piston->stroke)) return false;
    return true;
}

string LBAMTTcylinderToStringSVG (LBAMTTcylinder * cylinder, double cxShaft, double cyShaft, bool quote, bool header){

    if(cylinder == NULL) return "";

    double maxPistonY = cyShaft - cylinder->piston->stroke/2 - cylinder->piston->lRod - cylinder->piston->hPiston + cylinder->piston->wRod*7/10;
    double additionalY = 5; //additional height for the combustion chamber to not touch the valves
    double valveSpace = cylinder->valveDx->rMax - cylinder->valveDx->rMin; //valve Y movement
    double lenValve = cylinder->valveSx->lenValve;
    double cyValve =  maxPistonY - additionalY - cylinder->valveSx->rMax - lenValve * 1.1;
    //piston height
    double L1 = cylinder->piston->stroke/2; //crank lenght
    double L2 = cylinder->piston->lRod;
    double q = PI/2 - cylinder->piston->angle * PI / 180.0; //crank angle in radiants
    double cyPistone = cyShaft - sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) + L1 * sin(q);

    string cylinderSVG = "";

    //combustion chamber
    //horizontal line
    cylinderSVG +=  LBAMTTlineSVG(  cxShaft - cylinder->piston->dPiston/2 -2, 
                                    maxPistonY - additionalY - valveSpace - lenValve/10 -1,
                                    cxShaft + cylinder->piston->dPiston/2 +2, 
                                    maxPistonY - additionalY - valveSpace - lenValve/10 -1);
    cylinderSVG += "\n";
    //vertical lines
    cylinderSVG +=  LBAMTTlineSVG(  cxShaft - cylinder->piston->dPiston/2 -1, 
                                    maxPistonY - additionalY - valveSpace - lenValve/10 -2,
                                    cxShaft - cylinder->piston->dPiston/2 -1, 
                                    maxPistonY + cylinder->piston->stroke + additionalY);
    cylinderSVG += "\n";
    cylinderSVG +=  LBAMTTlineSVG(  cxShaft + cylinder->piston->dPiston/2 +1, 
                                    maxPistonY - additionalY - valveSpace - lenValve/10 -2,
                                    cxShaft + cylinder->piston->dPiston/2 +1, 
                                    maxPistonY + cylinder->piston->stroke + additionalY);
    cylinderSVG += "\n";

    //piston
    cylinderSVG += LBAMTTdeviceToStringSVG(cylinder->piston, cxShaft, cyShaft, false, false);

    //fill combustion chamber
    /**
     * pistonAngle = 0 -> start compression: both valve closed
     * pistonAngle = 180 -> explosion: both valve closed
     * pistonAngle = 360 -> start expelling: valve Dx open, valve Sx closed
     * pistonAngle = 540 -> start aspiration: valve Sx open, valve Dx closed
    */
    double angle = cylinder->piston->angle;
    double r,g,b;
    double brightness = 225;
    if(angle < 180){ //compression -> yellow--red
        b = 0;
        g = brightness * (180 - angle) / 180;
        r = brightness;
    }
    else if(angle >= 180 && angle < 360){//expanding -> red--blue
        g = 0;
        r = brightness * (360 - angle) / 180;
        b = brightness * (angle - 180) / 180;
    }
    else if(angle >= 360 && angle < 540){ //expelling -> blue--green
        r = 0;
        b = brightness * (540 - angle) / 180;
        g = brightness * (angle - 360) / 180;
    }
    else {//aspiration -> green--yellow
        b = 0;
        r = brightness * (angle - 540) / 180;
        g = brightness;
    }
    string color = "rgb(" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ")";
    double rectBottom = maxPistonY - additionalY - valveSpace - lenValve/10;
    double rectTop = cyPistone + cylinder->piston->wRod*7/10 - cylinder->piston->hPiston;
    cylinderSVG += LBAMTTrectSVG(   cxShaft - cylinder->piston->dPiston/2, rectBottom,
                                    cylinder->piston->dPiston, rectTop - rectBottom,
                                    color);
    cylinderSVG += "\n";

    //valves
    //valveSx
    double cxValveSx = cxShaft - cylinder->piston->dPiston/4; 
    cylinderSVG += ENRICtoStringSVG(cylinder->valveSx, cxValveSx, cyValve, false, false);
    cylinderSVG += "\n";
    //valveDx
    double cxValveDx = cxShaft + cylinder->piston->dPiston/4; 
    cylinderSVG += ENRICtoStringSVG(cylinder->valveDx, cxValveDx, cyValve, false, false);

    if(quote){
        double lQuote = 10;
        cylinderSVG = LBAMTTarrowMarkerSVG() + cylinderSVG; //marker arrow def
        //bore
        cylinderSVG += LBAMTTquoteDistSVG(  cxShaft - cylinder->piston->dPiston/2, maxPistonY - additionalY - valveSpace - lenValve/10,
                                            cxShaft + cylinder->piston->dPiston/2, maxPistonY - additionalY - valveSpace - lenValve/10,
                                            cylinder->valveSx->rMax*2 + lenValve * 1.1, lQuote, false);
        //stroke
        cylinderSVG += LBAMTTquoteDistSVG(  cxShaft - cylinder->piston->dPiston/2, maxPistonY,
                                            cxShaft - cylinder->piston->dPiston/2, maxPistonY + cylinder->piston->stroke,
                                            lQuote, lQuote, true);
    }

    cylinderSVG += "\n";

    if(header){
        cylinderSVG = LBAMTTheaderSVG(cylinderSVG);
    }

    return cylinderSVG;
}

string LBAMTTmotorToStringSVG(LBAMTTmotor * motor, bool quote, bool header){

    if(motor == NULL) return "";

    double maxY = 480; // maxY for the cylinder to fit in the draw
    double wRod = motor->cylinders[0]->piston->wRod;
    double stroke = motor->cylinders[0]->piston->stroke;
    double distance = wRod*8/5 + stroke;
    double lQuote = 10;
    
    string motorSVG = "";

    for(int i=0;i<motor->n;i++){
        double cxShaft0 = 400 - distance/2 * (motor->n-1);
        if(quote && i==0) motorSVG += LBAMTTcylinderToStringSVG(motor->cylinders[i], cxShaft0 + distance*i, maxY, true, false);
        else motorSVG += LBAMTTcylinderToStringSVG(motor->cylinders[i], cxShaft0 + distance*i, maxY, false, false);
        //motorSVG += "\n";
    }

    if(quote){
        //cout << "quote motor" << endl;
        double displacement = PI * pow(motor->cylinders[0]->piston->dPiston/2, 2) * motor->cylinders[0]->piston->stroke * motor->n;
        string s = "DISPLACEMENT: " + to_string(displacement);
        s.erase(s.end()-4, s.end());
        motorSVG += LBAMTTtextSVG(s, 30, 20, 0.0, 0.0, 0.0, "black", "start");
        motorSVG += LBAMTTtextSVG("N: " + to_string(motor->n), 30, 40, 0.0, 0.0, 0.0, "black", "start");
        motorSVG += LBAMTTquoteAngleSVG(400 - distance/2 * (motor->n-1), maxY, 
                                        90 - motor->angle, 90,
                                        stroke*5/16, lQuote);
        motorSVG += "\n";
    }

    if(header){ //add SVG header with drawing dimensions
        motorSVG = LBAMTTheaderSVG(motorSVG);
    }

    return motorSVG;
}

LBAMTTmotor * LBAMTTmotorFromStringSVG(string s){
    vector<string> vTot = LBAMTTsplitString(s, ">\n\n<");

    //erase the strings that aren't circles, rectangles, paths or lines
    int i = 0;
    while(i < vTot.size()){
        if(vTot[i][0] != 'r' && vTot[i][0] != 'c' && vTot[i][0] != 'p' && vTot[i][0] != 'l') vTot.erase(vTot.begin() + i);
        else i++;
    }

    string control = "lllrrccrcccrpp";
    //count number of cylinders
    string firstColumn = "";
    for(i = 0; i < vTot.size(); i++) firstColumn += vTot[i][0]; //string with the first character of each row
    bool found = true;
    int position = -1;
    int n = 0;
    while(found){
        position = firstColumn.find(control, position+1);
        if(position < 0) found = false;
        else n++;
    }

    //check number of figure
    if((vTot.size() - 3*n) % (control.size() - 3) != 0 || vTot.size() < control.size()) return NULL;

    //check if the figure succession is correct
    //for(i = 0; i < vTot.size(); i++) if(vTot[i][0] != control[i%control.size()]) return NULL;

    //param extraction

    double bore, stroke, angle;

    vector<string> vTmp;
    vTmp = LBAMTTsplitString(vTot[4],"\"");
    bore = atof(vTmp[5].c_str());
    vTmp = LBAMTTsplitString(vTot[7],"\"");
    stroke = 2 * atof(vTmp[5].c_str());
    vTmp = LBAMTTsplitString(vTmp[11],"(");
    vTmp = LBAMTTsplitString(vTmp[1],",");//gets rotate values
    angle = 90 - atof(vTmp[0].c_str());

    double displacement = PI * pow(bore/2,2) * stroke * n;

    LBAMTTmotor * ret = LBAMTTinitMotor(n, bore, displacement, angle);

    return ret;
}

LBAMTTcmdlineRet * LBAMTTcommandLineParam(int argc, char** argv){
    LBAMTTcmdlineRet * ret = new LBAMTTcmdlineRet;
    ret->device = NULL;
    ret->motor = NULL;

    string sargv[argc];
    for(int i=0;i<argc;i++) sargv[i] = string(argv[i]);
    // ./mainentry -device/motor -i /path_da_cui_importare -eq cx cy /path_dove_esportare -p <param>
    if(argc == 1) return NULL; //no params

    if(sargv[1] == "-h"){//help
        string help =   "--HELP\n"
                        "Command format: ./mainentry -\"struct\" -i importPath -e/-eq/-ea (n T) cxShaft cyShaft exportPath -p params...\n"
                        "-\"struct\" must be device or motor: define wich one is used"
                        "-i import a struct from the file with path importPath\n"
                        "-e export a struct (-eq export with quotes, -ea export animated) on the file with path exportPath.\n"
                        "   The struct is taken from:\n"
                        "       an imported file called with the option -i (prioritized action)\n"
                        "       the one crated with the params passed after the option -p (ignoerd if -i is called)\n"
                        "   cxShaft cyShaft are the coordinates of the shaft's center on the SVG draw, needed only with device\n"
                        "-eq export a struct with quotes on the file with path exportPath (options as before)\n"
                        "-ea export a struct animated on the file with path exportPath. Must specify two more options\n"
                        "   n number of frame\n"
                        "   T duration of the animation\n"
                        "-p followed by the params of the struct to be exported (can't be called if -e or -eq isn't called before)\n"
                        "   Params: dShaft stroke lRod wRod hPiston dPiston angle(defult value 0) for device (for details see README)\n"
                        "           n bore displacement angle(defult value 0) for motor                      (for details see README)\n"
                        "More following params will be ignored";
        cout << help << endl;
        return NULL;
    }

    else if(sargv[1] == "-device"){//device
    // ./mainentry -device -i /path_da_cui_importare -eq (n T) cx cy /path_dove_esportare -p <param>*7
        if(argc >= 4){
            if(sargv[2] == "-i"){ //import
                cout << "DEBUG: Importing device from " << sargv[3] << endl;
                string file = LBAMTTloadFromFile(sargv[3]);
                if(file == "") { //file not found
                    cout << "DEBUG: File not found" << endl;
                    return NULL;
                } 

                ret->device = LBAMTTdeviceFromStringSVG(file);

                if(ret->device == NULL) {  //string isn't a device
                    cout << "DEBUG: Can't import device from file " << sargv[3] <<", wrong format" << endl;
                    return NULL;
                }
                cout << "DEBUG: Import successful" << endl;

                if(argc >= 8){ //requested import + export
                    if(sargv[4] == "-e"){ //export
                        cout << "DEBUG: Exporting device on file " << sargv[7] << endl;
                        LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(ret->device, stod(sargv[5]), stod(sargv[6])), sargv[7]);
                    }
                    else if(sargv[4] == "-eq"){ //export with quotes
                        cout << "DEBUG: Exporting device with quotes on file " << sargv[7] << endl;
                        LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(ret->device, stod(sargv[5]), stod(sargv[6]), true), sargv[7]);
                    }
                    else if(sargv[4] == "-ea" && argc >= 10){ //export animated
                        cout << "DEBUG: Exporting device animated on file " << sargv[9] << endl;
                        LBAMTTanimation * anim = LBAMTTinitAnimation(stoi(sargv[5]), stod(sargv[6]));
                        LBAMTTsaveToFile(LBAMTTanimateDeviceSVG(ret->device, stod(sargv[7]), stod(sargv[8]), anim, true), sargv[9]);
                    }
                    cout << "DEBUG: Export successful" << endl;
                }
                else if(argc >= 5 && (sargv[4] == "-e" || sargv[4] == "-eq" || sargv[4] == "-ea")){ //more than 4 arguments but not enough to export
                    cout << "DEBUG: Missing arguments for export" << endl;
                }

                return ret;
            }

            else if(argc >= 13){// only export with params
                if(sargv[2] == "-e" || sargv[2] == "-eq"){ 
                    cout << "DEBUG: Exporting device with params on file " << sargv[5] << endl;
                    if(sargv[6] != "-p"){ //params not found
                        cout << "DEBUG: Params not found or wrong syntax" << endl;
                        return NULL;
                    } 

                    if(argc == 13) ret->device = LBAMTTinitDevice(stod(sargv[7]), stod(sargv[8]), stod(sargv[9]), stod(sargv[10]), stod(sargv[11]), stod(sargv[12]));
                    else if(argc >= 14) ret->device = LBAMTTinitDevice(stod(sargv[7]), stod(sargv[8]), stod(sargv[9]), stod(sargv[10]), stod(sargv[11]), stod(sargv[12]), stod(sargv[13]));

                    if(ret->device == NULL) { //params don't match constraints
                        cout << "DEBUG: Unable to init device with the given params, see README and check the constraints" << endl;
                        return NULL;
                    } 
                    if(sargv[2] == "-e") LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(ret->device, stod(sargv[3]), stod(sargv[4])), sargv[5]);
                    else if(sargv[2] == "-eq") LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(ret->device, stod(sargv[3]), stod(sargv[4]), true), sargv[5]);

                    cout << "DEBUG: Export successful" << endl;
                    
                    return ret;
                }
                else if(sargv[2] == "-ea"){
                    cout << "DEBUG: Exporting device animated with params on file " << sargv[7] << endl;
                    if(sargv[8] != "-p"){ //params not found
                        cout << "DEBUG: Params not found or wrong syntax" << endl;
                        return NULL;
                    } 
                    if(argc == 15) ret->device = LBAMTTinitDevice(stod(sargv[9]), stod(sargv[10]), stod(sargv[11]), stod(sargv[12]), stod(sargv[13]), stod(sargv[14]));
                    else if(argc >= 16) ret->device = LBAMTTinitDevice(stod(sargv[9]), stod(sargv[10]), stod(sargv[11]), stod(sargv[12]), stod(sargv[13]), stod(sargv[14]), stod(sargv[15]));

                    if(ret->device == NULL) { //params don't match constraints
                        cout << "DEBUG: Unable to init device with the given params, see README and check the constraints" << endl;
                        return NULL;
                    }
                    LBAMTTanimation * anim = LBAMTTinitAnimation(stoi(sargv[3]), stod(sargv[4]));
                    if(anim == NULL){
                        cout << "DEBUG: Unable to init animation with the given params, see README and check the constraints" << endl;
                        return ret; //still return the device
                    } 
                    LBAMTTsaveToFile(LBAMTTanimateDeviceSVG(ret->device, stod(sargv[5]), stod(sargv[6]), anim), sargv[7]);
                    cout << "DEBUG: Export successful" << endl;
                    return ret;
                }
            }
        }
    }

    else if(sargv[1] == "-motor"){//motor
    // ./mainentry -motor -i /path_da_cui_importare -eq/ea (n,T) /path_dove_esportare -p <param>*4
        if(argc >= 4){
            if(sargv[2] == "-i"){ //import
                cout << "DEBUG: Importing motor from " << sargv[3] << endl;
                string file = LBAMTTloadFromFile(sargv[3]);
                if(file == "") { //file not found
                    cout << "DEBUG: File not found" << endl;
                    return NULL;
                } 

                ret->motor = LBAMTTmotorFromStringSVG(file);

                if(ret->motor == NULL) {  //string isn't a motor
                    cout << "DEBUG: Can't import motor from file " << sargv[3] <<", wrong format" << endl;
                    return NULL;
                }
                cout << "DEBUG: Import successful" << endl;

                if(argc >= 6){ //requested export
                    if(sargv[4] == "-e"){ //export
                        cout << "DEBUG: Exporting motor on file " << sargv[5] << endl;
                        LBAMTTsaveToFile(LBAMTTmotorToStringSVG(ret->motor), sargv[5]);
                    }
                    else if(sargv[4] == "-eq"){ //export with quotes
                        cout << "DEBUG: Exporting motor with quotes on file " << sargv[5] << endl;
                        LBAMTTsaveToFile(LBAMTTmotorToStringSVG(ret->motor, true), sargv[5]);
                    }
                    else if(sargv[4] == "-ea" && argc >= 8){ //export animated
                        cout << "DEBUG: Exporting motor animated on file " << sargv[7] << endl;
                        LBAMTTanimation * anim = LBAMTTinitAnimation(stoi(sargv[5]), stod(sargv[6]));
                        LBAMTTsaveToFile(LBAMTTanimateMotorSVG(ret->motor, anim, true), sargv[7]);
                    }
                    cout << "DEBUG: Export successful" << endl;
                }
                else if(argc >= 5 && (sargv[4] == "-e" || sargv[4] == "-eq" || sargv[4] == "-ea")){ //more than 4 arguments but not enough to export
                    cout << "DEBUG: Missing arguments for export" << endl;
                }
                return ret;
            }

            else if(argc >= 8){
                if(sargv[2] == "-e" || sargv[2] == "-eq"){ //export with params
                    cout << "DEBUG: Exporting motor with params on file " << sargv[3] << endl;
                    if(sargv[4] != "-p"){ //params not found
                        cout << "DEBUG: Params not found" << endl;
                        return NULL;
                    } 

                    if(argc == 8) ret->motor = LBAMTTinitMotor(stoi(sargv[5]), stod(sargv[6]), stod(sargv[7]));
                    else if(argc >= 9)ret->motor = LBAMTTinitMotor(stoi(sargv[5]), stod(sargv[6]), stod(sargv[7]), stod(sargv[8]));

                    if(ret->motor == NULL) { //params don't match constraints
                        cout << "DEBUG: Unable to init motor with the given params, see README and check the constraints" << endl;
                        return NULL;
                    } 
                    if(sargv[2] == "-e") LBAMTTsaveToFile(LBAMTTmotorToStringSVG(ret->motor), sargv[3]);
                    else LBAMTTsaveToFile(LBAMTTmotorToStringSVG(ret->motor, true), sargv[3]);
                    cout << "DEBUG: Export successful" << endl;
                    
                    return ret;
                }
                else if(sargv[2] == "-ea"){
                    cout << "DEBUG: Exporting motor animated with params on file " << sargv[5] << endl;
                    if(sargv[6] != "-p"){ //params not found
                        cout << "DEBUG: Params not found or wrong syntax" << endl;
                        return NULL;
                    } 
                    if(argc == 10) ret->motor = LBAMTTinitMotor(stod(sargv[7]), stod(sargv[8]), stod(sargv[9]));
                    else if(argc >= 11) ret->motor = LBAMTTinitMotor(stod(sargv[7]), stod(sargv[8]), stod(sargv[9]), stod(sargv[10]));

                    if(ret->motor == NULL) { //params don't match constraints
                        cout << "DEBUG: Unable to init motor with the given params, see README and check the constraints" << endl;
                        return NULL;
                    }
                    LBAMTTanimation * anim = LBAMTTinitAnimation(stoi(sargv[3]), stod(sargv[4]));
                    if(anim == NULL){
                        cout << "DEBUG: Unable to init animation with the given params, see README and check the constraints" << endl;
                        return ret; //still return the motor
                    } 
                    LBAMTTsaveToFile(LBAMTTanimateMotorSVG(ret->motor, anim, true), sargv[5]);
                    cout << "DEBUG: Export successful" << endl;
                    return ret;
                }
            }
        }
    }

    cout << "command not found or too few arguments, type -h to display the helper" << endl;

    return NULL;
}

string LBAMTTanimateCylinderSVG(LBAMTTcylinder * cylinder, double cxShaft, double cyShaft, LBAMTTanimation * anim, bool header){
    if(cylinder == NULL) return "";
    
    double maxPistonY = cyShaft - cylinder->piston->stroke/2 - cylinder->piston->lRod - cylinder->piston->hPiston + cylinder->piston->wRod*7/10;
    double additionalY = 5; //additional height for the combustion chamber to not touch the valves
    double valveSpace = cylinder->valveDx->rMax - cylinder->valveDx->rMin; //valve Y movement
    double lenValve = cylinder->valveSx->lenValve;
    double cyValve =  maxPistonY - additionalY - cylinder->valveSx->rMax - lenValve * 1.1;

    string cylinderSVG = "";

    //combustion chamber
    //horizontal line
    cylinderSVG +=  LBAMTTlineSVG(  cxShaft - cylinder->piston->dPiston/2 -2, 
                                    maxPistonY - additionalY - valveSpace - lenValve/10 -1,
                                    cxShaft + cylinder->piston->dPiston/2 +2, 
                                    maxPistonY - additionalY - valveSpace - lenValve/10 -1);
    cylinderSVG += "\n";
    //vertical lines
    cylinderSVG +=  LBAMTTlineSVG(  cxShaft - cylinder->piston->dPiston/2 -1, 
                                    maxPistonY - additionalY - valveSpace - lenValve/10 -2,
                                    cxShaft - cylinder->piston->dPiston/2 -1, 
                                    maxPistonY + cylinder->piston->stroke + additionalY);
    cylinderSVG += "\n";
    cylinderSVG +=  LBAMTTlineSVG(  cxShaft + cylinder->piston->dPiston/2 +1, 
                                    maxPistonY - additionalY - valveSpace - lenValve/10 -2,
                                    cxShaft + cylinder->piston->dPiston/2 +1, 
                                    maxPistonY + cylinder->piston->stroke + additionalY);
    cylinderSVG += "\n";

    for(int i = 0; i < anim->n; i++){
        LBAMTTanimSetIndex(anim, i);
        cylinder->piston->angle += 720.0 / anim->n;
        cylinder->piston->angle = LBAMTTnormAng(cylinder->piston->angle, 720);
        cylinder->valveSx->Alpha = LBAMTTnormAng(PI*3/4 + (cylinder->piston->angle * PI/180 / 2), 2*PI);
        cylinder->valveDx->Alpha = LBAMTTnormAng(cylinder->valveSx->Alpha + PI/2, 2*PI);

        //piston
        cylinderSVG += LBAMTTdeviceToStringSVG(cylinder->piston, cxShaft, cyShaft, false, false, anim);

        //combustion chamber fill color
        /**
         * pistonAngle = 0 -> start compression: both valve closed
         * pistonAngle = 180 -> explosion: both valve closed
         * pistonAngle = 360 -> start expelling: valve Dx open, valve Sx closed
         * pistonAngle = 540 -> start aspiration: valve Sx open, valve Dx closed
        */
        double angle = cylinder->piston->angle;
        double r,g,b;
        double brightness = 225;
        if(angle < 180){ //compression -> yellow--red
            b = 0;
            g = brightness * (180 - angle) / 180;
            r = brightness;
        }
        else if(angle >= 180 && angle < 360){//expanding -> red--blue
            g = 0;
            r = brightness * (360 - angle) / 180;
            b = brightness * (angle - 180) / 180;
        }
        else if(angle >= 360 && angle < 540){ //expelling -> blue--green
            r = 0;
            b = brightness * (540 - angle) / 180;
            g = brightness * (angle - 360) / 180;
        }
        else {//aspiration -> green--yellow
            b = 0;
            r = brightness * (angle - 540) / 180;
            g = brightness;
        }
        string color = "rgb(" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ")";
        //piston height
        double L1 = cylinder->piston->stroke/2; //crank lenght
        double L2 = cylinder->piston->lRod;
        double q = PI/2 - cylinder->piston->angle * PI / 180.0; //crank angle in radiants
        double cyPistone = cyShaft - sqrt(pow(L2, 2) - pow(L1 * cos(q), 2)) + L1 * sin(q);
        //rectangle fill
        double rectBottom = maxPistonY - additionalY - valveSpace - lenValve/10;
        double rectTop = cyPistone + cylinder->piston->wRod*7/10 - cylinder->piston->hPiston;
        cylinderSVG += LBAMTTrectSVG(   cxShaft - cylinder->piston->dPiston/2, rectBottom,
                                        cylinder->piston->dPiston, rectTop - rectBottom,
                                        color, 0.0, 0.0, 0.0, LBAMTTappearSVG(color, anim));
        cylinderSVG += "\n";

        //valves
        //valveSx
        double cxValveSx = cxShaft - cylinder->piston->dPiston/4; 
        cylinderSVG += ENRICtoStringSVG(cylinder->valveSx, cxValveSx, cyValve, false, false, anim);
        cylinderSVG += "\n";
        //valveDx
        double cxValveDx = cxShaft + cylinder->piston->dPiston/4; 
        cylinderSVG += ENRICtoStringSVG(cylinder->valveDx, cxValveDx, cyValve, false, false, anim);
        cylinderSVG += "\n";
    }

    if(header){
        cylinderSVG = LBAMTTheaderSVG(cylinderSVG);
    }

    return cylinderSVG;
}

string LBAMTTanimateMotorSVG(LBAMTTmotor * motor, LBAMTTanimation * anim, bool header){
    if(motor == NULL) return "";

    double maxY = 480; // maxY for the cylinder to fit in the draw
    double wRod = motor->cylinders[0]->piston->wRod;
    double stroke = motor->cylinders[0]->piston->stroke;
    double distance = wRod*8/5 + stroke;
    
    string motorSVG = "";

    for(int i=0;i<motor->n;i++){
        double cxShaft0 = 400 - distance/2 * (motor->n-1);
        motorSVG += LBAMTTanimateCylinderSVG(motor->cylinders[i], cxShaft0 + distance*i, maxY, anim, false);
    }

    if(header){ //add SVG header with drawing dimensions
        motorSVG = LBAMTTheaderSVG(motorSVG);
    }

    return motorSVG;
}