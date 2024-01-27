#include <stdlib.h>
#include <iostream>
#include <unistd.h> 
#include <string>
#include "../include/constantes.hpp"
#include "../include/keys.hpp"

#define DELTA_TIME 0.001 //s
#define DELTA_TIME_MS 10

#define PROPORTIONAL 0
#define INTEGRAL 1
#define DERIVATE 2
#define ERROR 3
#define OLD 4
#define NEW 5
#define RESULT 6

#define PID_MIN_PITCH -130
#define PID_MAX_PITCH 130
#define PID_MIN_ROLL -138
#define PID_MAX_ROLL 138

#define DESIRE_VALUE_PITCH_MAX 100
#define DESIRE_VALUE_PITCH_MIN -100
#define DESIRE_VALUE_ROLL_MAX 100
#define DESIRE_VALUE_ROLL_MIN -100
#define DESIRE_VALUE_YAW_MAX 50
#define DESIRE_VALUE_YAW_MIN -50

//PID TERMS
#define KP 1  //  TODO tuning
#define KI 0.5//  TODO tuning
#define KD 0.5//  TODO tuning

// void pitch();
// void roll();
void PID(double *YRP, double sensor_value, double controller_value);
void PID_P(double sensor_value, double controller_value);
void PID_R(double sensor_value, double controller_value);

int map_value_to_range(int controller_value, int min_controller, int max_controller, int min_expected_value, int max_expected_value);

//void PID(double YRP[], double sensor_value, double controller_value);

void yawMotors();
void rollMotors();
void pitchMotors();

void mixer();

// void handler_sma(const Sample &sample);
// void handler_smg(const Sample &sample);
// void handler_smm(const Sample &sample);
void handler_kp(const Sample &sample);
void handler_ki(const Sample &sample);
void handler_kd(const Sample &sample);

void handler_bp(const Sample &sample);
void handler_bt(const Sample &sample);
void handler_yaw(const Sample &sample);
// void handler_roll(const Sample &sample);
// void handler_pitch(const Sample &sample);
void handler_yaw_controller(const Sample &sample);
// void handler_roll_controller(const Sample &sample);
// void handler_pitch_controller(const Sample &sample);
void handler_power_controller(const Sample &sample);

void init_variables();

double Roll[7];// 0: proportional, 1: integral, 2: derivate, 3: error, 4: old, 5: new, 6:SUMA
double Pitch[7];

double dYawMotors[4];
double dRollMotors[4];
double dPitchMotors[4];

double dFinalMotors[4];

Config config;

KeyExprView exprAcc(KEY_LSM_ACC);
KeyExprView exprMag(KEY_LSM_MAG);
KeyExprView exprGyr(KEY_LSM_GYR);
KeyExprView exprBP(KEY_BAR_PRE);
KeyExprView exprBT(KEY_BAR_TEM);
KeyExprView exprYaw(KEY_RTIMULIB_YAW);
KeyExprView exprRoll(KEY_RTIMULIB_ROLL);
KeyExprView exprPitch(KEY_RTIMULIB_PITCH);
KeyExprView exprYawController(KEY_YAW_CONTROLLER);
KeyExprView exprRollController(KEY_ROLL_CONTROLLER);
KeyExprView exprPitchController(KEY_PITCH_CONTROLLER);
KeyExprView exprPowerController(KEY_POTENCIA);

double dYaw;
double dRoll;
double dPitch;

double dkp, dki, dkd;

double dYawController;
double dRollController;
double dPitchController;
double dPowerController;

double dBPresion;
double dBTemperature;


