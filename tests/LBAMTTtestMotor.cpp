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
    double cylinderDisplacement = pow(bore,2)*PI * stroke;

    int n = 4;
    LBAMTTmotor * motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0); 
    REQUIRE(motor->n == n);
    REQUIRE(motor->angle == 0);
    REQUIRE(motor->cylinders[0] != NULL);
    REQUIRE(motor->cylinders[0]->piston != NULL);
    REQUIRE(motor->cylinders[0]->valveSx != NULL);
    REQUIRE(motor->cylinders[0]->valveDx != NULL);
    //piston values
    REQUIRE(motor->cylinders[0]->piston->dPiston == bore);
    REQUIRE(abs(motor->cylinders[0]->piston->stroke - stroke) < 0.01);
    //valve values
    REQUIRE(abs(motor->cylinders[0]->valveSx->rMax - stroke/6) < 0.01);
    REQUIRE(motor->cylinders[0]->valveDx->diamValve == bore*2/5);
    REQUIRE(motor->cylinders[0]->valveDx->Gamma == PI/8);

    REQUIRE(motor->cylinders[2] != NULL);
    REQUIRE(motor->cylinders[2]->piston != NULL);
    REQUIRE(motor->cylinders[2]->valveSx != NULL);
    REQUIRE(motor->cylinders[2]->valveDx != NULL);
    //piston values
    REQUIRE(motor->cylinders[2]->piston->dPiston == bore);
    REQUIRE(abs(motor->cylinders[2]->piston->stroke - stroke) < 0.01);
    //valve values
    REQUIRE(abs(motor->cylinders[2]->valveSx->rMax - stroke/6) < 0.01);
    REQUIRE(motor->cylinders[2]->valveDx->diamValve == bore*2/5);
    REQUIRE(motor->cylinders[2]->valveDx->Gamma == PI/8);
}

TEST_CASE("test LBAMTTdeleteMotor when succeed and error occures","[LBAMTTmotor]"){
    LBAMTTmotor * motor = NULL;
    REQUIRE(LBAMTTdeleteMotor(motor) == 1);

    int n = 3;
    double bore = 150;
    double stroke = 120;
    double cylinderDisplacement = pow(bore,2)*PI * stroke;
    motor = LBAMTTinitMotor(n, bore, cylinderDisplacement*n, 0); 
    REQUIRE(LBAMTTdeleteMotor(motor) == 0);

}

