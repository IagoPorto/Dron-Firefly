#include "pid.hpp"
#include <bits/stdc++.h>

void controller_parser(std::string s);
void sensors_parser(std::string s);
std::string controller_values = "";
std::string sensors_values = "";

int main(){
     
     auto session = expect<Session>(open(std::move(config)));

     KeyExprView exprKP(KEY_KP);
     KeyExprView exprKI(KEY_KI);
     KeyExprView exprKD(KEY_KD);

     dkp = 2;
     dki = 0.15;
     dkd = 0.15;

     int count = 0;

     auto subscriberKP = expect<Subscriber>(session.declare_subscriber(exprKP, handler_kp));
     auto subscriberKI = expect<Subscriber>(session.declare_subscriber(exprKI, handler_ki));
     auto subscriberKD = expect<Subscriber>(session.declare_subscriber(exprKD, handler_kd));

     auto subscriberYaw = expect<Subscriber>(session.declare_subscriber(exprYaw, handler_yaw));
     auto subscriberYawController = expect<Subscriber>(session.declare_subscriber(exprYawController, handler_yaw_controller));

     auto publisherMotor0 = expect <Publisher> (session.declare_publisher(KEY_MOTOR_0)); 
     auto publisherMotor1 = expect <Publisher> (session.declare_publisher(KEY_MOTOR_1)); 
     auto publisherMotor2 = expect <Publisher> (session.declare_publisher(KEY_MOTOR_2)); 
     auto publisherMotor3 = expect <Publisher> (session.declare_publisher(KEY_MOTOR_3)); 

     init_variables();

     while(true){

          count++;

          if(count == 10000){
               Roll[INTEGRAL] = 0;
               Pitch[INTEGRAL] = 0;
               count = 0;
          }

          PID_R(dRoll, dRollController);
          PID_P(dPitch, dPitchController);
          
          yawMotors();
          rollMotors();
          pitchMotors();

          mixer();

          publisherMotor0.put(std::to_string(dFinalMotors[0]));
          publisherMotor1.put(std::to_string(dFinalMotors[1]));
          publisherMotor2.put(std::to_string(dFinalMotors[2]));
          publisherMotor3.put(std::to_string(dFinalMotors[3]));

          sleep(DELTA_TIME);
     }
}

void PID_P(double sensor_value, double controller_value){

     Pitch[ERROR] = controller_value - sensor_value;

     Pitch[PROPORTIONAL] = dkp * Pitch[ERROR];//dkp
     Pitch[DERIVATE] = dkd * ((Pitch[ERROR] - Pitch[OLD]) / DELTA_TIME);//dkd
     Pitch[INTEGRAL] = (Pitch[INTEGRAL] + (Pitch[ERROR] * DELTA_TIME)); //dki

     Pitch[RESULT] = Pitch[PROPORTIONAL] + (dki * Pitch[INTEGRAL]) + Pitch[DERIVATE];
     Pitch[OLD] = Pitch[ERROR];     
}

void PID_R(double sensor_value, double controller_value){

     Roll[ERROR] = controller_value - sensor_value;

     Roll[PROPORTIONAL] = dkp * Roll[ERROR];//dkp
     Roll[DERIVATE] = dkd * ((Roll[ERROR] - Roll[OLD]) / DELTA_TIME);//dkd
     Roll[INTEGRAL] = (Roll[INTEGRAL] + (Roll[ERROR] * DELTA_TIME)); //dki

     Roll[RESULT] = Roll[PROPORTIONAL] + (dki * Roll[INTEGRAL]) + Roll[DERIVATE];

     Roll[OLD] = Roll[ERROR];     
}

void yawMotors(){
     
     int valuePlus = map_value_to_range(dYawController, MIN_YAW, MAX_YAW, DESIRE_VALUE_YAW_MIN, DESIRE_VALUE_YAW_MAX);
     dYawMotors[0] = valuePlus;
     dYawMotors[1] = valuePlus;
     dYawMotors[2] = -1 * valuePlus;
     dYawMotors[3] = -1 * valuePlus;
}

void pitchMotors(){
     int valuePlus = map_value_to_range(Pitch[RESULT], PID_MIN_PITCH, PID_MAX_PITCH, DESIRE_VALUE_PITCH_MIN, DESIRE_VALUE_PITCH_MAX);
     dPitchMotors[0] = valuePlus;
     dPitchMotors[1] = -1 * valuePlus;
     dPitchMotors[2] = valuePlus;
     dPitchMotors[3] = -1 * valuePlus;
}

