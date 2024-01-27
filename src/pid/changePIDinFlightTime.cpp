#include <stdlib.h>
#include <iostream>
#include <unistd.h> 
#include <string>
#include "../include/constantes.hpp"
#include "../include/keys.hpp"

void handler_kp(const Sample &sample);
void handler_ki(const Sample &sample);
void handler_kd(const Sample &sample);

double kp, ki, kd;

int main(){

    Config config;

    kp = 2;
    ki = 0.05;
    kd = 0.05;

    KeyExprView exprKP(KEY_KP);
    KeyExprView exprKI(KEY_KI);
    KeyExprView exprKD(KEY_KD);

    auto session = expect <Session> (open (std::move (config)));
    
    auto publisherKP = expect <Publisher> (session.declare_publisher (KEY_KP)); 
    auto publisherKI = expect <Publisher> (session.declare_publisher (KEY_KI)); 
    auto publisherKD = expect <Publisher> (session.declare_publisher (KEY_KD)); 

    auto subscriberKP = expect<Subscriber>(session.declare_subscriber(exprKP, handler_kp));
    auto subscriberKI = expect<Subscriber>(session.declare_subscriber(exprKI, handler_ki));
    auto subscriberKD = expect<Subscriber>(session.declare_subscriber(exprKD, handler_kd));

    int option;
    std::string value;
    

    while(true){
        printf("Selecciona variable: \n\t1. KP  --> actual value: %f\n\t2. KI  --> actual value: %f\n\t3. KD  --> actual value: %f\n", kp, ki, kd);
        scanf("%d", &option);

        switch(option){
            case 1:
                std::cout << "KP: ";
                std::cin >> value;
                publisherKP.put(value);
            break;
            case 2:
                std::cout << "KI: ";
                std::cin >> value;
                publisherKI.put(value);
            break;
            case 3:
                std::cout << "KD: ";
                std::cin >> value;
                publisherKD.put(value);
            break;
            default:
                std::cout << "Try again idiot!" << std::endl;
            break;
        }
        value = "";
        value.clear();
    }

    return 0;
}

void handler_kp(const Sample &sample){
     kp = std::stod(std::string(sample.get_payload().as_string_view()));
}

void handler_ki(const Sample &sample){
     ki = std::stod(std::string(sample.get_payload().as_string_view()));
}

void handler_kd(const Sample &sample){
     kd = std::stod(std::string(sample.get_payload().as_string_view()));
}