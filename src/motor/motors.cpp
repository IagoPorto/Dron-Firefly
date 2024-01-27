#include "motors.hpp"

int main (int argc, char *argv[]) {

    Config config;
    auto session = expect<Session>(open(std::move(config)));

    auto subscriberArmado = expect <Subscriber>(session.declare_subscriber(exprArm, handler_armado));
    auto subscriberKamikaze = expect<Subscriber>(session.declare_subscriber(exprKam, handler_kamikaze));
    
    auto subscriberMotor0 = expect<Subscriber>(session.declare_subscriber(exprMotor0, handler_motor0));
    auto subscriberMotor1 = expect<Subscriber>(session.declare_subscriber(exprMotor1, handler_motor1));
    auto subscriberMotor2 = expect<Subscriber>(session.declare_subscriber(exprMotor2, handler_motor2));
    auto subscriberMotor3 = expect<Subscriber>(session.declare_subscriber(exprMotor3, handler_motor3));
    
    if (check_apm()) {
        printf ("No checkee\n");
        return 1;
    }

    if (getuid()) {
        fprintf (stderr, "Not root. Please launch like this: sudo %s\n", argv[0]);
    }

    if (!(pwm->initialize(PWM_OUTPUT_MOTOR_1)) || !(pwm->initialize(PWM_OUTPUT_MOTOR_2)) ||
        !(pwm->initialize(PWM_OUTPUT_MOTOR_3)) || !(pwm->initialize(PWM_OUTPUT_MOTOR_0))) {
        return 1;
    }

    pwm->set_frequency(PWM_OUTPUT_MOTOR_1, 50);
    pwm->set_frequency(PWM_OUTPUT_MOTOR_2, 50);
    pwm->set_frequency(PWM_OUTPUT_MOTOR_3, 50);
    pwm->set_frequency(PWM_OUTPUT_MOTOR_0, 50);



    if (!(pwm->enable(PWM_OUTPUT_MOTOR_1)) || !(pwm->enable(PWM_OUTPUT_MOTOR_2)) ||
        !(pwm->enable(PWM_OUTPUT_MOTOR_3)) || !(pwm->enable(PWM_OUTPUT_MOTOR_0))) {

        printf ("No se pudieron inicializar los motores");
        return 1;
    }

    while(1) {

        if(armado) {

            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_0, power_motor_0);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_1, power_motor_1);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_2, power_motor_2);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_3, power_motor_3);    

        }
        
        usleep(1500);
    }
}

void handler_armado(const Sample &sample) {

    if(sample.get_payload ().as_string_view () == "964" && !armado) {

        std::cout << "Iniciando armado\n";

        for(int i = 0; i < 100; i++) {

            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_1, SERVO_MIN);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_2, SERVO_MIN);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_3, SERVO_MIN);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_0, SERVO_MIN);

            usleep(15000);
        }
        
        for(int i = 0; i < 100; i++) {

            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_1, SERVO_MID);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_2, SERVO_MID);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_3, SERVO_MID);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_0, SERVO_MID);

            usleep(15000);
        }

        for(int i = 0; i < 100; i++) {

            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_1, SERVO_MIN);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_2, SERVO_MIN);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_3, SERVO_MIN);
            pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_0, SERVO_MIN);

            usleep(15000);
        }

        armado = true;

        power_motor_0 = 1200;
        power_motor_1 = 1200;
        power_motor_2 = 1200;
        power_motor_3 = 1200;

        std::cout << "Ready to fly\n";
    }
}

void handler_kamikaze(const Sample &sample) {

    if (sample.get_payload().as_string_view() == "2064") {
        pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_1, SERVO_MIN);
        pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_2, SERVO_MIN);
        pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_3, SERVO_MIN);
        pwm->set_duty_cycle(PWM_OUTPUT_MOTOR_0, SERVO_MIN);
        exit (-1);
    }
}

void handler_motor0(const Sample &sample){

    if (armado) {
        std::string dataIn(sample.get_payload().as_string_view());
        power_motor_0 = std::stoi (dataIn);
    }
}

void handler_motor1(const Sample &sample){
        
    if (armado) {
        std::string dataIn(sample.get_payload().as_string_view());
        power_motor_1 = std::stoi (dataIn);
    }
}

void handler_motor2(const Sample &sample){
        
    if (armado) {
        std::string dataIn(sample.get_payload().as_string_view());
        power_motor_2 = std::stoi (dataIn);
    }
}

void handler_motor3(const Sample &sample){
        
    if (armado) {
        std::string dataIn(sample.get_payload().as_string_view());
        power_motor_3 = std::stoi (dataIn);
    }
}

