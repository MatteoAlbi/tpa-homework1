#include "catch2/catch2.hpp"
#include "LBAMTTmotor.h"

TEST_CASE("test LBAMTTsetOffsets when succeed and error occures","[LBAMTTmotor]"){
    LBAMTTmotor * motor = NULL;
    REQUIRE(LBAMTTsetOffsets(motor) == 1);

    motor = new LBAMTTmotor;
    motor->n = 1;
    REQUIRE(LBAMTTsetOffsets(motor) == 0);
    REQUIRE(motor->offset[0] == 0);

    motor->n = 2;
    REQUIRE(LBAMTTsetOffsets(motor) == 0);
    REQUIRE(motor->offset[0] == 0);
    REQUIRE(motor->offset[1] == 360.0);

    motor->n = 3;
    REQUIRE(LBAMTTsetOffsets(motor) == 0);
    REQUIRE(motor->offset[0] == 0);
    REQUIRE(motor->offset[1] == 240.0);
    REQUIRE(motor->offset[2] == 480.0);

    motor->n = 4;
    REQUIRE(LBAMTTsetOffsets(motor) == 0);
    REQUIRE(motor->offset[0] == 0);
    REQUIRE(motor->offset[1] == 180.0);
    REQUIRE(motor->offset[2] == 540.0);
    REQUIRE(motor->offset[3] == 360.0);

    motor->n = 5;
    REQUIRE(LBAMTTsetOffsets(motor) == 1);
}

TEST_CASE("test LBAMTTinitMotor, LBAMTTinitCylinder when succeed","[LBAMTTmotor]"){
    double bore = 150;
    double stroke = 120;
    double cylinderDisplacement = pow(bore/2,2)*PI * stroke;

    int n = 4;
    LBAMTTmotor * motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0); 
    REQUIRE(motor != NULL);
    REQUIRE(motor->n == n);
    REQUIRE(motor->angle == 0);
    REQUIRE(motor->cylinders[0] != NULL);
    REQUIRE(motor->cylinders[0]->piston != NULL);
    REQUIRE(motor->cylinders[0]->valveSx != NULL);
    REQUIRE(motor->cylinders[0]->valveDx != NULL);
    //piston values
    REQUIRE(motor->cylinders[0]->piston->dPiston == bore);
    REQUIRE(dblCompare(motor->cylinders[0]->piston->stroke, stroke));
    //valve values
    REQUIRE(dblCompare(motor->cylinders[0]->valveSx->rMax, stroke/6));
    REQUIRE(motor->cylinders[0]->valveDx->diamValve == bore*2/5);
    REQUIRE(motor->cylinders[0]->valveDx->Gamma == PI/8);

    REQUIRE(motor->cylinders[2] != NULL);
    REQUIRE(motor->cylinders[2]->piston != NULL);
    REQUIRE(motor->cylinders[2]->valveSx != NULL);
    REQUIRE(motor->cylinders[2]->valveDx != NULL);
    //piston values
    REQUIRE(motor->cylinders[2]->piston->dPiston == bore);
    REQUIRE(dblCompare(motor->cylinders[2]->piston->stroke, stroke));
    //valve values
    REQUIRE(dblCompare(motor->cylinders[2]->valveSx->rMax, stroke/6));
    REQUIRE(motor->cylinders[2]->valveDx->diamValve == bore*2/5);
    REQUIRE(motor->cylinders[2]->valveDx->Gamma == PI/8);
}

TEST_CASE("test LBAMTTdelete when succeed and error occures","[LBAMTTmotor]"){
    LBAMTTmotor * motor = NULL;
    REQUIRE(LBAMTTdelete(motor) == 1);

    int n = 3;
    double bore = 150;
    double stroke = 120;
    double cylinderDisplacement = pow(bore/2,2)*PI * stroke;
    motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0); 
    REQUIRE(LBAMTTdelete(motor) == 0);
}

TEST_CASE("test LBAMTTrotateMotor when succceed and error occures","[LBAMTTmotor]"){
    LBAMTTmotor * motor = NULL;
    REQUIRE(LBAMTTrotateMotor(motor) == 1);

    int n = 3;
    double bore = 150;
    double stroke = 120;
    double cylinderDisplacement = pow(bore/2,2)*PI * stroke;
    motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0);

    motor->angle = 180;
    LBAMTTrotateMotor(motor);
    REQUIRE(motor->cylinders[0]->piston->angle == 180);
    REQUIRE(motor->cylinders[1]->piston->angle == 180 + 240);
    REQUIRE(motor->cylinders[2]->piston->angle == 180 + 480);
    REQUIRE(motor->cylinders[0]->valveSx->Alpha == PI*3/4 + PI / 2);
    REQUIRE(motor->cylinders[0]->valveDx->Alpha == PI*3/4 + PI);
}

TEST_CASE("test LBAMTTsetMotorN when succceed and error occures","[LBAMTTmotor]"){
    LBAMTTmotor * motor = NULL;
    REQUIRE(LBAMTTsetMotorN(motor, 4) == 1);

    int n = 3;
    double bore = 150;
    double stroke = 120;
    double cylinderDisplacement = pow(bore/2,2)*PI * stroke;
    motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0);

    REQUIRE(LBAMTTsetMotorN(motor, -1) == 1); //invalid n: too low
    REQUIRE(motor->n == n);
    REQUIRE(motor->cylinders != NULL); 

    REQUIRE(LBAMTTsetMotorN(motor, 8) == 1); //invalid n: too high
    REQUIRE(motor->n == n);
    REQUIRE(motor->cylinders != NULL); 

    REQUIRE(LBAMTTsetMotorN(motor, 4) == 0); //valid
    REQUIRE(motor->n == 4);
    REQUIRE(motor->cylinders[2]->piston->angle == 540);
    REQUIRE(motor->cylinders[2]->piston->dPiston == bore);
    REQUIRE(dblCompare(motor->cylinders[2]->piston->stroke, stroke));    
}

