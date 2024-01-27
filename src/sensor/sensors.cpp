#include "sensors.hpp"

int main(int argc, char *argv[]) {

    if (check_apm())
    {
        return 1;
    }

    Config config;
    auto session = expect<Session>(open(std::move(config)));
    
    //Sensores lsm
    auto publisherSLA = expect<Publisher>(session.declare_publisher(KEY_LSM_ACC));
    auto publisherSLM = expect<Publisher>(session.declare_publisher(KEY_LSM_MAG));
    auto publisherSLG = expect<Publisher>(session.declare_publisher(KEY_LSM_GYR));
    //sensor barometer
    auto publisherBP = expect<Publisher>(session.declare_publisher(KEY_BAR_PRE));
    auto publisherBT = expect<Publisher>(session.declare_publisher(KEY_BAR_TEM));
    //voltage
    auto publisherV = expect<Publisher>(session.declare_publisher(KEY_VOLTAGE));

    auto lsm = std::unique_ptr<InertialSensor>{new LSM9DS1()};
    
    MS5611 barometer;
    barometer.initialize();

    auto adc = std::unique_ptr <ADC>{ new ADC_Navio2() };
    adc->initialize();
    float results[adc->get_channel_count()] = {0.0f};

    if (!lsm->probe())
    {
        printf("Sensor not enabled o algo asi\n");
        return EXIT_FAILURE;
    }

    lsm->initialize();

    float a[3];
    float g[3];
    float m[3];

    //-------------------------------------------------------------------------

    while (1)
    {
        //LSM
        lsm->update();
        lsm->read_accelerometer(&a[0], &a[1], &a[2]);
        lsm->read_gyroscope(&g[0], &g[1], &g[2]);
        lsm->read_magnetometer(&m[0], &m[1], &m[2]);
        
        publisherSLA.put(cast(a));
        publisherSLG.put(cast(g));
        publisherSLM.put(cast(m));


        //Barometer
        barometer.refreshPressure();
        usleep(10000); // Waiting for pressure data ready
        barometer.readPressure();

        barometer.refreshTemperature();
        usleep(10000); // Waiting for temperature data ready
        barometer.readTemperature();

        barometer.calculatePressureAndTemperature();

        publisherBT.put(std::to_string(barometer.getTemperature()));
        publisherBP.put(std::to_string(barometer.getPressure()));

        for (int i = 0; i < adc->get_channel_count(); i++)
        {
            results[i] = adc->read(i);
            if (results[i] == READ_FAILED)
                return EXIT_FAILURE;
            printf("A%d: %.4fV ", i, results[i] / 1000);
        }
        printf("\n");

        usleep(500000);//TODO seleccionar un sleep adecuado, igual es ese no sé
    }
}

/*
*Función para pasar de un array de floats a string separado por espacios
*/
std::string cast(float f[3]){
    std::ostringstream ss;
    ss << f[0] << " " << f[1] << " " << f[2];
    std::string s(ss.str());
    return s;
}