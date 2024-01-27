#include <unistd.h>
#include <cstdio>

#include <Navio2/RCInput_Navio2.h>
#include <Common/Util.h>
#include <memory>

#include "../include/constantes.hpp"
#include "../include/keys.hpp"

#define READ_FAILED -1

#define NUM_CANALES 6

//Desire values
#define MIN_PITCH -45
#define MAX_PITCH 45
#define MIN_ROLL -45
#define MAX_ROLL 45
//YAW está en constantes.hpp
#define MIN_POWER 1200
#define MAX_POWER 1750

//CONTROLLER VALUES
//Estos valores son teniendo en cuenta que hay que setear a 0 los switches del mando
#define CONTROLLER_MAX 1104
#define CONTROLLER_MIN 1924

int map_value_to_range(int controller_value, int min_expected_value, int max_expected_value);

// int select_speed(int manditoSpeed);
// int pitch_roll_proportional(int value);
// int yaw_proportional(int value);