TEST_CASE("test LBAMTTsetMotorBore when succceed and error occures","[LBAMTTmotor]"){
    LBAMTTmotor * motor = NULL;
    REQUIRE(LBAMTTsetMotorBore(motor, 150) == 1);

    int n = 2;
    double bore = 150;
    double stroke = 100;
    double cylinderDisplacement = pow(bore/2,2)*PI * stroke;
    motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0);

    REQUIRE(LBAMTTsetMotorBore(motor, 40) == 1); //invalid bore: too low 
    REQUIRE(motor->cylinders[0]->piston->dPiston == bore); 

    REQUIRE(LBAMTTsetMotorBore(motor, 250) == 1); //invalid ratio: too high
    REQUIRE(motor->cylinders[0]->piston->dPiston == bore); 

    REQUIRE(LBAMTTsetMotorBore(motor, 65) == 1); //invalid ratio: too low
    REQUIRE(motor->cylinders[0]->piston->dPiston == bore); 

    REQUIRE(LBAMTTsetMotorBore(motor, 400) == 1); //invalid bore: too high
    REQUIRE(motor->cylinders[0]->piston->dPiston == bore); 

    REQUIRE(LBAMTTsetMotorBore(motor, 200) == 0); //valid
    REQUIRE(motor->cylinders[0]->piston->dPiston == 200); 
}

TEST_CASE("test LBAMTTsetMotorDisplacement when succceed and error occures","[LBAMTTmotor]"){
    LBAMTTmotor * motor = NULL;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, 20000) == 1);

    int n = 2;
    double bore = 100;
    double oldStroke = 80;
    double cylinderDisplacement = pow(bore/2,2)*PI * oldStroke;
    motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0);

    double stroke = 200;
    double displacement = pow(bore/2,2)*PI * stroke * n;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, displacement) == 1); //invalid stroke: too high
    REQUIRE(dblCompare(motor->cylinders[0]->piston->stroke, oldStroke)); 

    stroke = 35;
    displacement = pow(bore/2,2)*PI * stroke * n;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, displacement) == 1); //invalid ratio: too high
    REQUIRE(dblCompare(motor->cylinders[0]->piston->stroke, oldStroke)); 

    stroke = 150;
    displacement = pow(bore/2,2)*PI * stroke * n;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, displacement) == 1); //invalid ratio: too low
    REQUIRE(dblCompare(motor->cylinders[0]->piston->stroke, oldStroke)); 

    stroke = 100;
    displacement = pow(bore/2,2)*PI * stroke * n;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, displacement) == 0); //valid
    REQUIRE(dblCompare(motor->cylinders[0]->piston->stroke, stroke)); 
}

TEST_CASE("test LBAMTTsetMotorAngle when succceed and error occures","[LBAMTTmotor]"){
    LBAMTTmotor * motor = NULL;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, 360) == 1);

    int n = 2;
    double bore = 150;
    double stroke = 100;
    double cylinderDisplacement = pow(bore/2,2)*PI * stroke;
    motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0);

    LBAMTTsetMotorAngle(motor, 180);
    REQUIRE(motor->angle == 180);
    REQUIRE(motor->cylinders[0]->piston->angle == 180);
}

TEST_CASE("test LBAMTTmotorsCompare","[LBAMTTmotor]"){
    int nA = 2;
    double boreA = 150;
    double strokeA = 100;
    double displacementA = pow(boreA/2,2)*PI * strokeA * nA;
    LBAMTTmotor * a = LBAMTTinitMotor(nA, boreA, displacementA, 0);
    REQUIRE(LBAMTTmotorsCompare(a, a));

    REQUIRE(! LBAMTTmotorsCompare(a, NULL));
    REQUIRE(! LBAMTTmotorsCompare(NULL, a));

    int nB = 3;
    double boreB = 180;
    double strokeB = 120;
    double displacementB = pow(boreB/2,2)*PI * strokeB * nB;
    LBAMTTmotor * b = LBAMTTinitMotor(nB, boreB, displacementB, 0);
    REQUIRE(LBAMTTmotorsCompare(b, b));

    REQUIRE(! LBAMTTmotorsCompare(b, a));

    LBAMTTsetMotorBore(b, boreA);
    REQUIRE(! LBAMTTmotorsCompare(b, a));

    LBAMTTsetMotorN(b, nA);
    REQUIRE(! LBAMTTmotorsCompare(b, a));

    LBAMTTsetMotorDisplacement(b, displacementA);
    REQUIRE(LBAMTTmotorsCompare(b, a));

    LBAMTTsetMotorAngle(b, 180);
    REQUIRE(! LBAMTTmotorsCompare(b, a));
}

TEST_CASE("test LBAMTTcylinderToStringSVG when error occures", "[LBAMTTmotor]"){
    REQUIRE(LBAMTTcylinderToStringSVG(NULL,0,0) == "");
}

TEST_CASE("test LBAMTTmotorToStringSVG when error occures", "[LBAMTTmotor]"){
    REQUIRE(LBAMTTmotorToStringSVG(NULL,0,0) == "");
}

TEST_CASE("test LBAMTTmotorFromStringSVG when succeed", "[LBAMTTmotor]"){
    int n = 3;
    double bore = 120;
    double stroke = 120;
    double angle = 90;
    double displacement = PI * pow(bore/2,2) * stroke * n;

    LBAMTTmotor * motor = LBAMTTinitMotor(n, bore, displacement, angle);
    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor), "test.svg");

    REQUIRE(LBAMTTmotorsCompare(motor, LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg"))));

    remove("test.svg");
}