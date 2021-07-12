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
    REQUIRE(LBAMTTdblCompare(motor->cylinders[0]->piston->stroke, stroke));
    //valve values
    REQUIRE(LBAMTTdblCompare(motor->cylinders[0]->valveSx->rMax, stroke/6));
    REQUIRE(motor->cylinders[0]->valveDx->diamValve == bore*2/5);
    REQUIRE(motor->cylinders[0]->valveDx->Gamma == PI/8);

    REQUIRE(motor->cylinders[2] != NULL);
    REQUIRE(motor->cylinders[2]->piston != NULL);
    REQUIRE(motor->cylinders[2]->valveSx != NULL);
    REQUIRE(motor->cylinders[2]->valveDx != NULL);
    //piston values
    REQUIRE(motor->cylinders[2]->piston->dPiston == bore);
    REQUIRE(LBAMTTdblCompare(motor->cylinders[2]->piston->stroke, stroke));
    //valve values
    REQUIRE(LBAMTTdblCompare(motor->cylinders[2]->valveSx->rMax, stroke/6));
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
    REQUIRE(LBAMTTdblCompare(motor->cylinders[0]->valveSx->Alpha, PI*3/4 + PI / 2));
    REQUIRE( motor->cylinders[0]->valveDx->Alpha == PI*3/4 + PI);
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
    REQUIRE(LBAMTTdblCompare(motor->cylinders[2]->piston->stroke, stroke));    
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
    REQUIRE(LBAMTTdblCompare(motor->cylinders[0]->piston->stroke, oldStroke)); 

    stroke = 35;
    displacement = pow(bore/2,2)*PI * stroke * n;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, displacement) == 1); //invalid ratio: too high
    REQUIRE(LBAMTTdblCompare(motor->cylinders[0]->piston->stroke, oldStroke)); 

    stroke = 150;
    displacement = pow(bore/2,2)*PI * stroke * n;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, displacement) == 1); //invalid ratio: too low
    REQUIRE(LBAMTTdblCompare(motor->cylinders[0]->piston->stroke, oldStroke)); 

    stroke = 100;
    displacement = pow(bore/2,2)*PI * stroke * n;
    REQUIRE(LBAMTTsetMotorDisplacement(motor, displacement) == 0); //valid
    REQUIRE(LBAMTTdblCompare(motor->cylinders[0]->piston->stroke, stroke)); 
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

TEST_CASE("test LBAMTTmotorCompare","[LBAMTTmotor]"){
    int nA = 2;
    double boreA = 150;
    double strokeA = 100;
    double displacementA = pow(boreA/2,2)*PI * strokeA * nA;
    LBAMTTmotor * a = LBAMTTinitMotor(nA, boreA, displacementA, 0);
    REQUIRE(LBAMTTmotorCompare(a, a));

    REQUIRE(! LBAMTTmotorCompare(a, NULL));
    REQUIRE(! LBAMTTmotorCompare(NULL, a));

    int nB = 3;
    double boreB = 180;
    double strokeB = 120;
    double displacementB = pow(boreB/2,2)*PI * strokeB * nB;
    LBAMTTmotor * b = LBAMTTinitMotor(nB, boreB, displacementB, 0);
    REQUIRE(LBAMTTmotorCompare(b, b));

    REQUIRE(! LBAMTTmotorCompare(b, a));

    LBAMTTsetMotorBore(b, boreA);
    REQUIRE(! LBAMTTmotorCompare(b, a));

    LBAMTTsetMotorN(b, nA);
    REQUIRE(! LBAMTTmotorCompare(b, a));

    LBAMTTsetMotorDisplacement(b, displacementA);
    REQUIRE(LBAMTTmotorCompare(b, a));
}

TEST_CASE("test LBAMTTcylinderToStringSVG when error occures", "[LBAMTTmotor]"){
    REQUIRE(LBAMTTcylinderToStringSVG(NULL,0,0) == "");
}