void rollMotors(){
     int valuePlus = map_value_to_range(Roll[RESULT], PID_MIN_ROLL, PID_MAX_ROLL, DESIRE_VALUE_ROLL_MIN, DESIRE_VALUE_ROLL_MAX);
     int valueMinus = map_value_to_range(-1 * Roll[RESULT], PID_MIN_ROLL, PID_MAX_ROLL, DESIRE_VALUE_ROLL_MIN, DESIRE_VALUE_ROLL_MAX);
     dRollMotors[0] = valuePlus;
     dRollMotors[1] = -1 * valuePlus;
     dRollMotors[2] = -1 * valuePlus;
     dRollMotors[3] = valuePlus;
}

void mixer(){
     dFinalMotors[0] = dPowerController + dYawMotors[0] + dRollMotors[0] + dPitchMotors[0];// - 25;
     dFinalMotors[1] = dPowerController + dYawMotors[1] + dRollMotors[1] + dPitchMotors[1];// - 25;
     dFinalMotors[2] = dPowerController + dYawMotors[2] + dRollMotors[2] + dPitchMotors[2];
     dFinalMotors[3] = dPowerController + dYawMotors[3] + dRollMotors[3] + dPitchMotors[3];

     int i = 0;
     for (i; i < 4; i++){

          //Protección para no quemar motores si puede ser
          if(dFinalMotors[i] >= NO_SUPERAR){
               dFinalMotors[i] = NO_SUPERAR -1;//Por miedo 
          }

          //Protección para que no se pare el motor
          if(dFinalMotors[i] < 1200){
               dFinalMotors[i] = 1200;
          }
     }
}

void handler_yaw(const Sample &sample){
     sensors_values = std::string (sample.get_payload().as_string_view());
     sensors_parser(sensors_values);
}

void handler_yaw_controller(const Sample &sample){
     controller_values = std::string(sample.get_payload().as_string_view());
     controller_parser(controller_values);
}

void handler_kp(const Sample &sample){
     dkp = std::stod(std::string(sample.get_payload().as_string_view()));
}

void handler_ki(const Sample &sample){
     dki = std::stod(std::string(sample.get_payload().as_string_view()));
}

void handler_kd(const Sample &sample){
     dkd = std::stod(std::string(sample.get_payload().as_string_view()));
}

void init_variables(){
     int i = 0;
     for(i; i < RESULT; i++){
          Roll[i] = 0;
          Pitch[i] = 0;
          if(i < 4){
               dFinalMotors[i] = 0;
               dYawMotors[i] = 0;
               dRollMotors[i] = 0;
               dPitchMotors[i] = 0;
          }
     }
     dYaw = 0;
     dRoll = 0;
     dPitch = 0;
     dPowerController = 0;
     dYawController = 0;
     dPitchController = 0;
     dRollController = 0;
}

int map_value_to_range(int controller_value, int min_controller, int max_controller, int min_expected_value, int max_expected_value){
    float valor_proporcional = ((controller_value - min_controller) * 1.0) / (max_controller - min_controller);
    valor_proporcional = (valor_proporcional * (max_expected_value - min_expected_value)) + min_expected_value;
    return static_cast<int>(valor_proporcional);
}

void controller_parser(std::string s) {

     if(s.length() < 1){
          return;
     }

     // Use find function to find 1st position of delimiter.
     int end = s.find(" "); 
     dRollController = std::stod(s.substr(0, end ));
     s.erase(s.begin(), s.begin() + end + 1);
     end = s.find(" ");

     dPitchController = std::stod(s.substr(0, end));
     s.erase(s.begin(), s.begin() + end + 1);
     end = s.find(" ");

     dPowerController = std::stod(s.substr(0, end ));
     s.erase(s.begin(), s.begin() + end + 1);
     end = s.find(" ");

     dYawController = std::stod(s);
}

void sensors_parser(std::string s) {

     if (s.length() < 1)
     {
          return;
     }

     // Use find function to find 1st position of delimiter.
     int end = s.find(" ");
     dPitch = std::stod(s.substr(0, end));
     s.erase(s.begin(), s.begin() + end + 1);

     end = s.find(" ");
     dRoll = std::stod(s.substr(0, end));
     s.erase(s.begin(), s.begin() + end + 1);

     dYaw = std::stod(s);
}
