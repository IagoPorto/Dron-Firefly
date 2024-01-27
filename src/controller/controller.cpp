#include "controller.hpp"

int main (int argc, char *argv []) {

    if (check_apm ()) {

        return 1;
    }

    auto rcin = std::unique_ptr <RCInput> {new RCInput_Navio2 ()};
    rcin->initialize ();

    Config config;
    auto session = expect <Session> (open (std::move (config)));
    
    auto publisherArmado = expect <Publisher> (session.declare_publisher (KEY_ARMADO)); 
    auto publisherPMotores = expect <Publisher> (session.declare_publisher (KEY_POTENCIA));
    auto publisherKamikaze = expect <Publisher> (session.declare_publisher (KEY_KAMIKAZE));
    auto publisherYawController = expect <Publisher>(session.declare_publisher(KEY_YAW_CONTROLLER));
    auto publisherRollController = expect <Publisher>(session.declare_publisher(KEY_ROLL_CONTROLLER));
    auto publisherPitchController = expect <Publisher>(session.declare_publisher(KEY_PITCH_CONTROLLER));


    std::ostringstream dataCanal;
    std::string sDataCanal;
    
    std::string canal_value[2];
    //std::ostringstream x;

    while (true) {

        // Canal 0 --> ROLL
        dataCanal.str("");
        sDataCanal = "";
        
        //dataCanal << pitch_roll_proportional(rcin-> read(0));
        dataCanal << map_value_to_range(rcin -> read(0), MIN_ROLL, MAX_ROLL) << " ";
        sDataCanal = sDataCanal + dataCanal.str();
        dataCanal.str("");
        
        dataCanal << map_value_to_range(rcin -> read(1), MIN_PITCH, MAX_PITCH) << " ";
        sDataCanal = sDataCanal + dataCanal.str();
        dataCanal.str("");
        
        dataCanal << map_value_to_range(rcin -> read(2), MAX_POWER, MIN_POWER) << " ";
        sDataCanal = sDataCanal + dataCanal.str();
        dataCanal.str("");
        
        dataCanal << map_value_to_range(rcin -> read(3), MIN_YAW, MAX_YAW);
        sDataCanal = sDataCanal + dataCanal.str();
        dataCanal.str("");
        
        publisherYawController.put(sDataCanal.c_str());

        // Canal 4 --> ARMADO
        dataCanal.str ("");
        dataCanal << rcin-> read (4);

        if(dataCanal.str() != canal_value[0]){
            publisherArmado.put (dataCanal.str ());
            canal_value[0] = dataCanal.str();
        }
        

        // Canal 5 --> KAMIKAZE
        dataCanal.str("");
        dataCanal << rcin->read(5);

        if(dataCanal.str() != canal_value[1]){
            publisherKamikaze.put(dataCanal.str());
            canal_value[1] = dataCanal.str();
        }

        sleep (0.001);
    }

    return 0;
}

/*
* Función para devolver el valor proporcional
*/

int map_value_to_range(int controller_value, int min_expected_value, int max_expected_value){
    float valor_proporcional = ((controller_value - CONTROLLER_MIN) * 1.0) / (CONTROLLER_MAX - CONTROLLER_MIN);
    valor_proporcional = (valor_proporcional * (max_expected_value - min_expected_value)) + min_expected_value;
    return static_cast<int>(valor_proporcional);
}