TEST_CASE("test LBAMTTmotorToStringSVG when error occures", "[LBAMTTmotor]"){
    REQUIRE(LBAMTTmotorToStringSVG(NULL,0,0) == "");
}

TEST_CASE("test LBAMTTmotorFromStringSVG when succeed and error occures", "[LBAMTTmotor]"){
    int n = 3;
    double bore = 120;
    double stroke = 120;
    double angle = 90;
    double displacement = PI * pow(bore/2,2) * stroke * n;

    LBAMTTmotor * motor = LBAMTTinitMotor(n, bore, displacement, angle);

    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor), "test.svg");
    REQUIRE(LBAMTTmotorCompare(motor, LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg"))));

    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor, true), "test.svg");
    REQUIRE(LBAMTTmotorCompare(motor, LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg"))));

    LBAMTTsaveToFile(LBAMTTanimateMotorSVG(motor, LBAMTTinitAnimation(120, 6), true), "test.svg");
    REQUIRE(LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg")) != NULL);
    REQUIRE(LBAMTTmotorCompare(motor, LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg"))));

    LBAMTTsaveToFile("test", "test.svg");
    REQUIRE(LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg")) == NULL);

    remove("test.svg");
}

char** s_arr2c_arr(string * s_arr, int n){
    char ** c_arr = new char*[n];
    for(int i=0; i<n; i++){
        c_arr[i] = new char[s_arr[i].length()+1];
        strcpy(c_arr[i], s_arr[i].c_str());
    }    
    return c_arr;
}

TEST_CASE("test LBAMTTcommandLineParam, helper", "[LBAMTTmotor]"){
    int argc = 3;
    string s_arr[] = {"./mainentry", "-h", "test.svg"};
    char **argv = s_arr2c_arr(s_arr, argc);
    
    cout << "\nhelper display:\n";
    LBAMTTcommandLineParam(argc, argv);
    cout << "\n";
}

TEST_CASE("test LBAMTTcommandLineParam, import device request", "[LBAMTTmotor]"){
    LBAMTTcmdlineRet * out;
    char **argv;
    int argc = 4;
    string s_arr_d[] = {"./mainentry", "-device", "-i", "test.svg"};
    argv = s_arr2c_arr(s_arr_d, argc);
    
    //device
    LBAMTTdevice *device_in = LBAMTTinitDevice(80, 200, 300, 60, 100, 100);
    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device_in, 400, 400, true), "test.svg");

    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out->device != NULL); //import successful
    REQUIRE(LBAMTTdeviceCompare(out->device, device_in));

    LBAMTTsaveToFile(LBAMTTanimateDeviceSVG(device_in, 400, 400, LBAMTTinitAnimation(120, 6), true), "test.svg");
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out->device != NULL); //import successful
    REQUIRE(LBAMTTdeviceCompare(out->device, device_in));

    LBAMTTsaveToFile("test", "test.svg");
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //not a motor/device

    remove("test.svg");
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //file not found
}

TEST_CASE("test LBAMTTcommandLineParam, import motor request", "[LBAMTTmotor]"){
    LBAMTTcmdlineRet * out;
    char **argv;
    int argc = 4;
    //motor
    LBAMTTmotor *motor_in = LBAMTTinitMotor(4, 140, 8620530, 90);
    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor_in, true), "test.svg");

    string s_arr_m[] = {"./mainentry", "-motor", "-i", "test.svg"};
    argv = s_arr2c_arr(s_arr_m, argc);
    
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out->motor != NULL); //import successful
    REQUIRE(LBAMTTmotorCompare(out->motor, motor_in));

    LBAMTTsaveToFile(LBAMTTanimateMotorSVG(motor_in, LBAMTTinitAnimation(120, 6), true), "test.svg");
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out->motor != NULL); //import successful
    REQUIRE(LBAMTTmotorCompare(out->motor, motor_in));
}

