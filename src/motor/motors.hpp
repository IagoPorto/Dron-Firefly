#include <unistd.h>
#include <Navio2/PWM.h>
#include <Navio2/RCOutput_Navio2.h>
#include <Common/Util.h>
#include <stdlib.h>

#include <memory>
#include <string>
#include <string_view>

#include "../include/constantes.hpp"
#include "../include/keys.hpp"

#define SERVO_MIN 1000 /*us*/
#define SERVO_MAX 1700 /*us*/
#define SERVO_MID 1050

#define PWM_OUTPUT_MOTOR_0 0
#define PWM_OUTPUT_MOTOR_1 1
#define PWM_OUTPUT_MOTOR_2 2
#define PWM_OUTPUT_MOTOR_3 3

#define SLEEP_PWM 8000

void handler_kamikaze(const Sample &sample);
void handler_armado (const Sample &sample);

void handler_motor0(const Sample &sample);
void handler_motor1(const Sample &sample);
void handler_motor2(const Sample &sample);
void handler_motor3(const Sample &sample);

KeyExprView exprArm (KEY_ARMADO);
KeyExprView exprPot(KEY_POTENCIA);
KeyExprView exprKam(KEY_KAMIKAZE);
KeyExprView exprMotor0(KEY_MOTOR_0);
KeyExprView exprMotor1(KEY_MOTOR_1);
KeyExprView exprMotor2(KEY_MOTOR_2);
KeyExprView exprMotor3(KEY_MOTOR_3);

auto pwm = std::unique_ptr <RCOutput> {new RCOutput_Navio2 ()};

int power_motor_0 = 0;
int power_motor_1 = 0;
int power_motor_2 = 0;
int power_motor_3 = 0;

bool armado = false;