TEST_CASE("test LBAMTTcommandLineParam, export device with params request", "[LBAMTTmotor]"){
    char **argv; 
    LBAMTTcmdlineRet * out;
    
    double dShaft = 120;
    double stroke = 300;
    double lRod = 300;
    double wRod = 60;
    double hPiston = 100;
    double dPiston = 150;
    double angle = 90;
    LBAMTTdevice *device_in = LBAMTTinitDevice(dShaft, stroke, lRod, wRod, hPiston, dPiston, angle);
    string s_arr_d[] = {"./mainentry", "-device", "-eq", "400", "200", "test.svg", "-p", to_string(dShaft), to_string(stroke), to_string(lRod), 
                                                                            to_string(wRod), to_string(hPiston), to_string(dPiston), 
                                                                            to_string(angle)};
    int argc = 14;
    argv = s_arr2c_arr(s_arr_d, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out->device != NULL);
    REQUIRE(LBAMTTdeviceCompare(out->device, device_in));

    //animated
    s_arr_d[2] = "-ea";
    argv = s_arr2c_arr(s_arr_d, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //missing anim params
    string s_arr_da[] = {"./mainentry", "-device", "-ea", "120", "6", "400", "200", "test.svg", "-p", to_string(dShaft), to_string(stroke), to_string(lRod), 
                                                                            to_string(wRod), to_string(hPiston), to_string(dPiston), 
                                                                            to_string(angle)};
    argc = 16;
    argv = s_arr2c_arr(s_arr_da, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out->device != NULL);
    REQUIRE(LBAMTTdeviceCompare(out->device, device_in));
    
    argc = 14;
    s_arr_d[2] = "-eq";
    s_arr_d[10] = "600";
    argv = s_arr2c_arr(s_arr_d, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //constraints not respected

    s_arr_d[10] = to_string(wRod);
    s_arr_d[6] = "-q";
    argv = s_arr2c_arr(s_arr_d, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //missing params

    argc = 12;
    argv = s_arr2c_arr(s_arr_d, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //missing argument

    remove("test.svg");
}

TEST_CASE("test LBAMTTcommandLineParam, export motor with params request", "[LBAMTTmotor]"){
    char **argv; 
    LBAMTTcmdlineRet * out;

    int n = 4;
    double bore = 140;
    double displacement = 8620530;
    double angle = 90;
    LBAMTTmotor *motor_in = LBAMTTinitMotor(n, bore, displacement, angle);
    string s_arr_m[] = {"./mainentry", "-motor", "-eq", "test.svg", "-p", to_string(n), to_string(bore), to_string(displacement), to_string(angle)};

    int argc = 9;
    argv = s_arr2c_arr(s_arr_m, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out->motor != NULL);
    REQUIRE(LBAMTTmotorCompare(out->motor, motor_in));

    //animated
    s_arr_m[2] = "-ea";
    argv = s_arr2c_arr(s_arr_m, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //missing anim params
    string s_arr_ma[] = {"./mainentry", "-motor", "-ea", "120", "6", "test.svg", "-p", to_string(n), to_string(bore), 
                                                                                              to_string(displacement), to_string(angle)};
    argc = 11;
    argv = s_arr2c_arr(s_arr_ma, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out->motor != NULL);
    REQUIRE(LBAMTTmotorCompare(out->motor, motor_in));

    s_arr_m[2] = "-eq";
    argc = 9;
    s_arr_m[7] = "600";
    argv = s_arr2c_arr(s_arr_m, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //constraints not respected

    s_arr_m[7] = to_string(displacement);
    s_arr_m[4] = "-q";
    argv = s_arr2c_arr(s_arr_m, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //missing params

    argc = 7;
    argv = s_arr2c_arr(s_arr_m, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(out == NULL); //missing param

    remove("test.svg");
}

TEST_CASE("test LBAMTTcommandLineParam, import and export device request", "[LBAMTTmotor]"){
    LBAMTTdevice * device_in = LBAMTTinitDevice(80, 200, 300, 60, 100, 100);
    int argc = 15;
    string s_arr_d[] = {"./mainentry", "-device", "-i", "test.svg", "-eq", "400", "200", "test_copy.svg", "-p", "80", "200", "300", "60", "100", "80"};
    char **argv = s_arr2c_arr(s_arr_d, argc);

    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device_in, 400, 200, true, true), "test.svg");//with quotes
    LBAMTTcmdlineRet * out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(LBAMTTdeviceCompare(LBAMTTdeviceFromStringSVG(LBAMTTloadFromFile("test.svg")),LBAMTTdeviceFromStringSVG(LBAMTTloadFromFile("test_copy.svg"))));

    LBAMTTsaveToFile(LBAMTTdeviceToStringSVG(device_in, 400, 200), "test.svg");//without quotes
    s_arr_d[4] = "-e";
    argv = s_arr2c_arr(s_arr_d, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(LBAMTTdeviceCompare(LBAMTTdeviceFromStringSVG(LBAMTTloadFromFile("test.svg")),LBAMTTdeviceFromStringSVG(LBAMTTloadFromFile("test_copy.svg"))));

    LBAMTTsaveToFile(LBAMTTanimateDeviceSVG(device_in, 400, 200, LBAMTTinitAnimation(120, 6)), "test.svg");//animated
    string s_arr_da[] = {"./mainentry", "-device", "-i", "test.svg", "-ea", "240", "4", "400", "200", "test_copy.svg", "-p", "80", "200", "300", "60"};
    argv = s_arr2c_arr(s_arr_da, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(LBAMTTdeviceCompare(LBAMTTdeviceFromStringSVG(LBAMTTloadFromFile("test.svg")),LBAMTTdeviceFromStringSVG(LBAMTTloadFromFile("test_copy.svg"))));

    remove("test_copy.svg");

    argc = 7;
    argv = s_arr2c_arr(s_arr_d, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    ifstream fin_d("test_copy.svg");
    //not enough arguments to export
    REQUIRE(! fin_d.is_open()); //file not found
    REQUIRE(LBAMTTdeviceCompare(out->device, device_in)); //file still imported

    remove("test.svg");
}

TEST_CASE("test LBAMTTcommandLineParam, import and export motor request", "[LBAMTTmotor]"){
    LBAMTTmotor * motor_in = LBAMTTinitMotor(4, 140, 8620530, 90);
    int argc = 11;
    string s_arr_m[] = {"./mainentry", "-motor", "-i", "test.svg", "-e", "test_copy.svg", "-p", "4", "140", "8620530", "60"};
    
    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor_in, true), "test.svg");// with quotes
    char **argv = s_arr2c_arr(s_arr_m, argc);// -e
    LBAMTTcmdlineRet * out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(LBAMTTmotorCompare(LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg")),LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test_copy.svg"))));

    LBAMTTsaveToFile(LBAMTTanimateMotorSVG(motor_in, LBAMTTinitAnimation(120, 6), true), "test.svg"); //animated
    s_arr_m[4] = "-eq";
    argv = s_arr2c_arr(s_arr_m, argc); // -eq
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(LBAMTTmotorCompare(LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg")),LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test_copy.svg"))));

    LBAMTTsaveToFile(LBAMTTmotorToStringSVG(motor_in), "test.svg"); //without quotes
    string s_arr_ma[] = {"./mainentry", "-motor", "-i", "test.svg", "-ea", "240", "4", "test_copy.svg", "-p", "4", "140"};
    argv = s_arr2c_arr(s_arr_ma, argc); // -ea
    out = LBAMTTcommandLineParam(argc, argv);
    REQUIRE(LBAMTTmotorCompare(LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test.svg")),LBAMTTmotorFromStringSVG(LBAMTTloadFromFile("test_copy.svg"))));

    remove("test_copy.svg");

    argc = 5;
    argv = s_arr2c_arr(s_arr_m, argc);
    out = LBAMTTcommandLineParam(argc, argv);
    ifstream fin_m("test_copy.svg");
    //not enough arguments to export
    REQUIRE(! fin_m.is_open()); //file not found
    REQUIRE(LBAMTTmotorCompare(out->motor, motor_in)); //file still imported

    remove("test.svg");
    remove("test_copy.svg");
}